// @HEADER
// ***********************************************************************
// 
//    Thyra: Interfaces and Support for Abstract Numerical Algorithms
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

#ifndef THYRA_LINEAR_OP_WITH_SOLVE_FACTORY_SUBCLASS_HELPERS_HPP
#define THYRA_LINEAR_OP_WITH_SOLVE_FACTORY_SUBCLASS_HELPERS_HPP


#include "Thyra_LinearOpWithSolveBase.hpp"


namespace Thyra {


/** \brief Assert that a LOWSB object supports a particular solve type.
 *
 * This function will throw an excetion with a very good error message if the
 * requested solve type is not supported.
 *
 * \ingroup thyra_operator_solve_support_LOWSF_subclass_helpers_grp
 */
template<class Scalar>
void assertSupportsSolveMeasureType(
  const LinearOpWithSolveBase<Scalar> &lows,
  const EOpTransp M_trans,
  const SolveMeasureType &solveMeasureType
  );


} // namespace Thyra


//
// Implementations
//


template<class Scalar>
void Thyra::assertSupportsSolveMeasureType(
  const LinearOpWithSolveBase<Scalar> &lows,
  const EOpTransp M_trans,
  const SolveMeasureType &solveMeasureType
  )
{
  TEST_FOR_EXCEPTION(
    !solveSupportsSolveMeasureType(lows,M_trans,solveMeasureType),
    std::logic_error,
    "Error, the LinearOpWithSolve object \"" << lows.description() << "\"\n"
    "for M_trans = " << toString(M_trans) << " does not support the solve"
    " measure = "
    << toString(solveMeasureType.numerator)
    << "/"
    << toString(solveMeasureType.denominator)
    << "!"
    );
}



#endif // THYRA_LINEAR_OP_WITH_SOLVE_FACTORY_SUBCLASS_HELPERS_HPP
