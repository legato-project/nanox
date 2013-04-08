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
/*! \file nanos_dependences.cpp
 *  \brief 
 */
#include "nanos.h"
#include "system.hpp"
#include "instrumentationmodule_decl.hpp"

/*! \defgroup capi_wd C/C++ API: Dependences services. */
/*! \addtogroup capi_wd
 *  \{
 */

using namespace nanos;

/*! \brief Returns if there are any pendant write for a given addr
 *
 *  \param [out] res is the result
 *  \param [in] addr is the related address
 */
NANOS_API_DEF(nanos_err_t, nanos_dependence_pendant_writes, ( bool *res, void *addr ))
{
   try {
      *res = true;
   } catch ( ... ) {
      return NANOS_UNKNOWN_ERR;
   }
   return NANOS_OK;
}

/*!
 * \}
 */ 