// @HEADER
// ***********************************************************************
//
//		   Belos: Block Linear Solvers Package
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
//

#ifndef BELOS_STATUS_TEST_OUTPUT_HPP
#define BELOS_STATUS_TEST_OUTPUT_HPP

/*!
  \file BelosStatusTestOutput.hpp
  \brief Virtual base class for StatusTest that printing status tests.
*/

#include "BelosConfigDefs.hpp"
#include "BelosTypes.hpp"
#include "BelosIteration.hpp"

#include "BelosStatusTest.hpp"
#include "BelosOutputManager.hpp"


namespace Belos {

  /*! 
    \class StatusTestOutput
    \brief A virtual base class for StatusTest that print other status tests.     

    StatusTestOutput provides an interface for status tests that wrap another StatusTest.  These
    printing status tests can be generic, calling StatusTest::print() on the underlying object, or 
    can specifically require input status tests to be of a certain type. The frequency and occasion 
    of the printing can be dictated according to some parameters passed to 
    StatusTestOutput::StatusTestOutput().
  */
template <class ScalarType, class MV, class OP>
class StatusTestOutput : public virtual StatusTest<ScalarType,MV,OP> {

 public:
  //! @name Constructors/destructors
  //@{ 

  //! \brief Default constructor
  StatusTestOutput() {}

  /*! \brief Constructor
   *
   * The StatusTestOutput requires an OutputManager for printing the underlying StatusTest on
   * calls to checkStatus(), as well as an underlying StatusTest.
   *
   * The last two parameters, described below, in addition to the verbosity level of the OutputManager, control when printing is 
   * called. When both the \c mod criterion and the \c printStates criterion are satisfied, the status test will be printed to the 
   * OutputManager with ::MsgType of ::StatusTestDetails.
   *
   * @param[in] mod A positive number describes how often the output should be printed. On every call to checkStatus(), an internal counter
   *                is incremented. Printing may only occur when this counter is congruent to zero modulo \c mod. Default: 1 (attempt to print on every call to checkStatus())
   * @param[in] printStates A combination of ::StatusType values for which the output may be printed. Default: ::Passed (attempt to print whenever checkStatus() will return ::Passed)
   *
   */
  StatusTestOutput(const Teuchos::RCP<OutputManager<ScalarType> > &printer, 
                   Teuchos::RCP<StatusTest<ScalarType,MV,OP> > test,
                   int mod = 1,
                   int printStates = Passed)
    {}   

  //! Destructor
  virtual ~StatusTestOutput() {}
  //@}

  //! @name Accessor methods
  //@{ 

  /*! \brief Set the output manager.
   */ 
  virtual void setOutputManager(const Teuchos::RCP<OutputManager<ScalarType> > &printer) = 0;

  /*! \brief Set how often the child test is printed.
   */
  virtual void setOutputFrequency(int mod) = 0;

  /*! \brief Set child test.
   *
   *  \note This also resets the test status to ::Undefined.
   */
  virtual void setChild(Teuchos::RCP<StatusTest<ScalarType,MV,OP> > test) = 0;

  //! \brief Get child test.
  virtual Teuchos::RCP<StatusTest<ScalarType,MV,OP> > getChild() const = 0;

  /*! \brief Set a short solver description for output clarity.
   */
  virtual void setSolverDesc(const std::string& solverDesc) = 0;

  /*! \brief Set a short preconditioner description for output clarity.
   */
  virtual void setPrecondDesc(const std::string& precondDesc) = 0;

  //@}


  //! @name Reset methods
  //@{ 

  //! Informs the outputting status test that it should reset the number of calls to zero.
  virtual void resetNumCalls() = 0;

  //@}

};

} // end of Belos namespace

#endif /* BELOS_STATUS_TEST_OUTPUT_HPP */
