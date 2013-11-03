
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

#ifndef BELOS_SOLVERMANAGER_HPP
#define BELOS_SOLVERMANAGER_HPP

/*! \file BelosSolverManager.hpp
    \brief Pure virtual base class which describes the basic interface for a solver manager.
*/

#include "BelosConfigDefs.hpp"
#include "BelosTypes.hpp"
#include "BelosLinearProblem.hpp"

#include "Teuchos_ParameterList.hpp"
#include "Teuchos_RCP.hpp"
#include "Teuchos_Describable.hpp"

/*! \class Belos::SolverManager
  \brief The Belos::SolverManager is a templated virtual base class that defines the
	basic interface that any solver manager will support.
*/

namespace Belos {

template<class ScalarType, class MV, class OP>
class SolverManager : virtual public Teuchos::Describable {
    
  public:

  //!@name Constructors/Destructor 
  //@{ 

  //! Empty constructor.
  SolverManager() {};

  //! Destructor.
  virtual ~SolverManager() {};
  //@}
  
  //! @name Accessor methods
  //@{ 

  //! Return a reference to the linear problem being solved by this solver manager.
  virtual const LinearProblem<ScalarType,MV,OP>& getProblem() const = 0;

  //! Return the valid parameters for this solver manager.
  virtual Teuchos::RCP<const Teuchos::ParameterList> getValidParameters() const = 0;

  //! Return the current parameters being used for this solver manager.
  virtual Teuchos::RCP<const Teuchos::ParameterList> getCurrentParameters() const = 0;

  //! Get the iteration count for the most recent call to \c solve().
  virtual int getNumIters() const = 0;

  /*! \brief Returns whether a loss of accuracy was detected in the solver. 
   *  \note This method is normally applicable to GMRES-type solvers.
  */
  virtual bool isLOADetected() const = 0;
 
  //@}

  //! @name Set methods
  //@{

  //! Set the linear problem that needs to be solved. 
  virtual void setProblem( const Teuchos::RCP<LinearProblem<ScalarType,MV,OP> > &problem ) = 0;

  //! Set the parameters the solver manager should use to solve the linear problem.
  virtual void setParameters( const Teuchos::RCP<Teuchos::ParameterList> &params ) = 0;

  //@}

  //! @name Reset methods
  //@{
  /*! \brief Performs a reset of the solver manager specified by the \c ResetType.  This informs the
   *  solver manager that the solver should prepare for the next call to solve by resetting certain elements
   *  of the iterative solver strategy.
  */ 
  virtual void reset( const ResetType type ) = 0;
  //@}

  //! @name Solver application methods
  //@{ 
    
  /*! \brief This method performs possibly repeated calls to the underlying linear solver's iterate() routine
   * until the problem has been solved (as decided by the solver manager) or the solver manager decides to 
   * quit.
   *
   * \returns ::ReturnType specifying:
   *    - ::Converged: the linear problem was solved to the specification required by the solver manager.
   *    - ::Unconverged: the linear problem was not solved to the specification desired by the solver manager
  */
  virtual ReturnType solve() = 0;
  //@}
  
};

} // end Belos namespace

#endif /* BELOS_SOLVERMANAGER_HPP */
