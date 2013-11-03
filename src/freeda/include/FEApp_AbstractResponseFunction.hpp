// $Id$ 
// $Source$ 
// @HEADER
// ***********************************************************************
// 
//                           Sacado Package
//                 Copyright (2006) Sandia Corporation
// 
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
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
// Questions? Contact David M. Gay (dmgay@sandia.gov) or Eric T. Phipps
// (etphipp@sandia.gov).
// 
// ***********************************************************************
// @HEADER

#ifndef FEAPP_ABSTRACTRESPONSEFUNCTION_HPP
#define FEAPP_ABSTRACTRESPONSEFUNCTION_HPP

#include "Teuchos_Array.hpp"
#include "Teuchos_RCP.hpp"
#include "FEApp_TemplateTypes.hpp"
#include "Epetra_Vector.h"
#if SG_ACTIVE
#include "Stokhos_OrthogPolyBasis.hpp"
#include "Stokhos_Quadrature.hpp"
#include "Stokhos_OrthogPolyExpansion.hpp"
#include "Stokhos_EpetraVectorOrthogPoly.hpp"
#include "Stokhos_EpetraMultiVectorOrthogPoly.hpp"
#endif

namespace FEApp {

  /*!
   * \brief Abstract interface for representing a response function
   */
  class AbstractResponseFunction {
  public:
  
    //! Default constructor
    AbstractResponseFunction() {};

    //! Destructor
    virtual ~AbstractResponseFunction() {};

    //! Get the number of responses
    virtual unsigned int numResponses() const = 0;

    //! Evaluate responses
    virtual void 
    evaluateResponses(const Epetra_Vector* xdot,
                      const Epetra_Vector& x,
                      const Teuchos::Array< Teuchos::RCP<ParamVec> >& p,
                      Epetra_Vector& g) = 0;

    //! Evaluate tangent = dg/dx*dx/dp + dg/dxdot*dxdot/dp + dg/dp
    virtual void 
    evaluateTangents(
	   const Epetra_Vector* xdot,
	   const Epetra_Vector& x,
	   const Teuchos::Array< Teuchos::RCP<ParamVec> >& p,
	   const Teuchos::Array< Teuchos::RCP<ParamVec> >& deriv_p,
	   const Teuchos::Array< Teuchos::RCP<Epetra_MultiVector> >& dxdot_dp,
	   const Teuchos::Array< Teuchos::RCP<Epetra_MultiVector> >& dx_dp,
	   Epetra_Vector* g,
	   const Teuchos::Array< Teuchos::RCP<Epetra_MultiVector> >& gt) = 0;

    //! Evaluate gradient = dg/dx, dg/dxdot, dg/dp
    virtual void 
    evaluateGradients(
	  const Epetra_Vector* xdot,
	  const Epetra_Vector& x,
	  const Teuchos::Array< Teuchos::RCP<ParamVec> >& p,
	  const Teuchos::Array< Teuchos::RCP<ParamVec> >& deriv_p,
	  Epetra_Vector* g,
	  Epetra_MultiVector* dg_dx,
	  Epetra_MultiVector* dg_dxdot,
	  const Teuchos::Array< Teuchos::RCP<Epetra_MultiVector> >& dg_dp) = 0;

#if SG_ACTIVE
    //! Initialize SG expansion data
    virtual void init_sg(
      const Teuchos::RCP<const Stokhos::OrthogPolyBasis<int,double> >& sg_basis,
      const Teuchos::RCP<const Stokhos::Quadrature<int,double> >& sg_quad,
      const Teuchos::RCP<Stokhos::OrthogPolyExpansion<int,double> >& sg_exp) = 0;

    //! Evaluate stochastic Galerkin responses
    virtual void 
    evaluateSGResponses(const Stokhos::EpetraVectorOrthogPoly* sg_xdot,
			const Stokhos::EpetraVectorOrthogPoly& sg_x,
			const Teuchos::Array< Teuchos::RCP<ParamVec> >& p,
			const Teuchos::Array<SGType>* sg_p_vals,
			Stokhos::EpetraVectorOrthogPoly& sg_g) = 0;

    //! Evaluate SG tangent = dg/dx*dx/dp + dg/dxdot*dxdot/dp + dg/dp
    virtual void 
    evaluateSGTangents(
      const Stokhos::EpetraVectorOrthogPoly* sg_xdot,
      const Stokhos::EpetraVectorOrthogPoly& sg_x,
      const Teuchos::Array< Teuchos::RCP<ParamVec> >& p,
      const Teuchos::Array< Teuchos::RCP<ParamVec> >& deriv_p,
      const Teuchos::Array<SGType>* sg_p_vals,
      const Teuchos::Array< Teuchos::RCP<Epetra_MultiVector> >& dxdot_dp,
      const Teuchos::Array< Teuchos::RCP<Epetra_MultiVector> >& dx_dp,
      Stokhos::EpetraVectorOrthogPoly* sg_g,
      const Teuchos::Array< Teuchos::RCP<Stokhos::EpetraMultiVectorOrthogPoly > >& sg_gt) = 0;

    //! Evaluate SG gradient = dg/dx, dg/dxdot, dg/dp
    virtual void 
    evaluateSGGradients(
      const Stokhos::EpetraVectorOrthogPoly* sg_xdot,
      const Stokhos::EpetraVectorOrthogPoly& sg_x,
      const Teuchos::Array< Teuchos::RCP<ParamVec> >& p,
      const Teuchos::Array< Teuchos::RCP<ParamVec> >& deriv_p,
      const Teuchos::Array<SGType>* sg_p_vals,
      Stokhos::EpetraVectorOrthogPoly* sg_g,
      Stokhos::EpetraMultiVectorOrthogPoly* sg_dg_dx,
      Stokhos::EpetraMultiVectorOrthogPoly* sg_dg_dxdot,
      const Teuchos::Array< Teuchos::RCP<Stokhos::EpetraMultiVectorOrthogPoly > >& sg_dg_dp) = 0;
#endif

  private:

    //! Private to prohibit copying
    AbstractResponseFunction(const AbstractResponseFunction&);
    
    //! Private to prohibit copying
    AbstractResponseFunction& operator=(const AbstractResponseFunction&);

  };

}

#endif // FEAPP_ABSTRACTRESPONSEFUNCTION_HPP
