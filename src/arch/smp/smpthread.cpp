/*************************************************************************************/
/*      Copyright 2009 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/

#include "smpprocessor.hpp"
#include "schedule.hpp"
#include "debug.hpp"
#include "system.hpp"
#include <iostream>
#include <sched.h>

extern "C"
{
// low-level routine to switch stacks
   void switchStacks( void *,void *,void *,void * );
}

using namespace nanos;
using namespace nanos::ext;

void * smp_bootthread ( void *arg )
{
   SMPThread *self = static_cast<SMPThread *>( arg );

   self->run();

   pthread_exit ( 0 );
}

// TODO: detect at configure
#ifndef PTHREAD_STACK_MIN
#define PTHREAD_STACK_MIN 16384
#endif

void SMPThread::start ()
{
   pthread_attr_t attr;
   pthread_attr_init(&attr);

   // user-defined stack size
   if ( _stackSize > 0 ) {
	// TODO: check alignment?

	if ( _stackSize < PTHREAD_STACK_MIN ) {
	 	warning("specified thread stack too small, adjusting it to minimum size");
		_stackSize = PTHREAD_STACK_MIN;
	}

       char *stack = new char[_stackSize];

       if ( stack == NULL || pthread_attr_setstack( &attr, stack, _stackSize ) )
	 warning("couldn't create pthread stack");
   }


   if ( pthread_create( &_pth, &attr, smp_bootthread, this ) )
      fatal( "couldn't create thread" );
}

void SMPThread::runDependent ()
{
   WD &work = getThreadWD();
   setCurrentWD( work );

   SMPDD &dd = ( SMPDD & ) work.activateDevice( SMP );
   dd.getWorkFct()( work.getData() );
}

void SMPThread::join ()
{
   pthread_join( _pth,NULL );
   joined();
}

// This is executed in between switching stacks
void SMPThread::switchHelperDependent ( WD *oldWD, WD *newWD, void *oldState  )
{
   SMPDD & dd = ( SMPDD & )oldWD->getActiveDevice();
   dd.setState( (intptr_t *) oldState );
//    myThread->switchHelper( oldWD, newWD );
}

void SMPThread::inlineWorkDependent ( WD &wd )
{
   SMPDD &dd = ( SMPDD & )wd.getActiveDevice();
   ( dd.getWorkFct() )( wd.getData() );
}

void SMPThread::switchTo ( WD *wd, SchedulerHelper *helper )
{
   // wd MUST have an active Device when it gets here
   ensure( wd->hasActiveDevice(),"WD has no active SMP device" );
   SMPDD &dd = ( SMPDD & )wd->getActiveDevice();

   if ( _useUserThreads ) {
      debug( "switching from task " << getCurrentWD() << ":" << getCurrentWD()->getId() << " to " << wd << ":" << wd->getId() );

      if ( !dd.hasStack() ) {
         dd.initStack( wd->getData() );
      }

      ::switchStacks(
         ( void * ) getCurrentWD(),
         ( void * ) wd,
         ( void * ) dd.getState(),
         ( void * ) helper );
   } else {
      inlineWork( wd );
      delete wd;
   }
}

void SMPThread::exitTo ( WD *wd, SchedulerHelper *helper)
{
   // wd MUST have an active Device when it gets here
   ensure( wd->hasActiveDevice(),"WD has no active SMP device" );
   SMPDD &dd = ( SMPDD & )wd->getActiveDevice();

   debug( "exiting task " << getCurrentWD() << ":" << getCurrentWD()->getId() << " to " << wd << ":" << wd->getId() );
   // TODO: reuse stack

   if ( !dd.hasStack() ) {
      dd.initStack( wd->getData() );
   }

   //TODO: optimize... we don't really need to save a context in this case
   ::switchStacks(
      ( void * ) getCurrentWD(),
      ( void * ) wd,
      ( void * ) dd.getState(),
      ( void * ) helper );
}

void SMPThread::bind( void )
{
   cpu_set_t cpu_set;
   int cpu_id = getCpuId();

   ensure( ( ( cpu_id >= 0 ) && ( cpu_id < CPU_SETSIZE ) ), "invalid value for cpu id" );
   CPU_ZERO( &cpu_set );
   CPU_SET( cpu_id, &cpu_set );
   verbose( "Binding thread " << getId() << " to cpu " << cpu_id );
   sched_setaffinity( ( pid_t ) 0, sizeof( cpu_set ), &cpu_set );
}

void SMPThread::yield()
{
   if (sched_yield() != 0)
      fatal("sched_yield call returned an error");
}

