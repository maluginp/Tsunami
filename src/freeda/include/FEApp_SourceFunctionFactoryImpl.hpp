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
#include <sstream>
#include "Teuchos_TestForException.hpp"
#include "FEApp_ConstantSourceFunction.hpp"
#include "FEApp_QuadraticSourceFunction.hpp"
#include "FEApp_CubicSourceFunction.hpp"
#include "FEApp_ExponentialSourceFunction.hpp"
#include "FEApp_MultiVariateExponentialSourceFunction.hpp"

template <typename EvalT>
FEApp::SourceFunctionFactory<EvalT>::SourceFunctionFactory(
	    const Teuchos::RCP<Teuchos::ParameterList>& funcParams_,
	    const Teuchos::RCP<ParamLib>& paramLib_) :
  funcParams(funcParams_), paramLib(paramLib_)
{
}

template <typename EvalT>
Teuchos::RCP< FEApp::AbstractSourceFunction<EvalT> >
FEApp::SourceFunctionFactory<EvalT>::create()
{
  Teuchos::RCP< FEApp::AbstractSourceFunction<EvalT> > strategy;

  std::string& method = funcParams->get("Name", "Quadratic");
  if (method == "Constant") {
    double value = funcParams->get("Constant Value", 1.0);
    strategy = 
      Teuchos::rcp(new FEApp::ConstantSourceFunction<EvalT>(value,
							    paramLib));
  }
  else if (method == "Quadratic") {
    double factor = funcParams->get("Nonlinear Factor", 1.0);
    strategy = 
      Teuchos::rcp(new FEApp::QuadraticSourceFunction<EvalT>(factor,
                                                             paramLib));
  }
  else if (method == "Cubic") {
    double factor = funcParams->get("Nonlinear Factor", 1.0);
    strategy = 
      Teuchos::rcp(new FEApp::CubicSourceFunction<EvalT>(factor,
                                                         paramLib));
  }
  else if (method == "Exponential") {
    double factor = funcParams->get("Nonlinear Factor", 1.0);
    strategy = 
      Teuchos::rcp(new FEApp::ExponentialSourceFunction<EvalT>(factor,
                                                               paramLib));
  }
  else if (method == "Multi-Variate Exponential") {
    unsigned int numdims = funcParams->template get<unsigned int>("Nonlinear Factor Dimensions", 1);
    std::vector<ScalarT> factor(numdims);
    for (unsigned int i=0; i<numdims; i++) {
      std::stringstream ss;
      ss << "Nonlinear Factor " << i;
      factor[i] = funcParams->get(ss.str(), 1.0);
    }
    strategy = 
      Teuchos::rcp(new FEApp::MultiVariateExponentialSourceFunction<EvalT>(factor,
                                                                           paramLib));
  }
  else {
    TEST_FOR_EXCEPTION(true, Teuchos::Exceptions::InvalidParameter,
                       std::endl << 
                       "Error!  Unknown source function " << method << 
                       "!" << std::endl << "Supplied parameter list is " << 
                       std::endl << *funcParams);
  }

  return strategy;
}
