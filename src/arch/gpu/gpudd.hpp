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

#ifndef _NANOS_GPU_WD
#define _NANOS_GPU_WD

#include "config.hpp"
#include "gpudevice.hpp"
#include "workdescriptor.hpp"

namespace nanos {
namespace ext
{

   extern GPUDevice GPU;

   class GPUPlugin;

   class GPUDD : public DD
   {
      friend class GPUPlugin;
      public:
         typedef void ( *work_fct ) ( void *self );

      private:
         static int     _gpuCount; // Number of CUDA-capable GPUs
         work_fct       _work;
         static bool    _prefetch; // Enable / disable data prefetching (set by the user)
         static bool    _overlap; // Enable / disable computation and data transfer overlapping (set by the user)
         static bool    _overlapInputs;
         static bool    _overlapOutputs;
         static size_t  _maxGPUMemory; // Maximum amount of memory for each GPU to use
         static void *  _gpusProperties; // Array of structs of cudaDeviceProp

      public:
         // constructors
         GPUDD( work_fct w ) : DD( &GPU ), _work( w ) {}

         GPUDD() : DD( &GPU ), _work( 0 ) {}

         // copy constructors
         GPUDD( const GPUDD &dd ) : DD( dd ), _work( dd._work ) {}

         // assignment operator
         const GPUDD & operator= ( const GPUDD &wd );

         // destructor
         virtual ~GPUDD() { }

         work_fct getWorkFct() const { return _work; }

         static int getGPUCount () { return _gpuCount; }

         static bool isPrefetchingDefined () { return _prefetch; }

         static bool isOverlappingInputsDefined () { return _overlapInputs; }

         static bool isOverlappingOutputsDefined () { return _overlapOutputs; }

         static size_t getGPUMaxMemory() { return _maxGPUMemory; }

         static void getGPUsProperties( int device, void * deviceProps );

         virtual void lazyInit (WD &wd, bool isUserLevelThread, WD *previous) { }
         virtual size_t size ( void ) { return sizeof(GPUDD); }
         virtual GPUDD *copyTo ( void *toAddr );

         static void printConfiguration();

   };

   inline const GPUDD & GPUDD::operator= ( const GPUDD &dd )
   {
      // self-assignment: ok
      if ( &dd == this ) return *this;

      DD::operator= ( dd );
      _work = dd._work;
      _gpusProperties = dd._gpusProperties;

      return *this;
   }

}
}

#endif
