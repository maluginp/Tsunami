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

#ifndef FEAPP_DISCRETIZATIONFACTORY_HPP
#define FEAPP_DISCRETIZATIONFACTORY_HPP

#include <vector>

#include "Teuchos_ParameterList.hpp"
#include "Teuchos_RCP.hpp"
#include "Epetra_Comm.h"

#include "FEApp_AbstractDiscretization.hpp"

namespace FEApp {

  /*!
   * \brief A factory class to instantiate AbstractDiscretization objects
   */
  class DiscretizationFactory {
  public:

    //! Default constructor
    DiscretizationFactory(
	      const Teuchos::RCP<Teuchos::ParameterList>& discParams);

    //! Destructor
    virtual ~DiscretizationFactory() {}

    virtual Teuchos::RCP<FEApp::AbstractDiscretization>
    create(const std::vector<double>& coords,
           unsigned int num_equations,
           const Teuchos::RCP<const Epetra_Comm>& epetra_comm);

  private:

    //! Private to prohibit copying
    DiscretizationFactory(const DiscretizationFactory&);

    //! Private to prohibit copying
    DiscretizationFactory& operator=(const DiscretizationFactory&);

  protected:

    //! Parameter list specifying what element to create
    Teuchos::RCP<Teuchos::ParameterList> discParams;

  };

}

#endif // FEAPP_DISCRETIZATIONFACTORY_HPP
