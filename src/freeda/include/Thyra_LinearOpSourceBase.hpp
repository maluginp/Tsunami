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

#ifndef THYRA_LINEAR_OP_SOURCE_BASE_HPP
#define THYRA_LINEAR_OP_SOURCE_BASE_HPP

#include "Thyra_SolveSupportTypes.hpp"
#include "Teuchos_Describable.hpp"


namespace Thyra {


/** \brief Base interface for objects that can return a linear operator.
 *
 * \ingroup Thyra_Op_Solve_fundamental_interfaces_code_grp
 */
template<class Scalar>
class LinearOpSourceBase : virtual public Teuchos::Describable
{
public:

  /** @name Pure virtual public functions that must be overridden in subclasses */
  //@{

  /** \brief Return if the underlying linear operator is const-only or not.
   */
  virtual bool isOpConst() const = 0;

  /** \brief Return a non-const reference to the underlying linear operator.
   *
   * <b>Preconditions:</b><ul>
   * <li>[<tt>isOpConst()==true</tt>] <tt>getOp().get()==NULL</tt>
   * </ul>
   */
  virtual Teuchos::RCP<LinearOpBase<Scalar> > getNonconstOp() = 0;

  /** \brief Return a const left preconditioner linear operator if one is
   * designed or targeted to be applied on the left.
   */
  virtual Teuchos::RCP<const LinearOpBase<Scalar> > getOp()const = 0;
  
  //@}

};

} // namespace Thyra

#endif // THYRA_LINEAR_OP_SOURCE_BASE_HPP
