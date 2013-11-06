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

#ifndef BELOS_GCRODR_SOLMGR_HPP
#define BELOS_GCRODR_SOLMGR_HPP

/*! \file BelosGCRODRSolMgr.hpp
 *  \brief The Belos::GCRODRSolMgr provides a solver manager for the GCRODR linear solver.
*/

#include "BelosConfigDefs.hpp"
#include "BelosTypes.hpp"

#include "BelosLinearProblem.hpp"
#include "BelosSolverManager.hpp"

#include "BelosGCRODRIter.hpp"
#include "BelosBlockFGmresIter.hpp"
#include "BelosDGKSOrthoManager.hpp"
#include "BelosICGSOrthoManager.hpp"
#include "BelosIMGSOrthoManager.hpp"
#include "BelosStatusTestMaxIters.hpp"
#include "BelosStatusTestGenResNorm.hpp"
#include "BelosStatusTestCombo.hpp"
#include "BelosStatusTestOutputFactory.hpp"
#include "BelosOutputManager.hpp"
#include "Teuchos_BLAS.hpp"
#include "Teuchos_LAPACK.hpp"
#include "Teuchos_TimeMonitor.hpp"

/** \example epetra/example/GCRODR/GCRODREpetraExFile.cpp
    This is an example of how to use the Belos::GCRODRSolMgr solver manager.
*/
/** \example epetra/example/GCRODR/PrecGCRODREpetraExFile.cpp
    This is an example of how to use the Belos::GCRODRSolMgr solver manager with an Ifpack preconditioner.
*/

/*! \class Belos::GCRODRSolMgr
 *
 *  \brief The Belos::GCRODRSolMgr provides a powerful and fully-featured solver manager over the GCRODR linear solver.

 \ingroup belos_solver_framework

 \author Michael Parks and Heidi Thornquist
 */

namespace Belos {
  
  //! @name GCRODRSolMgr Exceptions
  //@{
  
  /** \brief GCRODRSolMgrLinearProblemFailure is thrown when the linear problem is
   * not setup (i.e. setProblem() was not called) when solve() is called.
   *
   * This exception is thrown from the GCRODRSolMgr::solve() method.
   *
   */
  class GCRODRSolMgrLinearProblemFailure : public BelosError {
    public:
      GCRODRSolMgrLinearProblemFailure(const std::string& what_arg) : BelosError(what_arg) {}
  };
  
  /** \brief GCRODRSolMgrOrthoFailure is thrown when the orthogonalization manager is
   * unable to generate orthonormal columns from the initial basis vectors.
   *
   * This exception is thrown from the GCRODRSolMgr::solve() method.
   *
   */
  class GCRODRSolMgrOrthoFailure : public BelosError {
    public:
      GCRODRSolMgrOrthoFailure(const std::string& what_arg) : BelosError(what_arg) {}
  };
  
  /** \brief GCRODRSolMgrLAPACKFailure is thrown when a nonzero value is retuned
   * from an LAPACK call.
   *
   * This exception is thrown from the GCRODRSolMgr::solve() method.
   *
   */
  class GCRODRSolMgrLAPACKFailure : public BelosError {
    public:
      GCRODRSolMgrLAPACKFailure(const std::string& what_arg) : BelosError(what_arg) {}
  };

  /** \brief GCRODRSolMgrRecyclingFailure is thrown when any problem occurs in using/creating
   * the recycling subspace.
   *
   * This exception is thrown from the GCRODRSolMgr::solve() method.
   *
   */
  class GCRODRSolMgrRecyclingFailure : public BelosError {
    public:
      GCRODRSolMgrRecyclingFailure(const std::string& what_arg) : BelosError(what_arg) {}
  };

  //@}

  template<class ScalarType, class MV, class OP>
  class GCRODRSolMgr : public SolverManager<ScalarType,MV,OP> {
    
  private:
    typedef MultiVecTraits<ScalarType,MV> MVT;
    typedef OperatorTraits<ScalarType,MV,OP> OPT;
    typedef Teuchos::ScalarTraits<ScalarType> SCT;
    typedef typename Teuchos::ScalarTraits<ScalarType>::magnitudeType MagnitudeType;
    typedef Teuchos::ScalarTraits<MagnitudeType> MT;
    
  public:
    
    //! @name Constructors/Destructor
    //@{ 
   
    /*! \brief Empty constructor for GCRODRSolMgr.
     * This constructor takes no arguments and sets the default values for the solver.
     * The linear problem must be passed in using setProblem() before solve() is called on this object.
     * The solver values can be changed using setParameters().
     */
     GCRODRSolMgr();
 
    /*! \brief Basic constructor for GCRODRSolMgr.
     *
     * This constructor accepts the LinearProblem to be solved in addition
     * to a parameter list of options for the solver manager. These options include the following:
     *   - "Num Blocks" - a \c int specifying the number of blocks allocated for the Krylov basis. 25
     *   - "Num Recycled Blocks" - a \c int specifying the number of blocks allocated for the Krylov basis. Default: 5
     *   - "Maximum Iterations" - a \c int specifying the maximum number of iterations the underlying solver is allowed to perform. Default: 300
     *   - "Maximum Restarts" - a \c int specifying the maximum number of restarts the underlying solver is allowed to perform. Default: 20
     *   - "Orthogonalization" - a \c string specifying the desired orthogonalization:  DGKS, ICGS, IMGS. Default: "DGKS"
     *   - "Verbosity" - a sum of MsgType specifying the verbosity. Default: Belos::Errors
     *   - "Output Style" - a OutputType specifying the style of output. Default: Belos::General
     *   - "Convergence Tolerance" - a \c MagnitudeType specifying the level that residual norms must reach to decide convergence. Default: 1e-8.
     */
    GCRODRSolMgr( const Teuchos::RCP<LinearProblem<ScalarType,MV,OP> > &problem, const Teuchos::RCP<Teuchos::ParameterList> &pl );
    
    //! Destructor.
    virtual ~GCRODRSolMgr() {};
    //@}
    
    //! @name Accessor methods
    //@{ 
    
    /*! \brief Get current linear problem being solved for in this object.
     */
    const LinearProblem<ScalarType,MV,OP>& getProblem() const {
      return *problem_;
    }

    /*! \brief Get a parameter list containing the valid parameters for this object.
     */
    Teuchos::RCP<const Teuchos::ParameterList> getValidParameters() const;

    /*! \brief Get a parameter list containing the current parameters for this object.
     */
    Teuchos::RCP<const Teuchos::ParameterList> getCurrentParameters() const { 
      return params_; 
    }
 
    /*! \brief Return the timers for this object. 
     *
     * The timers are ordered as follows:
     *   - time spent in solve() routine
     */
    Teuchos::Array<Teuchos::RCP<Teuchos::Time> > getTimers() const {
      return tuple(timerSolve_);
    }
  
    //! Get the iteration count for the most recent call to \c solve().
    int getNumIters() const {
      return numIters_;
    }
 
    /*! \brief Return whether a loss of accuracy was detected by this solver during the most current solve.
     */
    bool isLOADetected() const { return false; }
 
    //@}
    
    //! @name Set methods
    //@{
   
    //! Set the linear problem that needs to be solved. 
    void setProblem( const Teuchos::RCP<LinearProblem<ScalarType,MV,OP> > &problem ) { 
      problem_ = problem; 
    }
   
    //! Set the parameters the solver manager should use to solve the linear problem. 
    void setParameters( const Teuchos::RCP<Teuchos::ParameterList> &params );
    
    //@}
   
    //! @name Reset methods
    //@{
    /*! \brief Performs a reset of the solver manager specified by the \c ResetType.  This informs the
     *  solver manager that the solver should prepare for the next call to solve by resetting certain elements
     *  of the iterative solver strategy.
     */
    void reset( const ResetType type ) { 
      if ((type & Belos::Problem) && !Teuchos::is_null(problem_))  
        problem_->setProblem();
    }
    //@}
 
    //! @name Solver application methods
    //@{ 
    
    /*! \brief This method performs possibly repeated calls to the underlying linear solver's iterate() routine
     * until the problem has been solved (as decided by the solver manager) or the solver manager decides to quit
     *
     * This method calls GCRODRIter::iterate(), which will return either because a specially constructed status test evaluates to 
     * ::Passed or an exception is thrown.
     *
     * A return from GCRODRIter::iterate() signifies one of the following scenarios:
     *    - the maximum number of restarts has been exceeded. In this scenario, the current solutions to the linear system
     *      will be placed in the linear problem and return ::Unconverged.
     *    - global convergence has been met. In this case, the current solutions to the linear system will be placed in the linear
     *      problem and the solver manager will return ::Converged
     *
     * \returns ::ReturnType specifying:
     *     - ::Converged: the linear problem was solved to the specification required by the solver manager.
     *     - ::Unconverged: the linear problem was not solved to the specification desired by the solver manager.
     */
    ReturnType solve();
    
    //@}
    
    /** \name Overridden from Teuchos::Describable */
    //@{
    
    /** \brief Method to return description of the block GMRES solver manager */
    std::string description() const;
    
    //@}
    
  private:

    // Called by all constructors; Contains init instructions common to all constructors
    void init();

    // Initialize solver state storage
    void initializeStateStorage();

    //  Computes harmonic eigenpairs of projected matrix created during the priming solve.
    //  HH is the projected problem from the initial cycle of Gmres, it is (at least) of dimension m+1 x m.
    //  PP contains the harmonic eigenvectors corresponding to the recycledBlocks eigenvalues of smallest magnitude.
    //  The return value is the number of vectors needed to be stored, recycledBlocks or recycledBlocks+1.
    int getHarmonicVecs1(int m, 
			 const Teuchos::SerialDenseMatrix<int,ScalarType>& HH, 
			 Teuchos::SerialDenseMatrix<int,ScalarType>& PP);

    //  Computes harmonic eigenpairs of projected matrix created during one cycle.
    //  HH is the total block projected problem from the GCRO-DR algorithm, it is (at least) of dimension keff+m+1 x keff+m.
    //  VV is the Krylov vectors from the projected GMRES algorithm, which has (at least) m+1 vectors.
    //  PP contains the harmonic eigenvectors corresponding to the recycledBlocks eigenvalues of smallest magnitude.
    //  The return value is the number of vectors needed to be stored, recycledBlocks or recycledBlocks+1.
    int getHarmonicVecs2(int keff, int m, 
			 const Teuchos::SerialDenseMatrix<int,ScalarType>& HH, 
			 const Teuchos::RCP<const MV>& VV,
			 Teuchos::SerialDenseMatrix<int,ScalarType>& PP);

    // Sort list of n floating-point numbers and return permutation vector
    void sort(std::vector<ScalarType>& dlist, int n, std::vector<int>& iperm);

