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

#ifndef BELOS_GMRESPOLYOP_HPP
#define BELOS_GMRESPOLYOP_HPP

/*!     \file BelosGmresPolyOp.hpp
        \brief Defines the GMRES polynomial operator hybrid-GMRES iterative linear solver.
*/

#include "BelosOperatorTraits.hpp"
#include "BelosMultiVecTraits.hpp"
#include "BelosLinearProblem.hpp"
#include "BelosConfigDefs.hpp"
#include "Teuchos_RCP.hpp"
#include "Teuchos_SerialDenseMatrix.hpp"
#include "Teuchos_SerialDenseVector.hpp"

/*!	\class Belos::GmresPolyOp

	\brief Belos's class for applying the GMRES polynomial operator that is used by the hybrid-GMRES linear solver.  

	This operator is used as the interface to the matrix polynomial (<tt>phi(A)</tt>), 
	solution (<tt>X</tt>), and right-hand side (<tt>B</tt>) of the linear system <tt>phi(A)X = B</tt>.
	Furthermore, it is also the interface to left/right preconditioning of the linear system.

	\author Heidi Thornquist
*/

namespace Belos {
  
  template <class ScalarType, class MV, class OP>
  class GmresPolyOp {
  public:
    
    //! @name Constructor/Destructor
    //@{ 
    
    //! Default constructor
    GmresPolyOp() {}
    
    //! Basic contstructor 
    GmresPolyOp( const Teuchos::RCP<LinearProblem<ScalarType,MV,OP> >& problem_in, 
                 const Teuchos::RCP<Teuchos::SerialDenseMatrix<int, ScalarType> >& hess,
                 const Teuchos::RCP<Teuchos::SerialDenseMatrix<int, ScalarType> >& comb,
                 const Teuchos::RCP<Teuchos::SerialDenseVector<int, ScalarType> >& scal
               ) 
      : problem_(problem_in), LP_(problem_in->getLeftPrec()), RP_(problem_in->getRightPrec()), H_(hess), y_(comb), r0_(scal)
    {
      dim_ = y_->numRows();
    }

    //! Destructor.
    virtual ~GmresPolyOp() {};
    //@}
    
    //! @name Operator application method
    //@{ 
    
    /*! \brief This routine takes the MV \c x and applies the polynomial 
        operator \c phi(OP) to it resulting in the MV \c y, which is returned.
        \note It is expected that any problem with applying this operator to \c x will be
	indicated by an std::exception being thrown.
    */
    void Apply ( const MV& x, MV& y, ETrans trans=NOTRANS );

    private:

    typedef MultiVecTraits<ScalarType,MV> MVT;
    typedef Teuchos::ScalarTraits<ScalarType> SCT ;

    int dim_;
    Teuchos::RCP<LinearProblem<ScalarType,MV,OP> > problem_;
    Teuchos::RCP<const OP> LP_, RP_;
    Teuchos::RCP<MV> V_, wL_, wR_;
    Teuchos::RCP<Teuchos::SerialDenseMatrix<int,ScalarType> > H_, y_;
    Teuchos::RCP<Teuchos::SerialDenseVector<int,ScalarType> > r0_;
  };
  
  template <class ScalarType, class MV, class OP>
  void GmresPolyOp<ScalarType, MV, OP>::Apply( const MV& x, MV& y, ETrans trans ) 
  {
    // Initialize vector storage.
    if (V_ == Teuchos::null) {
      V_ = MVT::Clone( x, dim_ );
      if (LP_ != Teuchos::null)
        wL_ = MVT::Clone( y, 1 );
      if (RP_ != Teuchos::null)
        wR_ = MVT::Clone( y, 1 );
    }
    //
    // Apply polynomial to x.
    // 
    int n = MVT::GetNumberVecs( x );
    std::vector<int> idxi(1), idxi2, idxj(1);
    Teuchos::RCP<MV> v_curr, v_next, v_prev;

    // Select vector x[j].
    for (int j=0; j<n; ++j) {

      idxi[0] = 0;
      idxj[0] = j;
      Teuchos::RCP<const MV> x_view = MVT::CloneView( x, idxj );
      Teuchos::RCP<MV> y_view = MVT::CloneView( y, idxj );
      if (LP_ != Teuchos::null) {
	v_curr = MVT::CloneView( *V_, idxi );
	problem_->applyLeftPrec( *x_view, *v_curr ); // Left precondition x into the first vector of V
      } else {
	MVT::SetBlock( *x_view, idxi, *V_ );  // Set x as the first vector of V
      }
      
      for (int i=0; i<dim_-1; ++i) {
	
	// Get views into the current and next vectors
	idxi2.resize(i+1);
	for (int ii=0; ii<i+1; ++ii) { idxi2[ii] = ii; }
	v_prev = MVT::CloneView( *V_, idxi2 );
	v_curr = MVT::CloneView( *V_, idxi );
	idxi[0] = i+1;
	v_next = MVT::CloneView( *V_, idxi );
	
	//---------------------------------------------
	// Apply operator to next vector
	//---------------------------------------------
	// 1) Apply right preconditioner, if we have one.
	if (RP_ != Teuchos::null) {
	  problem_->applyRightPrec( *v_curr, *wR_ );
	} else {
	  wR_ = v_curr;
	}
	// 2) Check for right preconditioner, if none exists, point at the next vector.
	if (LP_ == Teuchos::null) {
	  wL_ = v_next;
	}
	// 3) Apply operator A.
	problem_->applyOp( *wR_, *wL_ );
	// 4) Apply left preconditioner, if we have one.
	if (LP_ != Teuchos::null) {
	  problem_->applyLeftPrec( *wL_, *v_next );
	}
	
	// Compute A*v_curr - v_prev*H(1:i,i)
	Teuchos::SerialDenseMatrix<int,ScalarType> h(Teuchos::View,*H_,i+1,1,0,i);
	MVT::MvTimesMatAddMv( -SCT::one(), *v_prev, h, SCT::one(), *v_next );
	
	// Scale by H(i+1,i)
	MVT::MvScale( *v_next, SCT::one()/(*H_)(i+1,i) );  
      }
      
      // Compute output y = V*y_./r0_
      if (RP_ != Teuchos::null) {
	MVT::MvTimesMatAddMv( SCT::one()/(*r0_)(0), *V_, *y_, SCT::zero(), *wR_ );
	problem_->applyRightPrec( *wR_, *y_view );
      } else {
	MVT::MvTimesMatAddMv( SCT::one()/(*r0_)(0), *V_, *y_, SCT::zero(), *y_view );
      }
    } // (int j=0; j<n; ++j)
  } // end Apply()
    
  ////////////////////////////////////////////////////////////////////
  //
  // Implementation of the Belos::OperatorTraits for Belos::GmresPolyOp 
  //
  ////////////////////////////////////////////////////////////////////  
  
  /*!  \brief Template specialization of Belos::OperatorTraits class using Belos::GmresPolyOp
  */

  template <class ScalarType, class MV, class OP> 
  class OperatorTraits < ScalarType, MV, GmresPolyOp<ScalarType,MV,OP> > 
  {
  public:
    
    ///
    static void Apply ( const GmresPolyOp<ScalarType,MV,OP>& Op, 
			const MV& x, MV& y,
			ETrans trans=NOTRANS )
    { Op.Apply( x, y, trans ); }
    
  };
  
} // end Belos namespace

#endif

// end of file BelosGmresPolyOp.hpp
