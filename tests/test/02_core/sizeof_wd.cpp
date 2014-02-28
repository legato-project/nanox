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

/*
<testinfo>
test_generator=gens/core-generator
test_schedule="bf"
test_max_cpus=1
</testinfo>
*/

#include "system.hpp"
#include <iostream>

using namespace std;
using namespace nanos;

#define KMAX 128*sizeof(void *)

int main ( int argc, char **argv )
{
   int error = 0;
   cout << "Size of WorkDescriptor is " << sizeof(WD) << " out of " << KMAX << endl;
   if ( sizeof(WD) > KMAX ) error = 1;
   return error;
}