    // Method to convert string to enumerated type for residual.
    Belos::ScaleType convertStringToScaleType( std::string& scaleType ) {
      if (scaleType == "Norm of Initial Residual") {
	return Belos::NormOfInitRes;
      } else if (scaleType == "Norm of Preconditioned Initial Residual") {
	return Belos::NormOfPrecInitRes;
      } else if (scaleType == "Norm of RHS") {
	return Belos::NormOfRHS;
      } else if (scaleType == "None") {
	return Belos::None;
      } else 
	TEST_FOR_EXCEPTION( true ,std::logic_error,"Belos::GCRODRSolMgr(): Invalid residual scaling type.");
    }

    // Lapack interface
    Teuchos::LAPACK<int,ScalarType> lapack;

    // Linear problem.
    Teuchos::RCP<LinearProblem<ScalarType,MV,OP> > problem_;
    
    // Output manager.
    Teuchos::RCP<OutputManager<ScalarType> > printer_;
    Teuchos::RCP<std::ostream> outputStream_;

    // Status test.
    Teuchos::RCP<StatusTest<ScalarType,MV,OP> > sTest_;
    Teuchos::RCP<StatusTestMaxIters<ScalarType,MV,OP> > maxIterTest_;
    Teuchos::RCP<StatusTest<ScalarType,MV,OP> > convTest_;
    Teuchos::RCP<StatusTestGenResNorm<ScalarType,MV,OP> > expConvTest_, impConvTest_;
    Teuchos::RCP<StatusTestOutput<ScalarType,MV,OP> > outputTest_;

    // Orthogonalization manager.
    Teuchos::RCP<MatOrthoManager<ScalarType,MV,OP> > ortho_; 
    
    // Current parameter list.
    Teuchos::RCP<ParameterList> params_;

    // Default solver values.
    static const MagnitudeType convtol_default_;
    static const MagnitudeType orthoKappa_default_;
    static const int maxRestarts_default_;
    static const int maxIters_default_;
    static const int numBlocks_default_;
    static const int blockSize_default_;
    static const int recycledBlocks_default_;
    static const int verbosity_default_;
    static const int outputStyle_default_;
    static const int outputFreq_default_;
    static const std::string impResScale_default_; 
    static const std::string expResScale_default_; 
    static const std::string label_default_;
    static const std::string orthoType_default_;
    static const Teuchos::RCP<std::ostream> outputStream_default_;

    // Current solver values.
    MagnitudeType convtol_, orthoKappa_;
    int maxRestarts_, maxIters_, numIters_;
    int verbosity_, outputStyle_, outputFreq_;
    std::string orthoType_; 
    std::string impResScale_, expResScale_;

    /////////////////////////////////////////////////////////////////////////
    // Solver State Storage
    /////////////////////////////////////////////////////////////////////////
    // 
    // The number of blocks and recycle blocks (m and k, respectively)
    int numBlocks_, recycledBlocks_;
    // Current size of recycled subspace 
    int keff;
    //
    // Residual vector
    Teuchos::RCP<MV> r_;
    //
    // Search space
    Teuchos::RCP<MV> V_;
    //
    // Recycled subspace and its image
    Teuchos::RCP<MV> U_, C_;
    //
    // Updated recycle space and its image
    Teuchos::RCP<MV> U1_, C1_;
    //
    // Storage used in constructing harmonic Ritz values/vectors
    Teuchos::RCP<Teuchos::SerialDenseMatrix<int,ScalarType> > H2_;
    Teuchos::RCP<Teuchos::SerialDenseMatrix<int,ScalarType> > H_;
    Teuchos::RCP<Teuchos::SerialDenseMatrix<int,ScalarType> > B_;
    Teuchos::RCP<Teuchos::SerialDenseMatrix<int,ScalarType> > PP_;
    Teuchos::RCP<Teuchos::SerialDenseMatrix<int,ScalarType> > HP_;
    std::vector<ScalarType> tau_;
    std::vector<ScalarType> work_;
    Teuchos::RCP<Teuchos::SerialDenseMatrix<int,ScalarType> > R_;
    std::vector<int> ipiv_;
    /////////////////////////////////////////////////////////////////////////

    // Timers.
    std::string label_;
    Teuchos::RCP<Teuchos::Time> timerSolve_;

