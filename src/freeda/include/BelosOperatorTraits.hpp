// @HEADER
// ***********************************************************************
//
//                 Belos: Block Linear Solvers Package
//                 Copyright (2004) Sandia Corporation
//
// Under terms of Contract DE-AC04-94AL85000, there is a non-exclusive
// license for use of this work by or on behalf of the U.S. Government.
//
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
// Questions? Contact Michael A. Heroux (maherou@sandia.gov)
//
// ***********************************************************************
// @HEADER

#ifndef BELOS_OPERATOR_TRAITS_HPP
#define BELOS_OPERATOR_TRAITS_HPP

/*!     \file BelosOperatorTraits.hpp
        \brief Virtual base class which defines the operator interface 
	required by the iterative linear solver.
*/

#include "BelosConfigDefs.hpp"

namespace Belos {

  template< class ScalarType, class MV, class OP >
  struct UndefinedOperatorTraits
  {
    //! This function should not compile if there is an attempt to instantiate!
    /*! \note Any attempt to compile this function results in a compile time error.  This means
      that the template specialization of Belos::OperatorTraits class does not exist for type
      <tt>OP</tt>, or is not complete.
    */
    static inline void notDefined() { OP::this_type_is_missing_a_specialization(); };
  };
 
  /*!  \brief Virtual base class which defines basic traits for the operator type.

       An adapter for this traits class must exist for the <tt>MV</tt> and <tt>OP</tt> types.
       If not, this class will produce a compile-time error.

       \ingroup belos_opvec_interfaces
  */ 
  template <class ScalarType, class MV, class OP>
  class OperatorTraits 
  {
  public:
    
    //! Application method which performs operation <b>y = Op*x</b>. An OperatorError exception is thrown if there is an error.
    static void Apply ( const OP& Op, 
			const MV& x, 
			MV& y, 
			ETrans trans = NOTRANS )
    { UndefinedOperatorTraits<ScalarType, MV, OP>::notDefined(); };
    
  };
  
} // end Belos namespace

#endif // BELOS_OPERATOR_TRAITS_HPP

// end of file BelosOperatorTraits.hpp