    // Internal state variables.
    bool isSet_;
  };


// Default solver values.
template<class ScalarType, class MV, class OP>
const typename GCRODRSolMgr<ScalarType,MV,OP>::MagnitudeType GCRODRSolMgr<ScalarType,MV,OP>::convtol_default_ = 1e-8;

template<class ScalarType, class MV, class OP>
const typename GCRODRSolMgr<ScalarType,MV,OP>::MagnitudeType GCRODRSolMgr<ScalarType,MV,OP>::orthoKappa_default_ = -1.0;

template<class ScalarType, class MV, class OP>
const int GCRODRSolMgr<ScalarType,MV,OP>::maxRestarts_default_ = 100;

template<class ScalarType, class MV, class OP>
const int GCRODRSolMgr<ScalarType,MV,OP>::maxIters_default_ = 5000;

template<class ScalarType, class MV, class OP>
const int GCRODRSolMgr<ScalarType,MV,OP>::numBlocks_default_ = 50;

template<class ScalarType, class MV, class OP>
const int GCRODRSolMgr<ScalarType,MV,OP>::blockSize_default_ = 1;

template<class ScalarType, class MV, class OP>
const int GCRODRSolMgr<ScalarType,MV,OP>::recycledBlocks_default_ = 5;

template<class ScalarType, class MV, class OP>
const int GCRODRSolMgr<ScalarType,MV,OP>::verbosity_default_ = Belos::Errors;

template<class ScalarType, class MV, class OP>
const int GCRODRSolMgr<ScalarType,MV,OP>::outputStyle_default_ = Belos::General;

template<class ScalarType, class MV, class OP>
const int GCRODRSolMgr<ScalarType,MV,OP>::outputFreq_default_ = -1;

template<class ScalarType, class MV, class OP>
const std::string GCRODRSolMgr<ScalarType,MV,OP>::impResScale_default_ = "Norm of Preconditioned Initial Residual";

template<class ScalarType, class MV, class OP>
const std::string GCRODRSolMgr<ScalarType,MV,OP>::expResScale_default_ = "Norm of Initial Residual";

template<class ScalarType, class MV, class OP>
const std::string GCRODRSolMgr<ScalarType,MV,OP>::label_default_ = "Belos";

template<class ScalarType, class MV, class OP>
const std::string GCRODRSolMgr<ScalarType,MV,OP>::orthoType_default_ = "DGKS";

template<class ScalarType, class MV, class OP>
const Teuchos::RCP<std::ostream> GCRODRSolMgr<ScalarType,MV,OP>::outputStream_default_ = Teuchos::rcp(&std::cout,false);


// Empty Constructor
template<class ScalarType, class MV, class OP>
GCRODRSolMgr<ScalarType,MV,OP>::GCRODRSolMgr() {
  init();
}


// Basic Constructor
template<class ScalarType, class MV, class OP>
GCRODRSolMgr<ScalarType,MV,OP>::GCRODRSolMgr( 
					     const Teuchos::RCP<LinearProblem<ScalarType,MV,OP> > &problem,
					     const Teuchos::RCP<Teuchos::ParameterList> &pl ) {

  // initialize local pointers to null and local vars to default values
  init();

  TEST_FOR_EXCEPTION(problem == Teuchos::null, std::invalid_argument, "Problem not given to solver manager.");
  problem_ = problem;

  // set the parameters using the list that was passed in.
  if (!is_null(pl))
    setParameters( pl );  
}

// Common instructions executed in all constructors
template<class ScalarType, class MV, class OP>
void GCRODRSolMgr<ScalarType,MV,OP>::init() {
  outputStream_ = outputStream_default_;
  convtol_ = convtol_default_;
  orthoKappa_ = orthoKappa_default_;
  maxRestarts_ = maxRestarts_default_;
  maxIters_ = maxIters_default_;
  numBlocks_ = numBlocks_default_;
  recycledBlocks_ = recycledBlocks_default_;
  verbosity_ = verbosity_default_;
  outputStyle_ = outputStyle_default_;
  outputFreq_ = outputFreq_default_;
  orthoType_ = orthoType_default_;
  impResScale_ = impResScale_default_;
  expResScale_ = expResScale_default_;
  label_ = label_default_;
  isSet_ = false;
  numBlocks_ = 0;
  recycledBlocks_ = 0;
  keff = 0;
  r_ = Teuchos::null;
  V_ = Teuchos::null;
  U_ = Teuchos::null; 
  C_ = Teuchos::null;
  U1_ = Teuchos::null;
  C1_ = Teuchos::null;
  PP_ = Teuchos::null;
  HP_ = Teuchos::null;
  H2_ = Teuchos::null;
  R_ = Teuchos::null;
  H_ = Teuchos::null;
  B_ = Teuchos::null;
}

template<class ScalarType, class MV, class OP>
void GCRODRSolMgr<ScalarType,MV,OP>::setParameters( const Teuchos::RCP<Teuchos::ParameterList> &params )
{
  // Create the internal parameter list if ones doesn't already exist.
  if (params_ == Teuchos::null) {
    params_ = Teuchos::rcp( new Teuchos::ParameterList(*getValidParameters()) );
  }
  else {
    params->validateParameters(*getValidParameters());
  }

  // Check for maximum number of restarts
  if (params->isParameter("Maximum Restarts")) {
    maxRestarts_ = params->get("Maximum Restarts",maxRestarts_default_);

    // Update parameter in our list.
    params_->set("Maximum Restarts", maxRestarts_);
  }

  // Check for maximum number of iterations
  if (params->isParameter("Maximum Iterations")) {
    maxIters_ = params->get("Maximum Iterations",maxIters_default_);

    // Update parameter in our list and in status test.
    params_->set("Maximum Iterations", maxIters_);
    if (maxIterTest_!=Teuchos::null)
      maxIterTest_->setMaxIters( maxIters_ );
  }

  // Check for the maximum number of blocks.
  if (params->isParameter("Num Blocks")) {
    numBlocks_ = params->get("Num Blocks",numBlocks_default_);
    TEST_FOR_EXCEPTION(numBlocks_ <= 0, std::invalid_argument,
		       "Belos::GCRODRSolMgr: \"Num Blocks\" must be strictly positive.");

    // Update parameter in our list.
    params_->set("Num Blocks", numBlocks_);
  }

  // Check for the maximum number of blocks.
  if (params->isParameter("Num Recycled Blocks")) {
    recycledBlocks_ = params->get("Num Recycled Blocks",recycledBlocks_default_);
    TEST_FOR_EXCEPTION(recycledBlocks_ <= 0, std::invalid_argument,
		       "Belos::GCRODRSolMgr: \"Num Recycled Blocks\" must be strictly positive.");

    TEST_FOR_EXCEPTION(recycledBlocks_ >= numBlocks_, std::invalid_argument,
		       "Belos::GCRODRSolMgr: \"Num Recycled Blocks\" must be less than \"Num Blocks\".");

    // Update parameter in our list.
    params_->set("Num Recycled Blocks", recycledBlocks_);
  }

  // Check to see if the timer label changed.
  if (params->isParameter("Timer Label")) {
    std::string tempLabel = params->get("Timer Label", label_default_);

    // Update parameter in our list and solver timer
    if (tempLabel != label_) {
      label_ = tempLabel;
      params_->set("Timer Label", label_);
      std::string solveLabel = label_ + ": GCRODRSolMgr total solve time";
      timerSolve_ = Teuchos::TimeMonitor::getNewTimer(solveLabel);
    }
  }

  // Check if the orthogonalization changed.
  if (params->isParameter("Orthogonalization")) {
    std::string tempOrthoType = params->get("Orthogonalization",orthoType_default_);
    TEST_FOR_EXCEPTION( tempOrthoType != "DGKS" && tempOrthoType != "ICGS" && tempOrthoType != "IMGS", 
			std::invalid_argument,
			"Belos::GCRODRSolMgr: \"Orthogonalization\" must be either \"DGKS\", \"ICGS\", or \"IMGS\".");
    if (tempOrthoType != orthoType_) {
      orthoType_ = tempOrthoType;
      // Create orthogonalization manager
      if (orthoType_=="DGKS") {
	if (orthoKappa_ <= 0) {
	  ortho_ = Teuchos::rcp( new DGKSOrthoManager<ScalarType,MV,OP>( label_ ) );
	}
	else {
	  ortho_ = Teuchos::rcp( new DGKSOrthoManager<ScalarType,MV,OP>( label_ ) );
	  Teuchos::rcp_dynamic_cast<DGKSOrthoManager<ScalarType,MV,OP> >(ortho_)->setDepTol( orthoKappa_ );
	}
      }
      else if (orthoType_=="ICGS") {
	ortho_ = Teuchos::rcp( new ICGSOrthoManager<ScalarType,MV,OP>( label_ ) );
      } 
      else if (orthoType_=="IMGS") {
	ortho_ = Teuchos::rcp( new IMGSOrthoManager<ScalarType,MV,OP>( label_ ) );
      } 
    }  
  }

  // Check which orthogonalization constant to use.
  if (params->isParameter("Orthogonalization Constant")) {
    orthoKappa_ = params->get("Orthogonalization Constant",orthoKappa_default_);

    // Update parameter in our list.
    params_->set("Orthogonalization Constant",orthoKappa_);
    if (orthoType_=="DGKS") {
      if (orthoKappa_ > 0 && ortho_ != Teuchos::null) {
	Teuchos::rcp_dynamic_cast<DGKSOrthoManager<ScalarType,MV,OP> >(ortho_)->setDepTol( orthoKappa_ );
      }
    } 
  }

  // Check for a change in verbosity level
  if (params->isParameter("Verbosity")) {
    if (Teuchos::isParameterType<int>(*params,"Verbosity")) {
      verbosity_ = params->get("Verbosity", verbosity_default_);
    } else {
      verbosity_ = (int)Teuchos::getParameter<Belos::MsgType>(*params,"Verbosity");
    }

    // Update parameter in our list.
    params_->set("Verbosity", verbosity_);
    if (printer_ != Teuchos::null)
      printer_->setVerbosity(verbosity_);
  }

  // Check for a change in output style
  if (params->isParameter("Output Style")) {
    if (Teuchos::isParameterType<int>(*params,"Output Style")) {
      outputStyle_ = params->get("Output Style", outputStyle_default_);
    } else {
      outputStyle_ = (int)Teuchos::getParameter<Belos::OutputType>(*params,"Output Style");
    }

    // Update parameter in our list.
    params_->set("Output Style", outputStyle_);
    outputTest_ = Teuchos::null;
  }

  // output stream
  if (params->isParameter("Output Stream")) {
    outputStream_ = Teuchos::getParameter<Teuchos::RCP<std::ostream> >(*params,"Output Stream");

    // Update parameter in our list.
    params_->set("Output Stream", outputStream_);
    if (printer_ != Teuchos::null)
      printer_->setOStream( outputStream_ );
  }

  // frequency level
  if (verbosity_ & Belos::StatusTestDetails) {
    if (params->isParameter("Output Frequency")) {
      outputFreq_ = params->get("Output Frequency", outputFreq_default_);
    }

    // Update parameter in out list and output status test.
    params_->set("Output Frequency", outputFreq_);
    if (outputTest_ != Teuchos::null)
      outputTest_->setOutputFrequency( outputFreq_ );
  }

  // Create output manager if we need to.
  if (printer_ == Teuchos::null) {
    printer_ = Teuchos::rcp( new OutputManager<ScalarType>(verbosity_, outputStream_) );
  }  
  
  // Convergence
  typedef Belos::StatusTestCombo<ScalarType,MV,OP>  StatusTestCombo_t;
  typedef Belos::StatusTestGenResNorm<ScalarType,MV,OP>  StatusTestResNorm_t;

  // Check for convergence tolerance
  if (params->isParameter("Convergence Tolerance")) {
    convtol_ = params->get("Convergence Tolerance",convtol_default_);

    // Update parameter in our list and residual tests.
    params_->set("Convergence Tolerance", convtol_);
    if (impConvTest_ != Teuchos::null)
      impConvTest_->setTolerance( convtol_ );
    if (expConvTest_ != Teuchos::null)
      expConvTest_->setTolerance( convtol_ );
  }
 
  // Check for a change in scaling, if so we need to build new residual tests.
  if (params->isParameter("Implicit Residual Scaling")) {
    std::string tempImpResScale = Teuchos::getParameter<std::string>( *params, "Implicit Residual Scaling" );

    // Only update the scaling if it's different.
    if (impResScale_ != tempImpResScale) {
      Belos::ScaleType impResScaleType = convertStringToScaleType( tempImpResScale );
      impResScale_ = tempImpResScale;

      // Update parameter in our list and residual tests
      params_->set("Implicit Residual Scaling", impResScale_);
      if (impConvTest_ != Teuchos::null) {
        try { 
          impConvTest_->defineScaleForm( impResScaleType, Belos::TwoNorm );
        }
        catch (std::exception& e) { 
          // Delete the convergence test so it gets constructed again.
	  impConvTest_ = Teuchos::null;
          convTest_ = Teuchos::null;
        }
      }
    }      
  }
  
  if (params->isParameter("Explicit Residual Scaling")) {
    std::string tempExpResScale = Teuchos::getParameter<std::string>( *params, "Explicit Residual Scaling" );

    // Only update the scaling if it's different.
    if (expResScale_ != tempExpResScale) {
      Belos::ScaleType expResScaleType = convertStringToScaleType( tempExpResScale );
      expResScale_ = tempExpResScale;

      // Update parameter in our list and residual tests
      params_->set("Explicit Residual Scaling", expResScale_);
      if (expConvTest_ != Teuchos::null) {
        try { 
          expConvTest_->defineScaleForm( expResScaleType, Belos::TwoNorm );
        }
        catch (std::exception& e) {
          // Delete the convergence test so it gets constructed again.
	  expConvTest_ = Teuchos::null;
          convTest_ = Teuchos::null;
        }
      }
    }      
  }

  // Create status tests if we need to.

  // Basic test checks maximum iterations and native residual.
  if (maxIterTest_ == Teuchos::null)
    maxIterTest_ = Teuchos::rcp( new StatusTestMaxIters<ScalarType,MV,OP>( maxIters_ ) );

  // Implicit residual test, using the native residual to determine if convergence was achieved.
  if (impConvTest_ == Teuchos::null) {
    impConvTest_ = Teuchos::rcp( new StatusTestResNorm_t( convtol_ ) );
    impConvTest_->defineScaleForm( convertStringToScaleType(impResScale_), Belos::TwoNorm );
  }

  // Explicit residual test once the native residual is below the tolerance
  if (expConvTest_ == Teuchos::null) {
    expConvTest_ = Teuchos::rcp( new StatusTestResNorm_t( convtol_ ) );
    expConvTest_->defineResForm( StatusTestResNorm_t::Explicit, Belos::TwoNorm );
    expConvTest_->defineScaleForm( convertStringToScaleType(expResScale_), Belos::TwoNorm );
  }

  if (convTest_ == Teuchos::null) {
    convTest_ = Teuchos::rcp( new StatusTestCombo_t( StatusTestCombo_t::SEQ, impConvTest_, expConvTest_ ) );
  }

  sTest_ = Teuchos::rcp( new StatusTestCombo_t( StatusTestCombo_t::OR, maxIterTest_, convTest_ ) );
  
  // Create the status test output class.
  // This class manages and formats the output from the status test.
  StatusTestOutputFactory<ScalarType,MV,OP> stoFactory( outputStyle_ );
  outputTest_ = stoFactory.create( printer_, sTest_, outputFreq_, Passed+Failed+Undefined );

  // Set the solver string for the output test
  std::string solverDesc = " GCRODR ";
  outputTest_->setSolverDesc( solverDesc );

  // Create orthogonalization manager if we need to.
  if (ortho_ == Teuchos::null) {
    if (orthoType_=="DGKS") {
      if (orthoKappa_ <= 0) {
	ortho_ = Teuchos::rcp( new DGKSOrthoManager<ScalarType,MV,OP>( label_ ) );
      }
      else {
	ortho_ = Teuchos::rcp( new DGKSOrthoManager<ScalarType,MV,OP>( label_ ) );
	Teuchos::rcp_dynamic_cast<DGKSOrthoManager<ScalarType,MV,OP> >(ortho_)->setDepTol( orthoKappa_ );
      }
    }
    else if (orthoType_=="ICGS") {
      ortho_ = Teuchos::rcp( new ICGSOrthoManager<ScalarType,MV,OP>( label_ ) );
    } 
    else if (orthoType_=="IMGS") {
      ortho_ = Teuchos::rcp( new IMGSOrthoManager<ScalarType,MV,OP>( label_ ) );
    } 
    else {
      TEST_FOR_EXCEPTION(orthoType_!="ICGS"&&orthoType_!="DGKS"&&orthoType_!="IMGS",std::logic_error,
			 "Belos::GCRODRSolMgr(): Invalid orthogonalization type.");
    }  
  }

  // Create the timer if we need to.
  if (timerSolve_ == Teuchos::null) {
    std::string solveLabel = label_ + ": GCRODRSolMgr total solve time";
    timerSolve_ = Teuchos::TimeMonitor::getNewTimer(solveLabel);
  }

  // Inform the solver manager that the current parameters were set.
  isSet_ = true;
}

    
template<class ScalarType, class MV, class OP>
Teuchos::RCP<const Teuchos::ParameterList> GCRODRSolMgr<ScalarType,MV,OP>::getValidParameters() const {

  static Teuchos::RCP<const Teuchos::ParameterList> validPL;
  if (is_null(validPL)) {
    Teuchos::RCP<Teuchos::ParameterList> pl = Teuchos::parameterList();
    // Set all the valid parameters and their default values.
    pl->set("Convergence Tolerance", convtol_default_,
      "The relative residual tolerance that needs to be achieved by the\n"
      "iterative solver in order for the linear system to be declared converged.");
    pl->set("Maximum Restarts", maxRestarts_default_,
      "The maximum number of cycles allowed for each\n"
      "set of RHS solved.");
    pl->set("Maximum Iterations", maxIters_default_,
      "The maximum number of iterations allowed for each\n"
      "set of RHS solved.");
    pl->set("Block Size", blockSize_default_,
      "Block Size Parameter -- currently must be 1 for GCRODR");
    pl->set("Num Blocks", numBlocks_default_,
      "The maximum number of vectors allowed in the Krylov subspace\n"
      "for each set of RHS solved.");
    pl->set("Num Recycled Blocks", recycledBlocks_default_,
      "The maximum number of vectors in the recycled subspace." );
    pl->set("Verbosity", verbosity_default_,
      "What type(s) of solver information should be outputted\n"
      "to the output stream.");
    pl->set("Output Style", outputStyle_default_,
      "What style is used for the solver information outputted\n"
      "to the output stream.");
    pl->set("Output Frequency", outputFreq_default_,
      "How often convergence information should be outputted\n"
      "to the output stream.");  
    pl->set("Output Stream", outputStream_default_,
      "A reference-counted pointer to the output stream where all\n"
      "solver output is sent.");
    pl->set("Implicit Residual Scaling", impResScale_default_,
      "The type of scaling used in the implicit residual convergence test.");
    pl->set("Explicit Residual Scaling", expResScale_default_,
      "The type of scaling used in the explicit residual convergence test.");
    pl->set("Timer Label", label_default_,
      "The string to use as a prefix for the timer labels.");
    //  pl->set("Restart Timers", restartTimers_);
    pl->set("Orthogonalization", orthoType_default_,
      "The type of orthogonalization to use: DGKS, ICGS, IMGS");
    pl->set("Orthogonalization Constant",orthoKappa_default_,
      "The constant used by DGKS orthogonalization to determine\n"
      "whether another step of classical Gram-Schmidt is necessary.");
    validPL = pl;
  }
  return validPL;

}

// initializeStateStorage
template<class ScalarType, class MV, class OP>
void GCRODRSolMgr<ScalarType,MV,OP>::initializeStateStorage() {

    ScalarType zero = Teuchos::ScalarTraits<ScalarType>::zero();

    // Check if there is any multivector to clone from.
    Teuchos::RCP<const MV> rhsMV = problem_->getRHS();
    if (rhsMV == Teuchos::null) {
      // Nothing to do
      return;
    }
    else {

      // Initialize the state storage
      TEST_FOR_EXCEPTION(numBlocks_ > MVT::GetVecLength(*rhsMV),std::invalid_argument,
                         "Belos::GCRODRSolMgr::initializeStateStorage(): Cannot generate a Krylov basis with dimension larger the operator!");

      // If the subspace has not been initialized before, generate it using the RHS from lp_.
      if (U_ == Teuchos::null) {
        U_ = MVT::Clone( *rhsMV, recycledBlocks_+1 );
      }
      else {
        // Generate U_ by cloning itself ONLY if more space is needed.
        if (MVT::GetNumberVecs(*U_) < recycledBlocks_+1) {
          Teuchos::RCP<const MV> tmp = U_;
          U_ = MVT::Clone( *tmp, recycledBlocks_+1 );
        }
      }

      // If the subspace has not been initialized before, generate it using the RHS from lp_.
      if (C_ == Teuchos::null) {
        C_ = MVT::Clone( *rhsMV, recycledBlocks_+1 );
      }
      else {
        // Generate C_ by cloning itself ONLY if more space is needed.
        if (MVT::GetNumberVecs(*C_) < recycledBlocks_+1) {
          Teuchos::RCP<const MV> tmp = C_;
          C_ = MVT::Clone( *tmp, recycledBlocks_+1 );
        }
      }

      // If the subspace has not been initialized before, generate it using the RHS from lp_.
      if (V_ == Teuchos::null) {
        V_ = MVT::Clone( *rhsMV, numBlocks_+1 );
      }
      else {
        // Generate V_ by cloning itself ONLY if more space is needed.
        if (MVT::GetNumberVecs(*V_) < numBlocks_+1) {
          Teuchos::RCP<const MV> tmp = V_;
          V_ = MVT::Clone( *tmp, numBlocks_+1 );
        }
      }

      // If the subspace has not been initialized before, generate it using the RHS from lp_.
      if (U1_ == Teuchos::null) {
        U1_ = MVT::Clone( *rhsMV, recycledBlocks_+1 );
      }
      else {
        // Generate U1_ by cloning itself ONLY if more space is needed.
        if (MVT::GetNumberVecs(*U1_) < recycledBlocks_+1) {
          Teuchos::RCP<const MV> tmp = U1_;
          U1_ = MVT::Clone( *tmp, recycledBlocks_+1 );
        }
      }

      // If the subspace has not been initialized before, generate it using the RHS from lp_.
      if (C1_ == Teuchos::null) {
        C1_ = MVT::Clone( *rhsMV, recycledBlocks_+1 );
      }
      else {
        // Generate C1_ by cloning itself ONLY if more space is needed.
        if (MVT::GetNumberVecs(*U1_) < recycledBlocks_+1) {
          Teuchos::RCP<const MV> tmp = C1_;
          C1_ = MVT::Clone( *tmp, recycledBlocks_+1 );
        }
      }

      // Generate r_ only if it doesn't exist
      if (r_ == Teuchos::null)
        r_ = MVT::Clone( *rhsMV, 1 );

      // Size of tau_ will change during computation, so just be sure it starts with appropriate size
      tau_.resize(recycledBlocks_+1);

      // Size of work_ will change during computation, so just be sure it starts with appropriate size
      work_.resize(recycledBlocks_+1);

      // Size of ipiv_ will change during computation, so just be sure it starts with appropriate size
      ipiv_.resize(recycledBlocks_+1);

      // Generate H2_ only if it doesn't exist, otherwise resize it.
      if (H2_ == Teuchos::null)
        H2_ = Teuchos::rcp( new Teuchos::SerialDenseMatrix<int,ScalarType>( numBlocks_+recycledBlocks_+2, numBlocks_+recycledBlocks_+1 ) );
      else {
        if ( (H2_->numRows() != numBlocks_+recycledBlocks_+2) || (H2_->numCols() != numBlocks_+recycledBlocks_+1) )
          H2_->reshape( numBlocks_+recycledBlocks_+2, numBlocks_+recycledBlocks_+1 );
      }
      H2_->putScalar(zero);

      // Generate R_ only if it doesn't exist, otherwise resize it.
      if (R_ == Teuchos::null)
        R_ = Teuchos::rcp( new Teuchos::SerialDenseMatrix<int,ScalarType>( recycledBlocks_+1, recycledBlocks_+1 ) );
      else {
        if ( (R_->numRows() != recycledBlocks_+1) || (R_->numCols() != recycledBlocks_+1) )
          R_->reshape( recycledBlocks_+1, recycledBlocks_+1 );
      }
      R_->putScalar(zero);

      // Generate PP_ only if it doesn't exist, otherwise resize it.
      if (PP_ == Teuchos::null)
        PP_ = Teuchos::rcp( new Teuchos::SerialDenseMatrix<int,ScalarType>( numBlocks_+recycledBlocks_+2, recycledBlocks_+1 ) );
      else {
        if ( (PP_->numRows() != numBlocks_+recycledBlocks_+2) || (PP_->numCols() != recycledBlocks_+1) )
          PP_->reshape( numBlocks_+recycledBlocks_+2, recycledBlocks_+1 );
      }

      // Generate HP_ only if it doesn't exist, otherwise resize it.
      if (HP_ == Teuchos::null)
        HP_ = Teuchos::rcp( new Teuchos::SerialDenseMatrix<int,ScalarType>( numBlocks_+recycledBlocks_+2, numBlocks_+recycledBlocks_+1 ) );
      else {
        if ( (HP_->numRows() != numBlocks_+recycledBlocks_+2) || (HP_->numCols() != numBlocks_+recycledBlocks_+1) )
          HP_->reshape( numBlocks_+recycledBlocks_+2, numBlocks_+recycledBlocks_+1 );
      }

    } // end else
}

  
// solve()
template<class ScalarType, class MV, class OP>
ReturnType GCRODRSolMgr<ScalarType,MV,OP>::solve() {

  // Set the current parameters if they were not set before.
  // NOTE:  This may occur if the user generated the solver manager with the default constructor and 
  // then didn't set any parameters using setParameters().
  if (!isSet_) { setParameters( params_ ); }

  ScalarType one = Teuchos::ScalarTraits<ScalarType>::one();
  ScalarType zero = Teuchos::ScalarTraits<ScalarType>::zero();
  std::vector<int> index(numBlocks_+1);
  
  TEST_FOR_EXCEPTION(problem_ == Teuchos::null,GCRODRSolMgrLinearProblemFailure, "Belos::GCRODRSolMgr::solve(): Linear problem is not a valid object.");

  TEST_FOR_EXCEPTION(!problem_->isProblemSet(),GCRODRSolMgrLinearProblemFailure,"Belos::GCRODRSolMgr::solve(): Linear problem is not ready, setProblem() has not been called.");

  // Create indices for the linear systems to be solved.
  int numRHS2Solve = MVT::GetNumberVecs( *(problem_->getRHS()) );
  std::vector<int> currIdx(1);
  currIdx[0] = 0;

  // Inform the linear problem of the current linear system to solve.
  problem_->setLSIndex( currIdx );

  // Check the number of blocks and change them is necessary.
  int dim = MVT::GetVecLength( *(problem_->getRHS()) );  
  if (numBlocks_ > dim) {
    numBlocks_ = dim;
    printer_->stream(Warnings) << 
      "Warning! Requested Krylov subspace dimension is larger than operator dimension!" << std::endl <<
      " The maximum number of blocks allowed for the Krylov subspace will be adjusted to " << numBlocks_ << std::endl;
    params_->set("Num Blocks", numBlocks_);
  } 

  // Assume convergence is achieved, then let any failed convergence set this to false.
  bool isConverged = true;	

  // Initialize storage for all state variables
  initializeStateStorage();

  //////////////////////////////////////////////////////////////////////////////////////
  // Parameter list
  Teuchos::ParameterList plist;
  
  plist.set("Num Blocks",numBlocks_);
  plist.set("Recycled Blocks",recycledBlocks_);
  
  //////////////////////////////////////////////////////////////////////////////////////
  // GCRODR solver
  
  Teuchos::RCP<GCRODRIter<ScalarType,MV,OP> > gcrodr_iter;
  gcrodr_iter = Teuchos::rcp( new GCRODRIter<ScalarType,MV,OP>(problem_,printer_,outputTest_,ortho_,plist) );
  // Number of iterations required to generate initial recycle space (if needed)
  int prime_iterations = 0;

  // Enter solve() iterations
  {
    Teuchos::TimeMonitor slvtimer(*timerSolve_);
    
    while ( numRHS2Solve > 0 ) {
      
      // Reset the status test.  
      outputTest_->reset();
      
      //////////////////////////////////////////////////////////////////////////////////////
      // Initialize recycled subspace for GCRODR
      
      // If there is a subspace to recycle, recycle it, otherwise generate the initial recycled subspace.
      if (keff > 0) {
	TEST_FOR_EXCEPTION(keff < recycledBlocks_,GCRODRSolMgrRecyclingFailure,
			   "Belos::GCRODRSolMgr::solve(): Requested size of recycled subspace is not consistent with the current recycle subspace.");

	printer_->stream(Debug) << " Now solving RHS index " << currIdx[0] << " using recycled subspace of dimension " << keff << std::endl << std::endl;
	// Compute image of U_ under the new operator
	index.resize(keff);
	for (int ii=0; ii<keff; ++ii) { index[ii] = ii; }
	Teuchos::RCP<MV> Ctmp  = MVT::CloneView( *C_, index );
	Teuchos::RCP<MV> Utmp  = MVT::CloneView( *U_, index );
	Teuchos::RCP<MV> U1tmp = MVT::CloneView( *U1_, index );
	problem_->apply( *Utmp, *Ctmp );

	// Orthogonalize this block
	// Get a matrix to hold the orthonormalization coefficients.
        Teuchos::SerialDenseMatrix<int,ScalarType> Rtmp( Teuchos::View, *R_, keff, keff ); 
	int rank = ortho_->normalize(*Ctmp, Teuchos::rcp(&Rtmp,false));
	// Throw an error if we could not orthogonalize this block
	TEST_FOR_EXCEPTION(rank != keff,GCRODRSolMgrOrthoFailure,"Belos::GCRODRSolMgr::solve(): Failed to compute orthonormal basis for initial recycled subspace.");
	
	// U_ = U_*R^{-1}	
	// First, compute LU factorization of R
	int info = 0;
        ipiv_.resize(Rtmp.numRows());
	lapack.GETRF(Rtmp.numRows(),Rtmp.numCols(),Rtmp.values(),Rtmp.stride(),&ipiv_[0],&info);
	TEST_FOR_EXCEPTION(info != 0, GCRODRSolMgrLAPACKFailure,"Belos::GCRODRSolMgr::solve(): LAPACK _GETRF failed to compute an LU factorization.");
	
	// Now, form inv(R)
	int lwork = Rtmp.numRows();
        work_.resize(lwork);
	lapack.GETRI(Rtmp.numRows(),Rtmp.values(),Rtmp.stride(),&ipiv_[0],&work_[0],lwork,&info);
	TEST_FOR_EXCEPTION(info != 0, GCRODRSolMgrLAPACKFailure,"Belos::GCRODRSolMgr::solve(): LAPACK _GETRI failed to invert triangular matrix.");
	
        // U_ = U1_; (via a swap)
	MVT::MvTimesMatAddMv( one, *Utmp, Rtmp, zero, *U1tmp );
        Teuchos::RCP<MV> Uswap;
        Uswap = U_;
	U_ = U1_;
        U1_ = Uswap;
        Uswap = Teuchos::null;

        // Must reinitialize after swap
	index.resize(keff);
	for (int ii=0; ii<keff; ++ii) { index[ii] = ii; }
	Ctmp  = MVT::CloneView( *C_, index );
	Utmp  = MVT::CloneView( *U_, index );

	// Compute C_'*r_
	Teuchos::SerialDenseMatrix<int,ScalarType> Ctr(keff,1);
	problem_->computeCurrPrecResVec( &*r_ );
	MVT::MvTransMv( one, *Ctmp, *r_, Ctr );

	// Update solution ( x += U_*C_'*r_ )
	MVT::MvTimesMatAddMv( one, *Utmp, Ctr, one, *problem_->getCurrLHSVec() );
	
	// Update residual norm ( r -= C_*C_'*r_ )	
	MVT::MvTimesMatAddMv( -one, *Ctmp, Ctr, one, *r_ );

        // We recycled space from previous call
        prime_iterations = 0;

      }
      else {
	
	// Do one cycle of Gmres to "prime the pump" if there is no subspace to recycle
	printer_->stream(Debug) << " No recycled subspace available for RHS index " << currIdx[0] << std::endl << std::endl;
	
	Teuchos::ParameterList primeList;
	
	// Tell the block solver that the block size is one.
        primeList.set("Num Blocks",numBlocks_);
        primeList.set("Recycled Blocks",0);
	
	//  Create GCRODR iterator object to perform one cycle of GMRES.
	Teuchos::RCP<GCRODRIter<ScalarType,MV,OP> > gcrodr_prime_iter;
	gcrodr_prime_iter = Teuchos::rcp( new GCRODRIter<ScalarType,MV,OP>(problem_,printer_,outputTest_,ortho_,primeList) );
	
	// Create the first block in the current Krylov basis (residual).
	problem_->computeCurrPrecResVec( &*r_ );
        index.resize( 1 ); index[0] = 0;
	Teuchos::RCP<MV> v0 =  MVT::CloneView( *V_,  index );
	MVT::SetBlock(*r_,index,*v0); // V(:,0) = r

	// Set the new state and initialize the solver.
	GCRODRIterState<ScalarType,MV> newstate;
        index.resize( numBlocks_+1 );
        for (int ii=0; ii<(numBlocks_+1); ++ii) { index[ii] = ii; }
        newstate.V  = MVT::CloneView( *V_,  index );
	newstate.U = Teuchos::null;
	newstate.C = Teuchos::null;
        newstate.H = Teuchos::rcp( new Teuchos::SerialDenseMatrix<int,ScalarType>( Teuchos::View, *H2_, numBlocks_+1, numBlocks_, recycledBlocks_+1, recycledBlocks_+1 ) );
	newstate.B = Teuchos::null;
	newstate.curDim = 0;
	gcrodr_prime_iter->initialize(newstate);
	
	// Perform one cycle of GMRES
	bool primeConverged = false;
	try {
	  gcrodr_prime_iter->iterate();

	  // Check convergence first
	  if ( convTest_->getStatus() == Passed ) {
	    // we have convergence
	    primeConverged = true;
	  }
	}
	catch (const GCRODRIterOrthoFailure &e) {
	  // Try to recover the most recent least-squares solution
	  gcrodr_prime_iter->updateLSQR( gcrodr_prime_iter->getCurSubspaceDim() );
	  
	  // Check to see if the most recent least-squares solution yielded convergence.
	  sTest_->checkStatus( &*gcrodr_prime_iter );
	  if (convTest_->getStatus() == Passed)
	    primeConverged = true;
	}
	catch (const std::exception &e) {
	  printer_->stream(Errors) << "Error! Caught exception in GCRODRIter::iterate() at iteration " 
				   << gcrodr_prime_iter->getNumIters() << std::endl 
				   << e.what() << std::endl;
	  throw;
	}
        // Record number of iterations in generating initial recycle spacec
        prime_iterations = gcrodr_prime_iter->getNumIters();
           
	// Update the linear problem.
	Teuchos::RCP<MV> update = gcrodr_prime_iter->getCurrentUpdate();
	problem_->updateSolution( update, true );

	// Get the state.
	newstate = gcrodr_prime_iter->getState();
	int p = newstate.curDim;

	// Compute harmonic Ritz vectors 
	// NOTE:  The storage for the harmonic Ritz vectors (PP) is made one column larger 
        //        just in case we split a complex conjugate pair.
	// NOTE:  Generate a recycled subspace only if we have enough vectors.  If we converged
	//        too early, move on to the next linear system and try to generate a subspace again.
	if (recycledBlocks_ < p+1) {
	  int info = 0;
          Teuchos::RCP<Teuchos::SerialDenseMatrix<int,ScalarType> > PPtmp = rcp (new Teuchos::SerialDenseMatrix<int,ScalarType> ( Teuchos::View, *PP_, p, recycledBlocks_+1 ) );
          // getHarmonicVecs1 assumes PP has recycledBlocks_+1 columns available
	  keff = getHarmonicVecs1( p, *newstate.H, *PPtmp );
          // Hereafter, only keff columns of PP are needed
          PPtmp = rcp (new Teuchos::SerialDenseMatrix<int,ScalarType> ( Teuchos::View, *PP_, p, keff ) );
          // Now get views into C, U, V
          index.resize(keff);
          for (int ii=0; ii<keff; ++ii) { index[ii] = ii; }
          Teuchos::RCP<MV> Ctmp  = MVT::CloneView( *C_, index );
          Teuchos::RCP<MV> Utmp  = MVT::CloneView( *U_, index );
          Teuchos::RCP<MV> U1tmp = MVT::CloneView( *U1_, index );
	  index.resize(p);
          for (int ii=0; ii < p; ++ii) { index[ii] = ii; }
	  Teuchos::RCP<const MV> Vtmp = MVT::CloneView( *V_, index );

	  // Form U (the subspace to recycle)
          // U = newstate.V(:,1:p) * PP;
          MVT::MvTimesMatAddMv( one, *Vtmp, *PPtmp, zero, *U1tmp );

	  // Form orthonormalized C and adjust U so that C = A*U

	  // First, compute [Q, R] = qr(H*P);

          // Step #1: Form HP = H*P
	  Teuchos::SerialDenseMatrix<int,ScalarType> Htmp( Teuchos::View, *H2_, p+1, p, recycledBlocks_+1,recycledBlocks_+1);
	  Teuchos::SerialDenseMatrix<int,ScalarType> HPtmp( Teuchos::View, *HP_, p+1, keff );
	  HPtmp.multiply( Teuchos::NO_TRANS, Teuchos::NO_TRANS, one, Htmp, *PPtmp, zero );

	  // Step #1.5: Perform workspace size query for QR factorization of HP (the worksize will be placed in work_[0])
          int lwork = -1;
          tau_.resize(keff);
          lapack.GEQRF(HPtmp.numRows(),HPtmp.numCols(),HPtmp.values(),HPtmp.stride(),&tau_[0],&work_[0],lwork,&info);
	  TEST_FOR_EXCEPTION(info != 0, GCRODRSolMgrLAPACKFailure, "Belos::GCRODRSolMgr::solve(): LAPACK _GEQRF failed to compute a workspace size.");

          // Step #2: Compute QR factorization of HP
          lwork = (int)work_[0];
          work_.resize(lwork);
          lapack.GEQRF(HPtmp.numRows(),HPtmp.numCols(),HPtmp.values(),HPtmp.stride(),&tau_[0],&work_[0],lwork,&info);
	  TEST_FOR_EXCEPTION(info != 0, GCRODRSolMgrLAPACKFailure,  "Belos::GCRODRSolMgr::solve(): LAPACK _GEQRF failed to compute a QR factorization.");

          // Step #3: Explicitly construct Q and R factors 
	  // NOTE:  The upper triangular part of HP is copied into R and HP becomes Q.
          Teuchos::SerialDenseMatrix<int,ScalarType> Rtmp( Teuchos::View, *R_, keff, keff );
          for(int ii=0;ii<keff;ii++) { for(int jj=ii;jj<keff;jj++) Rtmp(ii,jj) = HPtmp(ii,jj); }
          lapack.ORGQR(HPtmp.numRows(),HPtmp.numCols(),HPtmp.numCols(),HPtmp.values(),HPtmp.stride(),&tau_[0],&work_[0],lwork,&info);
	  TEST_FOR_EXCEPTION(info != 0, GCRODRSolMgrLAPACKFailure, "Belos::GCRODRSolMgr::solve(): LAPACK _ORGQR failed to construct the Q factor.");

          // Now we have [Q,R] = qr(H*P)

	  // Now compute C = V(:,1:p+1) * Q 
	  index.resize( p+1 );
          for (int ii=0; ii < (p+1); ++ii) { index[ii] = ii; }
	  Vtmp = MVT::CloneView( *V_, index ); // need new view into V (p+1 vectors now; needed p above)
          MVT::MvTimesMatAddMv( one, *Vtmp, HPtmp, zero, *Ctmp );
	  	  
	  // Finally, compute U = U*R^{-1}. 
          // This unfortuntely requires me to form R^{-1} explicitly and execute U = U * R^{-1}, as 
          // backsolve capabilities don't exist in the Belos::MultiVec class

	  // Step #1: First, compute LU factorization of R
	  ipiv_.resize(Rtmp.numRows());
	  lapack.GETRF(Rtmp.numRows(),Rtmp.numCols(),Rtmp.values(),Rtmp.stride(),&ipiv_[0],&info);
	  TEST_FOR_EXCEPTION(info != 0, GCRODRSolMgrLAPACKFailure, "Belos::GCRODRSolMgr::solve(): LAPACK _GETRF failed to compute an LU factorization.");
	  
	  // Step #2: Form inv(R)
	  lwork = Rtmp.numRows();
	  work_.resize(lwork);
	  lapack.GETRI(Rtmp.numRows(),Rtmp.values(),Rtmp.stride(),&ipiv_[0],&work_[0],lwork,&info);
	  TEST_FOR_EXCEPTION(info != 0, GCRODRSolMgrLAPACKFailure,  "Belos::GCRODRSolMgr::solve(): LAPACK _GETRI failed to invert triangular matrix.");
	  
          // Step #3: Let U = U * R^{-1}
	  MVT::MvTimesMatAddMv( one, *U1tmp, Rtmp, zero, *Utmp );

	  printer_->stream(Debug) << " Generated recycled subspace using RHS index " << currIdx[0] << " of dimension " << keff << std::endl << std::endl;

	}  // if (recycledBlocks_ < p+1)

        // Return to outer loop if the priming solve converged, set the next linear system.
	if (primeConverged) {
	  // Inform the linear problem that we are finished with this block linear system.
	  problem_->setCurrLS();

          // Update indices for the linear systems to be solved.
          numRHS2Solve -= 1;
          if ( numRHS2Solve > 0 ) {
            currIdx[0]++;
 
            // Set the next indices.
            problem_->setLSIndex( currIdx );
          }
          else {
            currIdx.resize( numRHS2Solve );
          }

	  continue;
	}
      } // if (keff > 0) ...
      
      // Prepare for the Gmres iterations with the recycled subspace.

      // Set the current number of recycled blocks and subspace dimension with the GCRO-DR iteration.
      gcrodr_iter->setSize( keff, numBlocks_ );
      
      // Reset the number of iterations.
      gcrodr_iter->resetNumIters(prime_iterations);

      // Reset the number of calls that the status test output knows about.
      outputTest_->resetNumCalls();

      // Compute the residual after the priming solve, it will be the first block in the current Krylov basis.
      problem_->computeCurrPrecResVec( &*r_ );
      index.resize( 1 ); index[0] = 0;
      Teuchos::RCP<MV> v0 =  MVT::CloneView( *V_,  index );
      MVT::SetBlock(*r_,index,*v0); // V(:,0) = r

      // Set the new state and initialize the solver.
      GCRODRIterState<ScalarType,MV> newstate;
      index.resize( numBlocks_+1 );
      for (int ii=0; ii<(numBlocks_+1); ++ii) { index[ii] = ii; }
      newstate.V  = MVT::CloneView( *V_,  index );
      index.resize( keff );
      for (int ii=0; ii<keff; ++ii) { index[ii] = ii; }
      newstate.C  = MVT::CloneView( *C_,  index );
      newstate.U  = MVT::CloneView( *U_,  index );
      newstate.B = Teuchos::rcp( new Teuchos::SerialDenseMatrix<int,ScalarType>( Teuchos::View, *H2_,         keff, numBlocks_,    0, keff ) );
      newstate.H = Teuchos::rcp( new Teuchos::SerialDenseMatrix<int,ScalarType>( Teuchos::View, *H2_, numBlocks_+1, numBlocks_, keff, keff ) );
      newstate.curDim = 0;
      gcrodr_iter->initialize(newstate);

      // variables needed for inner loop
      int numRestarts = 0;
      std::vector<MagnitudeType> d(keff);
      Teuchos::RCP<Teuchos::SerialDenseMatrix<int,ScalarType> > H2tmp;
      Teuchos::RCP<MV> Utmp;
      Teuchos::RCP<MV> Ctmp;
      Teuchos::RCP<MV> U1tmp;
      Teuchos::RCP<MV> C1tmp;
      Teuchos::RCP<MV> Vtmp;
      Teuchos::RCP<Teuchos::SerialDenseMatrix<int,ScalarType> > PPtmp;
      while(1) {
	
	// tell gcrodr_iter to iterate
	try {
	  gcrodr_iter->iterate();
	  
	  ////////////////////////////////////////////////////////////////////////////////////
	  //
	  // check convergence first
 	  //
	  ////////////////////////////////////////////////////////////////////////////////////
	  if ( convTest_->getStatus() == Passed ) {
	    // we have convergence
	    break;  // break from while(1){gcrodr_iter->iterate()}
	  }
	  ////////////////////////////////////////////////////////////////////////////////////
	  //
	  // check for maximum iterations
	  //
	  ////////////////////////////////////////////////////////////////////////////////////
	  else if ( maxIterTest_->getStatus() == Passed ) {
	    // we don't have convergence
	    isConverged = false;
	    break;  // break from while(1){gcrodr_iter->iterate()}
	  }
	  ////////////////////////////////////////////////////////////////////////////////////
	  //
	  // check for restarting, i.e. the subspace is full
	  //
	  ////////////////////////////////////////////////////////////////////////////////////
	  else if ( gcrodr_iter->getCurSubspaceDim() == gcrodr_iter->getMaxSubspaceDim() ) {

            // Update the recycled subspace even if we have hit the maximum number of restarts.
 
	    // Get the state.
	    GCRODRIterState<ScalarType,MV> oldState = gcrodr_iter->getState();
            int p = oldState.curDim;	   

            // Update the linear problem.
            Teuchos::RCP<MV> update = gcrodr_iter->getCurrentUpdate();
            problem_->updateSolution( update, true );

            // Take the norm of the recycled vectors
            index.resize(keff);
            for (int ii=0; ii<keff; ++ii) { index[ii] = ii; }
            Teuchos::RCP<MV> Utmp  = MVT::CloneView( *U_, index );
            d.resize(keff);
            MVT::MvNorm( *Utmp, d );
            for (int i=0; i<keff; ++i) {
	      d[i] = one / d[i];
	    }
	    MVT::MvScale( *Utmp, d );
          
            // Get view into current "full" upper Hessnburg matrix
            H2tmp = Teuchos::rcp( new Teuchos::SerialDenseMatrix<int,ScalarType>( Teuchos::View, *H2_, p+keff+1, p+keff ) );

            // Insert D into the leading keff x keff  block of H2
            for (int i=0; i<keff; ++i) {
              (*H2tmp)(i,i) = d[i];
            }
            
            // Compute the harmoic Ritz pairs for the generalized eigenproblem
            // getHarmonicVecs2 assumes PP has recycledBlocks_+1 columns available
            PPtmp = rcp( new Teuchos::SerialDenseMatrix<int,ScalarType> ( Teuchos::View, *PP_, p+keff, recycledBlocks_+1 ) );
	    int keff_new = getHarmonicVecs2( keff, p, *H2tmp, oldState.V, *PPtmp );

	    printer_->stream(Debug) << " Generated new recycled subspace using RHS index " << currIdx[0] << " of dimension " << keff_new << std::endl << std::endl;

	    // Code to form new U, C
	    // U = [U V(:,1:p)] * P; (in two steps)

	    // U(:,1:keff) = matmul(U(:,1:keff_old),PP(1:keff_old,1:keff)) (step 1)
            index.resize( keff );
            for (int ii=0; ii<keff; ++ii) { index[ii] = ii; }
            Utmp  = MVT::CloneView( *U_,  index );
            index.resize( keff_new );
            for (int ii=0; ii<keff_new; ++ii) { index[ii] = ii; }
            U1tmp  = MVT::CloneView( *U1_,  index );
            PPtmp = rcp( new Teuchos::SerialDenseMatrix<int,ScalarType> ( Teuchos::View, *PP_, keff, keff_new ) );
	    MVT::MvTimesMatAddMv( one, *Utmp, *PPtmp, zero, *U1tmp );

	    // U(:,1:keff) = U(:,1:keff) + matmul(V(:,1:m-k),PP(keff_old+1:m-k+keff_old,1:keff)) (step 2)
	    index.resize(p);
	    for (int ii=0; ii < p; ii++) { index[ii] = ii; }
	    Vtmp = MVT::CloneView( *V_, index );
            PPtmp = rcp( new Teuchos::SerialDenseMatrix<int,ScalarType> ( Teuchos::View, *PP_, p, keff_new, keff ) );
	    MVT::MvTimesMatAddMv( one, *Vtmp, *PPtmp, one, *U1tmp );
	   
	    // Form HP = H*P
            PPtmp = rcp( new Teuchos::SerialDenseMatrix<int,ScalarType> ( Teuchos::View, *PP_, p+keff, keff_new ) );
            Teuchos::SerialDenseMatrix<int,ScalarType> HPtmp( Teuchos::View, *HP_, p+keff+1, keff_new );
	    HPtmp.multiply(Teuchos::NO_TRANS,Teuchos::NO_TRANS,one,*H2tmp,*PPtmp,zero);

	    // Workspace size query for QR factorization of HP (the worksize will be placed in work_[0])
	    int info = 0, lwork = -1;
	    tau_.resize(keff_new);
	    lapack.GEQRF(HPtmp.numRows(),HPtmp.numCols(),HPtmp.values(),HPtmp.stride(),&tau_[0],&work_[0],lwork,&info);
	    TEST_FOR_EXCEPTION(info != 0,GCRODRSolMgrLAPACKFailure,"Belos::GCRODRSolMgr::solve(): LAPACK _GEQRF failed to compute a workspace size.");
	    
	    lwork = (int)work_[0];
	    work_.resize(lwork);
	    lapack.GEQRF(HPtmp.numRows(),HPtmp.numCols(),HPtmp.values(),HPtmp.stride(),&tau_[0],&work_[0],lwork,&info);
	    TEST_FOR_EXCEPTION(info != 0,GCRODRSolMgrLAPACKFailure,"Belos::GCRODRSolMgr::solve(): LAPACK _GEQRF failed to compute a QR factorization.");

	    // Explicitly construct Q and R factors 
	    // NOTE:  The upper triangular part of HP is copied into R and HP becomes Q.
            Teuchos::SerialDenseMatrix<int,ScalarType> Rtmp( Teuchos::View, *R_, keff_new, keff_new );
            for(int i=0;i<keff_new;i++) { for(int j=i;j<keff_new;j++) Rtmp(i,j) = HPtmp(i,j); }
	    lapack.ORGQR(HPtmp.numRows(),HPtmp.numCols(),HPtmp.numCols(),HPtmp.values(),HPtmp.stride(),&tau_[0],&work_[0],lwork,&info);
	    TEST_FOR_EXCEPTION(info != 0,GCRODRSolMgrLAPACKFailure,"Belos::GCRODRSolMgr::solve(): LAPACK _ORGQR failed to construct the Q factor.");

	    // Form orthonormalized C and adjust U accordingly so that C = A*U	    
	    // C = [C V] * Q;

	    // C(:,1:keff) = matmul(C(:,1:keff_old),QQ(1:keff_old,1:keff))
	    index.resize(keff);
	    for (int i=0; i < keff; i++) { index[i] = i; }
            Ctmp  = MVT::CloneView( *C_,  index );
	    index.resize(keff_new);
	    for (int i=0; i < keff_new; i++) { index[i] = i; }
            C1tmp  = MVT::CloneView( *C1_,  index );
            PPtmp = rcp( new Teuchos::SerialDenseMatrix<int,ScalarType> ( Teuchos::View, *HP_, keff, keff_new ) );
	    MVT::MvTimesMatAddMv( one, *Ctmp, *PPtmp, zero, *C1tmp );

	    // Now compute C += V(:,1:p+1) * Q 
	    index.resize( p+1 );
	    for (int i=0; i < p+1; ++i) { index[i] = i; }
	    Vtmp = MVT::CloneView( *V_, index );
            PPtmp = rcp( new Teuchos::SerialDenseMatrix<int,ScalarType> ( Teuchos::View, *HP_, p+1, keff_new, keff, 0 ) );
	    MVT::MvTimesMatAddMv( one, *Vtmp, *PPtmp, one, *C1tmp );

            // C_ = C1_; (via a swap)
            Teuchos::RCP<MV> Cswap;
            Cswap = C_;
            C_ = C1_;
            C1_ = Cswap;
            Cswap = Teuchos::null;

	    // Finally, compute U_ = U_*R^{-1}	
	    // First, compute LU factorization of R
            ipiv_.resize(Rtmp.numRows());
	    lapack.GETRF(Rtmp.numRows(),Rtmp.numCols(),Rtmp.values(),Rtmp.stride(),&ipiv_[0],&info);
	    TEST_FOR_EXCEPTION(info != 0,GCRODRSolMgrLAPACKFailure,"Belos::GCRODRSolMgr::solve(): LAPACK _GETRF failed to compute an LU factorization.");
	    
	    // Now, form inv(R)
	    lwork = Rtmp.numRows();
	    work_.resize(lwork);
	    lapack.GETRI(Rtmp.numRows(),Rtmp.values(),Rtmp.stride(),&ipiv_[0],&work_[0],lwork,&info);
	    TEST_FOR_EXCEPTION(info != 0, GCRODRSolMgrLAPACKFailure,"Belos::GCRODRSolMgr::solve(): LAPACK _GETRI failed to compute an LU factorization.");
	  
	    index.resize(keff_new);
	    for (int i=0; i < keff_new; i++) { index[i] = i; }
            Utmp  = MVT::CloneView( *U_,  index );
	    MVT::MvTimesMatAddMv( one, *U1tmp, Rtmp, zero, *Utmp );

            // NOTE:  If we have hit the maximum number of restarts then quit
	    if ( numRestarts >= maxRestarts_ ) {
	      isConverged = false;
	      break; // break from while(1){gcrodr_iter->iterate()}
	    }
	    numRestarts++;
	    
	    printer_->stream(Debug) << " Performing restart number " << numRestarts << " of " << maxRestarts_ << std::endl << std::endl;

            // Create the restart vector (first block in the current Krylov basis)
            problem_->computeCurrPrecResVec( &*r_ );
            index.resize( 1 ); index[0] = 0;
            Teuchos::RCP<MV> v0 =  MVT::CloneView( *V_,  index );
            MVT::SetBlock(*r_,index,*v0); // V(:,0) = r
	    
	    // Set the current number of recycled blocks and subspace dimension with the GCRO-DR iteration.
            if (keff != keff_new) {
  	      keff = keff_new;
	      gcrodr_iter->setSize( keff, numBlocks_ );
              Teuchos::SerialDenseMatrix<int,ScalarType> b1( Teuchos::View, *H2_, recycledBlocks_+2, 1, 0, recycledBlocks_ );
              b1.putScalar(zero);
            }

	    // Set the new state and initialize the solver.
	    GCRODRIterState<ScalarType,MV> restartState;
            index.resize( numBlocks_+1 );
            for (int ii=0; ii<(numBlocks_+1); ++ii) { index[ii] = ii; }
            restartState.V  = MVT::CloneView( *V_,  index );
            index.resize( keff );
            for (int ii=0; ii<keff; ++ii) { index[ii] = ii; }
            restartState.U  = MVT::CloneView( *U_,  index );
            restartState.C  = MVT::CloneView( *C_,  index );
            restartState.B = Teuchos::rcp( new Teuchos::SerialDenseMatrix<int,ScalarType>( Teuchos::View, *H2_, keff,         numBlocks_,    0, keff ) );
            restartState.H = Teuchos::rcp( new Teuchos::SerialDenseMatrix<int,ScalarType>( Teuchos::View, *H2_, numBlocks_+1, numBlocks_, keff, keff ) );
            restartState.curDim = 0;
            gcrodr_iter->initialize(restartState);

	  } // end of restarting
	  
	  ////////////////////////////////////////////////////////////////////////////////////
	  //
	  // we returned from iterate(), but none of our status tests Passed.
	  // something is wrong, and it is probably our fault.
	  //
	  ////////////////////////////////////////////////////////////////////////////////////
	  
	  else {
	    TEST_FOR_EXCEPTION(true,std::logic_error,"Belos::GCRODRSolMgr::solve(): Invalid return from GCRODRIter::iterate().");
	  }
	}
        catch (const GCRODRIterOrthoFailure &e) {
	  // Try to recover the most recent least-squares solution
	  gcrodr_iter->updateLSQR( gcrodr_iter->getCurSubspaceDim() );
	  
	  // Check to see if the most recent least-squares solution yielded convergence.
	  sTest_->checkStatus( &*gcrodr_iter );
	  if (convTest_->getStatus() != Passed)
	    isConverged = false;
	  break;
        }
        catch (const std::exception &e) {
	  printer_->stream(Errors) << "Error! Caught exception in GCRODRIter::iterate() at iteration " 
	                           << gcrodr_iter->getNumIters() << std::endl 
				   << e.what() << std::endl;
          throw;
	}
      }
     
      // Compute the current solution.
      // Update the linear problem.
      Teuchos::RCP<MV> update = gcrodr_iter->getCurrentUpdate();
      problem_->updateSolution( update, true );

      // Inform the linear problem that we are finished with this block linear system.
      problem_->setCurrLS();

      // Update indices for the linear systems to be solved.
      numRHS2Solve -= 1;
      if ( numRHS2Solve > 0 ) {
	currIdx[0]++;

        // Set the next indices.
        problem_->setLSIndex( currIdx );
      }
      else {
        currIdx.resize( numRHS2Solve );
      }
 
    }// while ( numRHS2Solve > 0 )
    
  }
  
  // print final summary
  sTest_->print( printer_->stream(FinalSummary) );
  
  // print timing information
  Teuchos::TimeMonitor::summarize( printer_->stream(TimingDetails) );
 
  // get iteration information for this solve
  numIters_ = maxIterTest_->getNumIters();
 
  if (!isConverged) {
    return Unconverged; // return from GCRODRSolMgr::solve() 
  }
  return Converged; // return from GCRODRSolMgr::solve() 
}

//  Compute the harmonic eigenpairs of the projected, dense system.
template<class ScalarType, class MV, class OP>
int GCRODRSolMgr<ScalarType,MV,OP>::getHarmonicVecs1(int m, 
						     const Teuchos::SerialDenseMatrix<int,ScalarType>& HH, 
						     Teuchos::SerialDenseMatrix<int,ScalarType>& PP) {
  int i, j;
  bool xtraVec = false;
  ScalarType one = Teuchos::ScalarTraits<ScalarType>::one();
  ScalarType zero = Teuchos::ScalarTraits<ScalarType>::zero();

  // Real and imaginary eigenvalue components
  std::vector<MagnitudeType> wr(m), wi(m);

  // Real and imaginary (right) eigenvectors; Don't zero out matrix when constructing
  Teuchos::SerialDenseMatrix<int,ScalarType> vr(m,m,false);

  // Magnitude of harmonic Ritz values
  std::vector<MagnitudeType> w(m);

  // Sorted order of harmonic Ritz values, also used for DGEEV
  std::vector<int> iperm(m);

  // Size of workspace and workspace for DGEEV
  int lwork = 4*m;
  std::vector<ScalarType> work(lwork);

  // Output info
  int info = 0;

  // Solve linear system:  H_m^{-H}*e_m 
  Teuchos::SerialDenseMatrix<int, ScalarType> HHt( HH, Teuchos::TRANS );
  Teuchos::SerialDenseVector<int, ScalarType> e_m( m );
  e_m[m-1] = one;
  lapack.GESV(m, 1, HHt.values(), HHt.stride(), &iperm[0], e_m.values(), e_m.stride(), &info);
  TEST_FOR_EXCEPTION(info != 0, GCRODRSolMgrLAPACKFailure, "Belos::GCRODRSolMgr::solve(): LAPACK GESV failed to compute a solution.");

  // Compute H_m + d*H_m^{-H}*e_m*e_m^H
  ScalarType d = HH(m, m-1) * HH(m, m-1);
  Teuchos::SerialDenseMatrix<int, ScalarType> harmHH( HH );
  for( i=0; i<m; ++i )
    harmHH(i, m-1) += d * e_m[i];

  // Revise to do query for optimal workspace first
  // Create simple storage for the left eigenvectors, which we don't care about.
  const int ldvl = m;
  ScalarType* vl = 0;
  lapack.GEEV('N', 'V', m, harmHH.values(), harmHH.stride(), &wr[0], &wi[0],
	      vl, ldvl, vr.values(), vr.stride(), &work[0], lwork, &info);
  TEST_FOR_EXCEPTION(info != 0, GCRODRSolMgrLAPACKFailure,"Belos::GCRODRSolMgr::solve(): LAPACK GEEV failed to compute eigensolutions.");

  // Construct magnitude of each harmonic Ritz value
  for( i=0; i<m; ++i )
    w[i] = Teuchos::ScalarTraits<ScalarType>::squareroot( wr[i]*wr[i] + wi[i]*wi[i] );

  // Construct magnitude of each harmonic Ritz value
  this->sort(w, m, iperm);

  // Determine exact size for PP (i.e., determine if we need to store an additional vector)
  if (wi[iperm[recycledBlocks_-1]] != zero) {
    int countImag = 0;
    for ( i=0; i<recycledBlocks_; ++i ) {
      if (wi[iperm[i]] != zero)
	countImag++;
    }
    // Check to see if this count is even or odd:
    if (countImag % 2)
      xtraVec = true;
  }

  // Select recycledBlocks_ smallest eigenvectors
  for( i=0; i<recycledBlocks_; ++i ) {
    for( j=0; j<m; j++ ) {
      PP(j,i) = vr(j,iperm[i]);
    }
  }
  
  if (xtraVec) { // we need to store one more vector
    if (wi[iperm[recycledBlocks_-1]] > 0) { // I picked the "real" component
      for( j=0; j<m; ++j ) {   // so get the "imag" component
	PP(j,recycledBlocks_) = vr(j,iperm[recycledBlocks_-1]+1);
      }
    }
    else { //  I picked the "imag" component
      for( j=0; j<m; ++j ) {   // so get the "real" component
	PP(j,recycledBlocks_) = vr(j,iperm[recycledBlocks_-1]-1);
      }
    }
  }

  // Return whether we needed to store an additional vector
  if (xtraVec) {
    return recycledBlocks_+1;
  }
  return recycledBlocks_;
}

//  Compute the harmonic eigenpairs of the projected, dense system.
template<class ScalarType, class MV, class OP>
int GCRODRSolMgr<ScalarType,MV,OP>::getHarmonicVecs2(int keff, int m, 
						     const Teuchos::SerialDenseMatrix<int,ScalarType>& HH, 
						     const Teuchos::RCP<const MV>& VV,
						     Teuchos::SerialDenseMatrix<int,ScalarType>& PP) {
  int i, j;
  int m2 = HH.numCols();
  bool xtraVec = false;
  ScalarType one = Teuchos::ScalarTraits<ScalarType>::one();
  ScalarType zero = Teuchos::ScalarTraits<ScalarType>::zero();
  std::vector<int> index;
  
  // Real and imaginary eigenvalue components
  std::vector<ScalarType> wr(m2), wi(m2);

  // Magnitude of harmonic Ritz values
  std::vector<MagnitudeType> w(m2);

  // Real and imaginary (right) eigenvectors; Don't zero out matrix when constructing
  Teuchos::SerialDenseMatrix<int,ScalarType> vr(m2,m2,false);

  // Sorted order of harmonic Ritz values
  std::vector<int> iperm(m2);

  // Form matrices for generalized eigenproblem
  
  // B = H2' * H2; Don't zero out matrix when constructing
  Teuchos::SerialDenseMatrix<int,ScalarType> B(m2,m2,false);
  B.multiply(Teuchos::TRANS,Teuchos::NO_TRANS,one,HH,HH,zero);
  
  // A_tmp = | C'*U        0 |
  //         | V_{m+1}'*U  I |
  Teuchos::SerialDenseMatrix<int,ScalarType> A_tmp( keff+m+1, keff+m );

  // A_tmp(1:keff,1:keff) = C' * U;
  index.resize(keff);
  for (int i=0; i<keff; ++i) { index[i] = i; }
  Teuchos::RCP<MV> Ctmp  = MVT::CloneView( *C_, index );
  Teuchos::RCP<MV> Utmp  = MVT::CloneView( *U_, index );
  Teuchos::SerialDenseMatrix<int,ScalarType> A11( Teuchos::View, A_tmp, keff, keff );
  MVT::MvTransMv( one, *Ctmp, *Utmp, A11 );

  // A_tmp(keff+1:m-k+keff+1,1:keff) = V' * U;
  Teuchos::SerialDenseMatrix<int,ScalarType> A21( Teuchos::View, A_tmp, m+1, keff, keff );
  index.resize(m+1);
  for (i=0; i < m+1; i++) { index[i] = i; }
  Teuchos::RCP<const MV> Vp = MVT::CloneView( *VV, index );
  MVT::MvTransMv( one, *Vp, *Utmp, A21 );

  // A_tmp(keff+1:m-k+keff,keff+1:m-k+keff) = eye(m-k);
  for( i=keff; i<keff+m; i++ ) {
    A_tmp(i,i) = one;
  }

  // A = H2' * A_tmp;
  Teuchos::SerialDenseMatrix<int,ScalarType> A( m2, A_tmp.numCols() );
  A.multiply( Teuchos::TRANS, Teuchos::NO_TRANS, one, HH, A_tmp, zero );

  // Compute k smallest harmonic Ritz pairs
  // SUBROUTINE DGGEVX( BALANC, JOBVL, JOBVR, SENSE, N, A, LDA, B, LDB,
  //                   ALPHAR, ALPHAI, BETA, VL, LDVL, VR, LDVR, ILO,
  //                   IHI, LSCALE, RSCALE, ABNRM, BBNRM, RCONDE,
  //                   RCONDV, WORK, LWORK, IWORK, BWORK, INFO )
  // MLP: 'SCALING' in DGGEVX generates incorrect eigenvalues. Therefore, only permuting
  char balanc='P', jobvl='N', jobvr='V', sense='N';	
  int ld = A.numRows();
  int lwork = 6*ld;
  int ldvl = ld, ldvr = ld;
  int info = 0,ilo = 0,ihi = 0;
  ScalarType abnrm = zero, bbnrm = zero;
  ScalarType *vl = 0; // This is never referenced by dggevx if jobvl == 'N'
  std::vector<ScalarType> beta(ld);
  std::vector<ScalarType> work(lwork);
  std::vector<MagnitudeType> lscale(ld), rscale(ld);  
  std::vector<MagnitudeType> rconde(ld), rcondv(ld);
  std::vector<int> iwork(ld+6);
  int *bwork = 0; // If sense == 'N', bwork is never referenced
  lapack.GGEVX(balanc, jobvl, jobvr, sense, ld, A.values(), ld, B.values(), ld, &wr[0], &wi[0], 
               &beta[0], vl, ldvl, vr.values(), ldvr, &ilo, &ihi, &lscale[0], &rscale[0], 
               &abnrm, &bbnrm, &rconde[0], &rcondv[0], &work[0], lwork, &iwork[0], bwork, &info);
  TEST_FOR_EXCEPTION(info != 0, GCRODRSolMgrLAPACKFailure, "Belos::GCRODRSolMgr::solve(): LAPACK GGEVX failed to compute eigensolutions.");
  
  // Construct magnitude of each harmonic Ritz value
  // NOTE : Forming alpha/beta *should* be okay here, given assumptions on construction of matrix pencil above
  for( i=0; i<ld; i++ ) {
    w[i] = Teuchos::ScalarTraits<ScalarType>::squareroot( (wr[i]/beta[i])*(wr[i]/beta[i]) + (wi[i]/beta[i])*(wi[i]/beta[i]) );
  }

  // Construct magnitude of each harmonic Ritz value
  this->sort(w,ld,iperm);

  // Determine exact size for PP (i.e., determine if we need to store an additional vector)
  if (wi[iperm[ld-recycledBlocks_]] != zero) {
    int countImag = 0;
    for ( i=ld-recycledBlocks_; i<ld; i++ ) {
      if (wi[iperm[i]] != zero)
	countImag++;
    }
    // Check to see if this count is even or odd:
    if (countImag % 2)
      xtraVec = true;
  }
  
  // Select recycledBlocks_ smallest eigenvectors
  for( i=0; i<recycledBlocks_; i++ ) {
    for( j=0; j<ld; j++ ) {
      PP(j,i) = vr(j,iperm[ld-recycledBlocks_+i]);
    }
  }
  
  if (xtraVec) { // we need to store one more vector
    if (wi[iperm[ld-recycledBlocks_]] > 0) { // I picked the "real" component
      for( j=0; j<ld; j++ ) {   // so get the "imag" component
	PP(j,recycledBlocks_) = vr(j,iperm[ld-recycledBlocks_]+1);
      }
    }
    else { // I picked the "imag" component
      for( j=0; j<ld; j++ ) {   // so get the "real" component
	PP(j,recycledBlocks_) = vr(j,iperm[ld-recycledBlocks_]-1);
      }
    }
  }
  
  // Return whether we needed to store an additional vector
  if (xtraVec) {
    return recycledBlocks_+1;
  }
  return recycledBlocks_;
}


// This method sorts list of n floating-point numbers and return permutation vector
template<class ScalarType, class MV, class OP>
void GCRODRSolMgr<ScalarType,MV,OP>::sort(std::vector<ScalarType>& dlist, int n, std::vector<int>& iperm) {
  int l, r, j, i, flag;
  int    RR2;
  double dRR, dK;
  
  // Initialize the permutation vector.
  for(j=0;j<n;j++)
    iperm[j] = j;
  
  if (n <= 1) return;
  
  l    = n / 2 + 1;
  r    = n - 1;
  l    = l - 1;
  dRR  = dlist[l - 1];
  dK   = dlist[l - 1];
  
  RR2 = iperm[l - 1];
  while (r != 0) {
    j = l;
    flag = 1;
    
    while (flag == 1) {
      i = j;
      j = j + j;
      
      if (j > r + 1)
	flag = 0;
      else {
	if (j < r + 1)
	  if (dlist[j] > dlist[j - 1]) j = j + 1;
	
	if (dlist[j - 1] > dK) {
	  dlist[i - 1] = dlist[j - 1];
	  iperm[i - 1] = iperm[j - 1];
	}
	else {
	  flag = 0;
	}
      }
    }
    dlist[i - 1] = dRR;
    iperm[i - 1] = RR2;
    
    if (l == 1) {
      dRR  = dlist [r];
      RR2 = iperm[r];
      dK = dlist[r];
      dlist[r] = dlist[0];
      iperm[r] = iperm[0];
      r = r - 1;
    }
    else {
      l   = l - 1;
      dRR  = dlist[l - 1];
      RR2  = iperm[l - 1];
      dK   = dlist[l - 1];
    }
  }
  dlist[0] = dRR;
  iperm[0] = RR2; 
}


//  This method requires the solver manager to return a string that describes itself.
template<class ScalarType, class MV, class OP>
std::string GCRODRSolMgr<ScalarType,MV,OP>::description() const {
  std::ostringstream oss;
  oss << "Belos::GCRODRSolMgr<...,"<<Teuchos::ScalarTraits<ScalarType>::name()<<">";
  oss << "{";
  oss << "Ortho Type='"<<orthoType_;
  oss << ", Num Blocks=" <<numBlocks_;
  oss << ", Num Recycle Blocks=" << recycledBlocks_;
  oss << ", Max Restarts=" << maxRestarts_;
  oss << "}";
  return oss.str();
}
  
} // end Belos namespace

#endif /* BELOS_GCRODR_SOLMGR_HPP */
