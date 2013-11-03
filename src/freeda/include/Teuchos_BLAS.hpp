// @HEADER
// ***********************************************************************
// 
//                    Teuchos: Common Tools Package
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

// Kris
// 06.16.03 -- Start over from scratch
// 06.16.03 -- Initial templatization (Tpetra_BLAS.cpp is no longer needed)
// 06.18.03 -- Changed xxxxx_() function calls to XXXXX_F77()
//          -- Added warning messages for generic calls
// 07.08.03 -- Move into Teuchos package/namespace
// 07.24.03 -- The first iteration of BLAS generics is nearing completion. Caveats:
//             * TRSM isn't finished yet; it works for one case at the moment (left side, upper tri., no transpose, no unit diag.)
//             * Many of the generic implementations are quite inefficient, ugly, or both. I wrote these to be easy to debug, not for efficiency or legibility. The next iteration will improve both of these aspects as much as possible.
//             * Very little verification of input parameters is done, save for the character-type arguments (TRANS, etc.) which is quite robust.
//             * All of the routines that make use of both an incx and incy parameter (which includes much of the L1 BLAS) are set up to work iff incx == incy && incx > 0. Allowing for differing/negative values of incx/incy should be relatively trivial.
//             * All of the L2/L3 routines assume that the entire matrix is being used (that is, if A is mxn, lda = m); they don't work on submatrices yet. This *should* be a reasonably trivial thing to fix, as well.
//          -- Removed warning messages for generic calls
// 08.08.03 -- TRSM now works for all cases where SIDE == L and DIAG == N. DIAG == U is implemented but does not work correctly; SIDE == R is not yet implemented.
// 08.14.03 -- TRSM now works for all cases and accepts (and uses) leading-dimension information.
// 09.26.03 -- character input replaced with enumerated input to cause compiling errors and not run-time errors ( suggested by RAB ).

#ifndef _TEUCHOS_BLAS_HPP_
#define _TEUCHOS_BLAS_HPP_

/*! \file Teuchos_BLAS.hpp
    \brief Templated interface class to BLAS routines.
*/
/** \example BLAS/cxx_main.cpp
    This is an example of how to use the Teuchos::BLAS class.
*/

/* for INTEL_CXML, the second arg may need to be changed to 'one'.  If so
the appropriate declaration of one will need to be added back into
functions that include the macro:
*/
#if defined (INTEL_CXML)
        unsigned int one=1;
#endif

#ifdef CHAR_MACRO
#undef CHAR_MACRO
#endif
#if defined (INTEL_CXML)
#define CHAR_MACRO(char_var) &char_var, one
#else
#define CHAR_MACRO(char_var) &char_var
#endif

#include "Teuchos_ConfigDefs.hpp"
#include "Teuchos_ScalarTraits.hpp"
#include "Teuchos_OrdinalTraits.hpp"
#include "Teuchos_BLAS_types.hpp"
#include "Teuchos_TestForException.hpp"

/*! \class Teuchos::BLAS
    \brief The Templated BLAS Wrapper Class.

    The Teuchos::BLAS class provides functionality similar to the BLAS
    (Basic Linear Algebra Subprograms).  The BLAS provide portable, high-
    performance implementations of kernels such as dense std::vector multiplication,
    dot products, dense matrix-std::vector multiplication and dense matrix-matrix
    multiplication.

    The standard BLAS interface is Fortran-specific.  Unfortunately, the 
    interface between C++ and Fortran is not standard across all computer
    platforms.  The Teuchos_BLAS class provides C++ bindings for the BLAS
    kernels in order to insulate the rest of Petra from the details of 
    C++ to Fortran translation.

    In addition to giving access the standard BLAS functionality.
    Teuchos::BLAS also provide functionality for any <ScalarType> class that
    defines the +, - * and / operators.

    Teuchos::BLAS is a single memory image interface only.  This is appropriate 
    since the standard BLAS are only specified for serial execution 
    (or shared memory parallel).

    \note
    <ol>
            <li>These templates are specialized to use the Fortran BLAS routines for
            scalar types \c float and \c double.

            <li>If Teuchos is configured with \c --enable-teuchos-std::complex then these templates
            are specialized for scalar types \c std::complex<float> and \c std::complex<double> also.
    </ol>
*/

namespace Teuchos
{
  extern TEUCHOS_LIB_DLL_EXPORT const char ESideChar[];
  extern TEUCHOS_LIB_DLL_EXPORT const char ETranspChar[];
  extern TEUCHOS_LIB_DLL_EXPORT const char EUploChar[];
  extern TEUCHOS_LIB_DLL_EXPORT const char EDiagChar[];

  //! Default implementation for BLAS routines
  /*!
   * This class provides the default implementation for the BLAS routines.  It
   * is put in a separate class so that specializations of BLAS for other types
   * still have this implementation available.
   */
  template<typename OrdinalType, typename ScalarType>
  class DefaultBLASImpl
  {    

    typedef typename Teuchos::ScalarTraits<ScalarType>::magnitudeType MagnitudeType;
    
  public:
    //! @name Constructor/Destructor.
    //@{ 
    
    //! Default constructor.
    inline DefaultBLASImpl(void) {}

    //! Copy constructor.
    inline DefaultBLASImpl(const DefaultBLASImpl<OrdinalType, ScalarType>& /*BLAS_source*/) {}

    //! Destructor.
    inline virtual ~DefaultBLASImpl(void) {}
    //@}

    //! @name Level 1 BLAS Routines.
    //@{ 

    //! Computes a Givens plane rotation.
    void ROTG(ScalarType* da, ScalarType* db, MagnitudeType* c, ScalarType* s) const;

    //! Applies a Givens plane rotation.
    void ROT(const OrdinalType n, ScalarType* dx, const OrdinalType incx, ScalarType* dy, const OrdinalType incy, MagnitudeType* c, ScalarType* s) const;

    //! Scale the std::vector \c x by the constant \c alpha.
    void SCAL(const OrdinalType n, const ScalarType alpha, ScalarType* x, const OrdinalType incx) const;

    //! Copy the std::vector \c x to the std::vector \c y.
    void COPY(const OrdinalType n, const ScalarType* x, const OrdinalType incx, ScalarType* y, const OrdinalType incy) const;

    //! Perform the operation: \c y \c <- \c y+alpha*x.
    template <typename alpha_type, typename x_type>
    void AXPY(const OrdinalType n, const alpha_type alpha, const x_type* x, const OrdinalType incx, ScalarType* y, const OrdinalType incy) const;

    //! Sum the absolute values of the entries of \c x.
    typename ScalarTraits<ScalarType>::magnitudeType ASUM(const OrdinalType n, const ScalarType* x, const OrdinalType incx) const;

    //! Form the dot product of the vectors \c x and \c y.
    template <typename x_type, typename y_type>
    ScalarType DOT(const OrdinalType n, const x_type* x, const OrdinalType incx, const y_type* y, const OrdinalType incy) const;

    //! Compute the 2-norm of the std::vector \c x.
    typename ScalarTraits<ScalarType>::magnitudeType NRM2(const OrdinalType n, const ScalarType* x, const OrdinalType incx) const;

    //! Return the index of the element of \c x with the maximum magnitude.
    OrdinalType IAMAX(const OrdinalType n, const ScalarType* x, const OrdinalType incx) const;

    //@}

    //! @name Level 2 BLAS Routines.
    //@{ 

    //! Performs the matrix-std::vector operation:  \c y \c <- \c alpha*A*x+beta*y or \c y \c <- \c alpha*A'*x+beta*y where \c A is a general \c m by \c n matrix.
    template <typename alpha_type, typename A_type, typename x_type, typename beta_type>
    void GEMV(ETransp trans, const OrdinalType m, const OrdinalType n, const alpha_type alpha, const A_type* A, 
	      const OrdinalType lda, const x_type* x, const OrdinalType incx, const beta_type beta, ScalarType* y, const OrdinalType incy) const;

    //! Performs the matrix-std::vector operation:  \c x \c <- \c A*x or \c x \c <- \c A'*x where \c A is a unit/non-unit \c n by \c n upper/lower triangular matrix.
    template <typename A_type>
    void TRMV(EUplo uplo, ETransp trans, EDiag diag, const OrdinalType n, const A_type* A, 
	      const OrdinalType lda, ScalarType* x, const OrdinalType incx) const;

    //! \brief Performs the rank 1 operation:  \c A \c <- \c alpha*x*y'+A. 
    template <typename alpha_type, typename x_type, typename y_type>
    void GER(const OrdinalType m, const OrdinalType n, const alpha_type alpha, const x_type* x, const OrdinalType incx, 
	     const y_type* y, const OrdinalType incy, ScalarType* A, const OrdinalType lda) const;
    //@}
    
    //! @name Level 3 BLAS Routines. 
    //@{ 

    //! Performs the matrix-matrix operation: \c C \c <- \c alpha*op(A)*op(B)+beta*C where \c op(A) is either \c A or \c A', \c op(B) is either \c B or \c B', and C is an \c m by \c k matrix.
    template <typename alpha_type, typename A_type, typename B_type, typename beta_type>
    void GEMM(ETransp transa, ETransp transb, const OrdinalType m, const OrdinalType n, const OrdinalType k, const alpha_type alpha, const A_type* A, const OrdinalType lda, const B_type* B, const OrdinalType ldb, const beta_type beta, ScalarType* C, const OrdinalType ldc) const;

    //! Performs the matrix-matrix operation: \c C \c <- \c alpha*A*B+beta*C or \c C \c <- \c alpha*B*A+beta*C where \c A is an \c m by \c m or \c n by \c n symmetric matrix and \c B is a general matrix.
    template <typename alpha_type, typename A_type, typename B_type, typename beta_type>
    void SYMM(ESide side, EUplo uplo, const OrdinalType m, const OrdinalType n, const alpha_type alpha, const A_type* A, const OrdinalType lda, const B_type* B, const OrdinalType ldb, const beta_type beta, ScalarType* C, const OrdinalType ldc) const;

    //! Performs the matrix-matrix operation: \c C \c <- \c alpha*op(A)*B+beta*C or \c C \c <- \c alpha*B*op(A)+beta*C where \c op(A) is an unit/non-unit, upper/lower triangular matrix and \c B is a general matrix.
    template <typename alpha_type, typename A_type>
    void TRMM(ESide side, EUplo uplo, ETransp transa, EDiag diag, const OrdinalType m, const OrdinalType n,
                const alpha_type alpha, const A_type* A, const OrdinalType lda, ScalarType* B, const OrdinalType ldb) const;

    //! Solves the matrix equations:  \c op(A)*X=alpha*B or \c X*op(A)=alpha*B where \c X and \c B are \c m by \c n matrices, \c A is a unit/non-unit, upper/lower triangular matrix and \c op(A) is \c A or \c A'.  The matrix \c X is overwritten on \c B.
    template <typename alpha_type, typename A_type>
    void TRSM(ESide side, EUplo uplo, ETransp transa, EDiag diag, const OrdinalType m, const OrdinalType n,
                const alpha_type alpha, const A_type* A, const OrdinalType lda, ScalarType* B, const OrdinalType ldb) const;
    //@}
  };

  template<typename OrdinalType, typename ScalarType>
  class TEUCHOS_LIB_DLL_EXPORT BLAS : public DefaultBLASImpl<OrdinalType,ScalarType>
  {    

    typedef typename Teuchos::ScalarTraits<ScalarType>::magnitudeType MagnitudeType;
    
  public:
    //! @name Constructor/Destructor.
    //@{ 
    
    //! Default constructor.
    inline BLAS(void) {}

    //! Copy constructor.
    inline BLAS(const BLAS<OrdinalType, ScalarType>& /*BLAS_source*/) {}

    //! Destructor.
    inline virtual ~BLAS(void) {}
    //@}
  };

//------------------------------------------------------------------------------------------
//      LEVEL 1 BLAS ROUTINES  
//------------------------------------------------------------------------------------------
    
  template<typename OrdinalType, typename ScalarType>
  void DefaultBLASImpl<OrdinalType, ScalarType>::ROTG(ScalarType* da, ScalarType* db, MagnitudeType* c, ScalarType* s) const
  {
    ScalarType roe, alpha;
    ScalarType zero = ScalarTraits<ScalarType>::zero();
    ScalarType one = ScalarTraits<ScalarType>::one();
    MagnitudeType scale, norm;
    MagnitudeType m_one = ScalarTraits<MagnitudeType>::one();
    MagnitudeType m_zero = ScalarTraits<MagnitudeType>::zero();

    roe = *db;
    if ( ScalarTraits<ScalarType>::magnitude( *da ) > ScalarTraits<ScalarType>::magnitude( *db ) ) { roe = *da; }
    scale = ScalarTraits<ScalarType>::magnitude( *da ) + ScalarTraits<ScalarType>::magnitude( *db );
    if ( scale == m_zero ) // There is nothing to do.
    {
      *c = m_one;
      *s = zero;
      *da = zero; *db = zero;
    } 
    else if ( *da == zero ) // Still nothing to do.
    { 
      *c = m_zero;
      *s = one;
      *da = *db; *db = zero;
    } 
    else 
    { // Compute the Givens rotation.
      norm = scale*ScalarTraits<ScalarType>::magnitude(ScalarTraits<ScalarType>::squareroot( ( *da/scale)*(*da/scale) + (*db/scale)*(*db/scale) ) );
      alpha = roe / ScalarTraits<ScalarType>::magnitude(roe);
      *c = ScalarTraits<ScalarType>::magnitude(*da) / norm;
      *s = alpha * ScalarTraits<ScalarType>::conjugate(*db) / norm;
      *db = one;
      if( ScalarTraits<ScalarType>::magnitude( *da ) > ScalarTraits<ScalarType>::magnitude( *db ) ){ *db = *s; }
      if( ScalarTraits<ScalarType>::magnitude( *db ) >= ScalarTraits<ScalarType>::magnitude( *da ) &&
	   *c != ScalarTraits<MagnitudeType>::zero() ) { *db = one / *c; }
      *da = norm * alpha;
    }
  } /* end ROTG */
      
  template<typename OrdinalType, typename ScalarType>
  void DefaultBLASImpl<OrdinalType,ScalarType>::ROT(const OrdinalType n, ScalarType* dx, const OrdinalType incx, ScalarType* dy, const OrdinalType incy, MagnitudeType* c, ScalarType* s) const
  {
    ScalarType sconj = Teuchos::ScalarTraits<ScalarType>::conjugate(*s);
    if (n <= 0) return;
    if (incx==1 && incy==1) {
      for(OrdinalType i=0; i<n; ++i) {
        ScalarType temp = *c*dx[i] + sconj*dy[i];
        dy[i] = *c*dy[i] - sconj*dx[i];
        dx[i] = temp;
      }
    }
    else {
      OrdinalType ix = 0, iy = 0;
      if (incx < 0) ix = (-n+1)*incx;
      if (incy < 0) iy = (-n+1)*incy;
      for(OrdinalType i=0; i<n; ++i) {
        ScalarType temp = *c*dx[ix] + sconj*dy[iy];
        dy[iy] = *c*dy[iy] - sconj*dx[ix];
        dx[ix] = temp;
        ix += incx;
        iy += incy;
      }
    }
  }

  template<typename OrdinalType, typename ScalarType>
  void DefaultBLASImpl<OrdinalType, ScalarType>::SCAL(const OrdinalType n, const ScalarType alpha, ScalarType* x, const OrdinalType incx) const
  {
    OrdinalType izero = OrdinalTraits<OrdinalType>::zero();
    OrdinalType ione = OrdinalTraits<OrdinalType>::one();
    OrdinalType i, ix = izero;
    if ( n > izero ) {
        // Set the initial index (ix).
        if (incx < izero) { ix = (-n+ione)*incx; } 
        // Scale the std::vector.
        for(i = izero; i < n; i++)
        {
            x[ix] *= alpha;
            ix += incx;
        }
    }
  } /* end SCAL */

  template<typename OrdinalType, typename ScalarType>
  void DefaultBLASImpl<OrdinalType, ScalarType>::COPY(const OrdinalType n, const ScalarType* x, const OrdinalType incx, ScalarType* y, const OrdinalType incy) const
  {
    OrdinalType izero = OrdinalTraits<OrdinalType>::zero();
    OrdinalType ione = OrdinalTraits<OrdinalType>::one();
    OrdinalType i, ix = izero, iy = izero;
    if ( n > izero ) {
	// Set the initial indices (ix, iy).
    	if (incx < izero) { ix = (-n+ione)*incx; }
    	if (incy < izero) { iy = (-n+ione)*incy; }

        for(i = izero; i < n; i++)
          {
	    y[iy] = x[ix];
	    ix += incx;
	    iy += incy;
          }
      }
  } /* end COPY */

  template<typename OrdinalType, typename ScalarType>
  template <typename alpha_type, typename x_type>
  void DefaultBLASImpl<OrdinalType, ScalarType>::AXPY(const OrdinalType n, const alpha_type alpha, const x_type* x, const OrdinalType incx, ScalarType* y, const OrdinalType incy) const
  {
    OrdinalType izero = OrdinalTraits<OrdinalType>::zero();
    OrdinalType ione = OrdinalTraits<OrdinalType>::one();
    OrdinalType i, ix = izero, iy = izero;
    if( n > izero && alpha != ScalarTraits<alpha_type>::zero())
      {
	// Set the initial indices (ix, iy).
    	if (incx < izero) { ix = (-n+ione)*incx; }
    	if (incy < izero) { iy = (-n+ione)*incy; }

        for(i = izero; i < n; i++)
          {
	    y[iy] += alpha * x[ix];
	    ix += incx;
	    iy += incy;
          }
      }
  } /* end AXPY */

  template<typename OrdinalType, typename ScalarType>
  typename ScalarTraits<ScalarType>::magnitudeType DefaultBLASImpl<OrdinalType, ScalarType>::ASUM(const OrdinalType n, const ScalarType* x, const OrdinalType incx) const
  {
    OrdinalType izero = OrdinalTraits<OrdinalType>::zero();
    OrdinalType ione = OrdinalTraits<OrdinalType>::one();
    typename ScalarTraits<ScalarType>::magnitudeType result = 
      ScalarTraits<typename ScalarTraits<ScalarType>::magnitudeType>::zero();
    OrdinalType i, ix = izero;
    if( n > izero ) {
	// Set the initial indices
	if (incx < izero) { ix = (-n+ione)*incx; }

    	for(i = izero; i < n; i++)
          {
	    result += ScalarTraits<ScalarType>::magnitude(x[ix]);
	    ix += incx;
          }
    } 
   return result;
  } /* end ASUM */
  
  template<typename OrdinalType, typename ScalarType>
  template <typename x_type, typename y_type>
  ScalarType DefaultBLASImpl<OrdinalType, ScalarType>::DOT(const OrdinalType n, const x_type* x, const OrdinalType incx, const y_type* y, const OrdinalType incy) const
  {
    OrdinalType izero = OrdinalTraits<OrdinalType>::zero();
    OrdinalType ione = OrdinalTraits<OrdinalType>::one();
    ScalarType result = ScalarTraits<ScalarType>::zero();
    OrdinalType i, ix = izero, iy = izero;
    if( n > izero ) 
      {
	// Set the initial indices (ix,iy). 	    
	if (incx < izero) { ix = (-n+ione)*incx; }
	if (incy < izero) { iy = (-n+ione)*incy; }

	for(i = izero; i < n; i++)
	  {
	    result += ScalarTraits<x_type>::conjugate(x[ix]) * y[iy];
	    ix += incx;
	    iy += incy;
	  }
      }
    return result;
  } /* end DOT */
  
  template<typename OrdinalType, typename ScalarType>
  typename ScalarTraits<ScalarType>::magnitudeType DefaultBLASImpl<OrdinalType, ScalarType>::NRM2(const OrdinalType n, const ScalarType* x, const OrdinalType incx) const
  {
    OrdinalType izero = OrdinalTraits<OrdinalType>::zero();
    OrdinalType ione = OrdinalTraits<OrdinalType>::one();
    typename ScalarTraits<ScalarType>::magnitudeType result = 
      ScalarTraits<typename ScalarTraits<ScalarType>::magnitudeType>::zero();
    OrdinalType i, ix = izero;
    if ( n > izero ) 
      {
	// Set the initial index.
	if (incx < izero) { ix = (-n+ione)*incx; }	
    
	for(i = izero; i < n; i++)
      	  {
	    result += ScalarTraits<ScalarType>::magnitude(ScalarTraits<ScalarType>::conjugate(x[ix]) * x[ix]);
	    ix += incx;
       	  }
    	result = ScalarTraits<typename ScalarTraits<ScalarType>::magnitudeType>::squareroot(result);
      }	
    return result;
  } /* end NRM2 */
  
  template<typename OrdinalType, typename ScalarType>
  OrdinalType DefaultBLASImpl<OrdinalType, ScalarType>::IAMAX(const OrdinalType n, const ScalarType* x, const OrdinalType incx) const
  {
    OrdinalType izero = OrdinalTraits<OrdinalType>::zero();
    OrdinalType ione = OrdinalTraits<OrdinalType>::one();
    OrdinalType result = izero, ix = izero, i;
    typename ScalarTraits<ScalarType>::magnitudeType maxval =
      ScalarTraits<typename ScalarTraits<ScalarType>::magnitudeType>::zero();

    if ( n > izero ) 
      {
	if (incx < izero) { ix = (-n+ione)*incx; }
	maxval = ScalarTraits<ScalarType>::magnitude(x[ix]);
	ix += incx;
    	for(i = ione; i < n; i++)
      	  {
	    if(ScalarTraits<ScalarType>::magnitude(x[ix]) > maxval)
	      {
	    	result = i;
	        maxval = ScalarTraits<ScalarType>::magnitude(x[ix]);
	      }
	    ix += incx;
	  }
      }
    return result + 1; // the BLAS I?AMAX functions return 1-indexed (Fortran-style) values
  } /* end IAMAX */

//------------------------------------------------------------------------------------------
//      LEVEL 2 BLAS ROUTINES
//------------------------------------------------------------------------------------------
  template<typename OrdinalType, typename ScalarType>
  template <typename alpha_type, typename A_type, typename x_type, typename beta_type>
  void DefaultBLASImpl<OrdinalType, ScalarType>::GEMV(ETransp trans, const OrdinalType m, const OrdinalType n, const alpha_type alpha, const A_type* A, const OrdinalType lda, const x_type* x, const OrdinalType incx, const beta_type beta, ScalarType* y, const OrdinalType incy) const
  {
    OrdinalType izero = OrdinalTraits<OrdinalType>::zero();
    OrdinalType ione = OrdinalTraits<OrdinalType>::one();
    alpha_type alpha_zero = ScalarTraits<alpha_type>::zero();
    beta_type beta_zero = ScalarTraits<beta_type>::zero();
    x_type x_zero = ScalarTraits<x_type>::zero();
    ScalarType y_zero = ScalarTraits<ScalarType>::zero();
    beta_type beta_one = ScalarTraits<beta_type>::one();
    bool BadArgument = false;

    TEST_FOR_EXCEPTION(Teuchos::ScalarTraits<ScalarType>::isComplex && trans == CONJ_TRANS, std::logic_error,
        "Teuchos::BLAS::GEMV() does not currently support CONJ_TRANS for complex data types.");

    // Quick return if there is nothing to do!
    if( m == izero || n == izero || ( alpha == alpha_zero && beta == beta_one ) ){ return; }
    
    // Otherwise, we need to check the argument list.
    if( m < izero ) { 
	std::cout << "BLAS::GEMV Error: M == " << m << std::endl;	    
	BadArgument = true;
    }
    if( n < izero ) { 
	std::cout << "BLAS::GEMV Error: N == " << n << std::endl;	    
	BadArgument = true;
    }
    if( lda < m ) { 
	std::cout << "BLAS::GEMV Error: LDA < MAX(1,M)"<< std::endl;	    
	BadArgument = true;
    }
    if( incx == izero ) {
	std::cout << "BLAS::GEMV Error: INCX == 0"<< std::endl;
	BadArgument = true;
    }
    if( incy == izero ) {
	std::cout << "BLAS::GEMV Error: INCY == 0"<< std::endl;
	BadArgument = true;
    }

    if(!BadArgument) {
      OrdinalType i, j, lenx, leny, ix, iy, jx, jy; 
      OrdinalType kx = izero, ky = izero;
      ScalarType temp;

      // Determine the lengths of the vectors x and y.
      if(ETranspChar[trans] == 'N') {
	lenx = n;
	leny = m;
      } else {
	lenx = m;
	leny = n;
      }

      // Set the starting pointers for the vectors x and y if incx/y < 0.
      if (incx < izero ) { kx =  (ione - lenx)*incx; }
      if (incy < izero ) { ky =  (ione - leny)*incy; }

      // Form y = beta*y
      ix = kx; iy = ky;
      if(beta != beta_one) {
	if (incy == ione) {
	  if (beta == beta_zero) {
	    for(i = izero; i < leny; i++) { y[i] = y_zero; }
	  } else {
	    for(i = izero; i < leny; i++) { y[i] *= beta; }
	  }
	} else {
	  if (beta == beta_zero) {
	    for(i = izero; i < leny; i++) {
	      y[iy] = y_zero;
	      iy += incy;
	    }
	  } else {
	    for(i = izero; i < leny; i++) {
	      y[iy] *= beta;
	      iy += incy;
	    }
	  }
	}
      }
	
      // Return if we don't have to do anything more.
      if(alpha == alpha_zero) { return; }

      if( ETranspChar[trans] == 'N' ) {
	// Form y = alpha*A*y
	jx = kx;
	if (incy == ione) {
	  for(j = izero; j < n; j++) {
	    if (x[jx] != x_zero) {
	      temp = alpha*x[jx];
	      for(i = izero; i < m; i++) {
		y[i] += temp*A[j*lda + i];
	      }
	    }
	    jx += incx;
	  }
	} else {
	  for(j = izero; j < n; j++) {
	    if (x[jx] != x_zero) {
	      temp = alpha*x[jx];
	      iy = ky;
	      for(i = izero; i < m; i++) {
		y[iy] += temp*A[j*lda + i];
		iy += incy;
	      }
	    }
	    jx += incx;
	  }
	}
      } else {
	jy = ky;
	if (incx == ione) {
	  for(j = izero; j < n; j++) {
	    temp = y_zero;
	    for(i = izero; i < m; i++) {
	      temp += A[j*lda + i]*x[i];
	    }
	    y[jy] += alpha*temp;
	    jy += incy;
	  }
	} else {
	  for(j = izero; j < n; j++) {
	    temp = y_zero;
	    ix = kx;
	    for (i = izero; i < m; i++) {
	      temp += A[j*lda + i]*x[ix];
	      ix += incx;
	    }
	    y[jy] += alpha*temp;
	    jy += incy;
	  }
	}
      }
    } /* if (!BadArgument) */
  } /* end GEMV */

 template<typename OrdinalType, typename ScalarType>
 template <typename A_type>
 void DefaultBLASImpl<OrdinalType, ScalarType>::TRMV(EUplo uplo, ETransp trans, EDiag diag, const OrdinalType n, const A_type* A, const OrdinalType lda, ScalarType* x, const OrdinalType incx) const
  {
    OrdinalType izero = OrdinalTraits<OrdinalType>::zero();
    OrdinalType ione = OrdinalTraits<OrdinalType>::one();
    ScalarType zero = ScalarTraits<ScalarType>::zero();
    bool BadArgument = false;

    TEST_FOR_EXCEPTION(Teuchos::ScalarTraits<ScalarType>::isComplex && trans == CONJ_TRANS, std::logic_error,
	    "Teuchos::BLAS::TRMV() does not currently support CONJ_TRANS for complex data types.");

    // Quick return if there is nothing to do!
    if( n == izero ){ return; }
    
    // Otherwise, we need to check the argument list.
    if( n < izero ) { 
      std::cout << "BLAS::TRMV Error: N == " << n << std::endl;	    
      BadArgument = true;
    }
    if( lda < n ) { 
      std::cout << "BLAS::TRMV Error: LDA < MAX(1,N)"<< std::endl;	    
      BadArgument = true;
    }
    if( incx == izero ) {
      std::cout << "BLAS::TRMV Error: INCX == 0"<< std::endl;
      BadArgument = true;
    }

    if(!BadArgument) {
      OrdinalType i, j, ix, jx, kx = izero;
      ScalarType temp;
      bool NoUnit = (EDiagChar[diag] == 'N');

      // Set the starting pointer for the std::vector x if incx < 0.
      if (incx < izero) { kx = (-n+ione)*incx; }

      // Start the operations for a nontransposed triangular matrix 
      if (ETranspChar[trans] == 'N') {
	/* Compute x = A*x */
	if (EUploChar[uplo] == 'U') {
	  /* A is an upper triangular matrix */
	  if (incx == ione) {
	    for (j=izero; j<n; j++) {
	      if (x[j] != zero) {
		temp = x[j];
		for (i=izero; i<j; i++) {
		  x[i] += temp*A[j*lda + i];
		}
		if (NoUnit) 
		  x[j] *= A[j*lda + j];
	      }
	    }
	  } else {
	    jx = kx;
	    for (j=izero; j<n; j++) {
	      if (x[jx] != zero) {
		temp = x[jx];
		ix = kx;
		for (i=izero; i<j; i++) {
		  x[ix] += temp*A[j*lda + i];
		  ix += incx;
		}
		if (NoUnit)
		  x[jx] *= A[j*lda + j];
	      }
	      jx += incx;
	    }
	  } /* if (incx == ione) */
	} else { /* A is a lower triangular matrix */
	  if (incx == ione) {
	    for (j=n-ione; j>-ione; j--) {
	      if (x[j] != zero) {
		temp = x[j];
		for (i=n-ione; i>j; i--) {
		  x[i] += temp*A[j*lda + i];
		}
		if (NoUnit)
		  x[j] *= A[j*lda + j];
	      }
	    }
	  } else {
	    kx += (n-ione)*incx;
	    jx = kx;
	    for (j=n-ione; j>-ione; j--) {
	      if (x[jx] != zero) {
		temp = x[jx];
		ix = kx;
		for (i=n-ione; i>j; i--) {
		  x[ix] += temp*A[j*lda + i];
		  ix -= incx;
		}
		if (NoUnit) 
		  x[jx] *= A[j*lda + j];
	      }
	      jx -= incx;
	    }
	  }
	} /* if (EUploChar[uplo]=='U') */
      } else { /* A is transposed/conjugated */
	/* Compute x = A'*x */
	if (EUploChar[uplo]=='U') {
	  /* A is an upper triangular matrix */
	  if (incx == ione) {
	    for (j=n-ione; j>-ione; j--) {
	      temp = x[j];
	      if (NoUnit)
		temp *= A[j*lda + j];
	      for (i=j-ione; i>-ione; i--) {
		temp += A[j*lda + i]*x[i];
	      }
	      x[j] = temp;
	    }
	  } else {
	    jx = kx + (n-ione)*incx;
	    for (j=n-ione; j>-ione; j--) {
	      temp = x[jx];
	      ix = jx;
	      if (NoUnit)
		temp *= A[j*lda + j];
	      for (i=j-ione; i>-ione; i--) {
		ix -= incx;
		temp += A[j*lda + i]*x[ix];
	      }
	      x[jx] = temp;
	      jx -= incx;
	    }
	  }
	} else {
	  /* A is a lower triangular matrix */
	  if (incx == ione) {
	    for (j=izero; j<n; j++) {
	      temp = x[j];
	      if (NoUnit)
		temp *= A[j*lda + j];
	      for (i=j+ione; i<n; i++) {
		temp += A[j*lda + i]*x[i];
	      }
	      x[j] = temp;
	    }
	  } else {
	    jx = kx;
	    for (j=izero; j<n; j++) {
	      temp = x[jx];
	      ix = jx;
	      if (NoUnit) 
		temp *= A[j*lda + j];
	      for (i=j+ione; i<n; i++) {
		ix += incx;
		temp += A[j*lda + i]*x[ix];
	      }
	      x[jx] = temp;
	      jx += incx;	      
	    }
	  }
	} /* if (EUploChar[uplo]=='U') */
      } /* if (ETranspChar[trans]=='N') */
    } /* if (!BadArgument) */
  } /* end TRMV */
        
  template<typename OrdinalType, typename ScalarType>
  template <typename alpha_type, typename x_type, typename y_type>
  void DefaultBLASImpl<OrdinalType, ScalarType>::GER(const OrdinalType m, const OrdinalType n, const alpha_type alpha, const x_type* x, const OrdinalType incx, const y_type* y, const OrdinalType incy, ScalarType* A, const OrdinalType lda) const
  {
    OrdinalType izero = OrdinalTraits<OrdinalType>::zero();
    OrdinalType ione = OrdinalTraits<OrdinalType>::one();
    alpha_type alpha_zero = ScalarTraits<alpha_type>::zero();
    y_type y_zero = ScalarTraits<y_type>::zero();
    bool BadArgument = false;

    TEST_FOR_EXCEPTION(Teuchos::ScalarTraits<ScalarType>::isComplex, std::logic_error,
	    "Teuchos::BLAS::GER() does not currently support complex data types.");

    // Quick return if there is nothing to do!
    if( m == izero || n == izero || alpha == alpha_zero ){ return; }
    
    // Otherwise, we need to check the argument list.
    if( m < izero ) { 
	std::cout << "BLAS::GER Error: M == " << m << std::endl;	    
	BadArgument = true;
    }
    if( n < izero ) { 
	std::cout << "BLAS::GER Error: N == " << n << std::endl;	    
	BadArgument = true;
    }
    if( lda < m ) { 
	std::cout << "BLAS::GER Error: LDA < MAX(1,M)"<< std::endl;	    
	BadArgument = true;
    }
    if( incx == 0 ) {
	std::cout << "BLAS::GER Error: INCX == 0"<< std::endl;
	BadArgument = true;
    }
    if( incy == 0 ) {
	std::cout << "BLAS::GER Error: INCY == 0"<< std::endl;
	BadArgument = true;
    }

    if(!BadArgument) {
      OrdinalType i, j, ix, jy = izero, kx = izero;
      ScalarType temp;

      // Set the starting pointers for the vectors x and y if incx/y < 0.
      if (incx < izero) { kx = (-m+ione)*incx; }
      if (incy < izero) { jy = (-n+ione)*incy; }

      // Start the operations for incx == 1
      if( incx == ione ) {
	for( j=izero; j<n; j++ ) {
	  if ( y[jy] != y_zero ) {
	    temp = alpha*y[jy];
	    for ( i=izero; i<m; i++ ) {
	      A[j*lda + i] += x[i]*temp;
	    }
	  }
	  jy += incy;
	}
      } 
      else { // Start the operations for incx != 1
	for( j=izero; j<n; j++ ) {
	  if ( y[jy] != y_zero ) {
	    temp = alpha*y[jy];
	    ix = kx;
	    for( i=izero; i<m; i++ ) {
	      A[j*lda + i] += x[ix]*temp;
	      ix += incx;
	    }
	  }
	  jy += incy;
	}
      }
    } /* if(!BadArgument) */
  } /* end GER */
  
//------------------------------------------------------------------------------------------
//      LEVEL 3 BLAS ROUTINES
//------------------------------------------------------------------------------------------
        
  template<typename OrdinalType, typename ScalarType>
  template <typename alpha_type, typename A_type, typename B_type, typename beta_type>
  void DefaultBLASImpl<OrdinalType, ScalarType>::GEMM(ETransp transa, ETransp transb, const OrdinalType m, const OrdinalType n, const OrdinalType k, const alpha_type alpha, const A_type* A, const OrdinalType lda, const B_type* B, const OrdinalType ldb, const beta_type beta, ScalarType* C, const OrdinalType ldc) const
  {

    typedef TypeNameTraits<OrdinalType> OTNT;
    typedef TypeNameTraits<ScalarType> STNT;

    OrdinalType izero = OrdinalTraits<OrdinalType>::zero();
    alpha_type alpha_zero = ScalarTraits<alpha_type>::zero();
    beta_type beta_zero = ScalarTraits<beta_type>::zero();
    B_type B_zero = ScalarTraits<B_type>::zero();
    ScalarType C_zero = ScalarTraits<ScalarType>::zero();
    beta_type beta_one = ScalarTraits<beta_type>::one();
    OrdinalType i, j, p;
    OrdinalType NRowA = m, NRowB = k;
    ScalarType temp;
    bool BadArgument = false;

    TEST_FOR_EXCEPTION(
      Teuchos::ScalarTraits<ScalarType>::isComplex
      && (transa == CONJ_TRANS || transb == CONJ_TRANS),
      std::logic_error,
	    "Teuchos::BLAS<"<<OTNT::name()<<","<<STNT::name()<<">::GEMM()"
      " does not currently support CONJ_TRANS for complex data types."
      );

    // Change dimensions of matrix if either matrix is transposed
    if( !(ETranspChar[transa]=='N') ) {
      NRowA = k;
    }
    if( !(ETranspChar[transb]=='N') ) {
      NRowB = n;
    }

    // Quick return if there is nothing to do!
    if( (m==izero) || (n==izero) || (((alpha==alpha_zero)||(k==izero)) && (beta==beta_one)) ){ return; }
    if( m < izero ) { 
      std::cout << "BLAS::GEMM Error: M == " << m << std::endl;	    
      BadArgument = true;
    }
    if( n < izero ) { 
      std::cout << "BLAS::GEMM Error: N == " << n << std::endl;	    
      BadArgument = true;
    }
    if( k < izero ) { 
      std::cout << "BLAS::GEMM Error: K == " << k << std::endl;	    
      BadArgument = true;
    }
    if( lda < NRowA ) { 
      std::cout << "BLAS::GEMM Error: LDA < MAX(1,M)"<< std::endl;	    
      BadArgument = true;
    }
    if( ldb < NRowB ) { 
      std::cout << "BLAS::GEMM Error: LDB < MAX(1,K)"<< std::endl;	    
      BadArgument = true;
    }
     if( ldc < m ) { 
      std::cout << "BLAS::GEMM Error: LDC < MAX(1,M)"<< std::endl;	    
      BadArgument = true;
    }

    if(!BadArgument) {

      // Only need to scale the resulting matrix C.
      if( alpha == alpha_zero ) {
	if( beta == beta_zero ) {
	  for (j=izero; j<n; j++) {
	    for (i=izero; i<m; i++) {
	      C[j*ldc + i] = C_zero;
	    }
	  }
	} else {
	  for (j=izero; j<n; j++) {
	    for (i=izero; i<m; i++) {
	      C[j*ldc + i] *= beta;
	    }
	  }
	}
	return;
      }
      //
      // Now start the operations.
      //
      if ( ETranspChar[transb]=='N' ) {
	if ( ETranspChar[transa]=='N' ) {
	  // Compute C = alpha*A*B + beta*C
	  for (j=izero; j<n; j++) {
	    if( beta == beta_zero ) {
	      for (i=izero; i<m; i++){
		C[j*ldc + i] = C_zero;
	      }
	    } else if( beta != beta_one ) {
	      for (i=izero; i<m; i++){
		C[j*ldc + i] *= beta;
	      }
	    }
	    for (p=izero; p<k; p++){
	      if (B[j*ldb + p] != B_zero ){
		temp = alpha*B[j*ldb + p];
		for (i=izero; i<m; i++) {
		  C[j*ldc + i] += temp*A[p*lda + i];
		}
	      }
	    }
	  }
	} else {
	  // Compute C = alpha*A'*B + beta*C
	  for (j=izero; j<n; j++) {
	    for (i=izero; i<m; i++) {
	      temp = C_zero;
	      for (p=izero; p<k; p++) {
		temp += A[i*lda + p]*B[j*ldb + p];
	      }
	      if (beta == beta_zero) {
		C[j*ldc + i] = alpha*temp;
	      } else {
		C[j*ldc + i] = alpha*temp + beta*C[j*ldc + i];
	      }
	    }
	  }
	}
      } else {
	if ( ETranspChar[transa]=='N' ) {
	  // Compute C = alpha*A*B' + beta*C
	  for (j=izero; j<n; j++) {
	    if (beta == beta_zero) {
	      for (i=izero; i<m; i++) {
		C[j*ldc + i] = C_zero;
	      } 
	    } else if ( beta != beta_one ) {
	      for (i=izero; i<m; i++) {
		C[j*ldc + i] *= beta;
	      }
	    }
	    for (p=izero; p<k; p++) {
	      if (B[p*ldb + j] != B_zero) {
		temp = alpha*B[p*ldb + j];
		for (i=izero; i<m; i++) {
		  C[j*ldc + i] += temp*A[p*lda + i];
		}
	      }
	    }
	  }
	} else {
	  // Compute C += alpha*A'*B' + beta*C
	  for (j=izero; j<n; j++) {
	    for (i=izero; i<m; i++) {
	      temp = C_zero;
	      for (p=izero; p<k; p++) {
		temp += A[i*lda + p]*B[p*ldb + j];
	      }
	      if (beta == beta_zero) {
		C[j*ldc + i] = alpha*temp;
	      } else {
		C[j*ldc + i] = alpha*temp + beta*C[j*ldc + i];
	      }
	    }
	  }
	} // end if (ETranspChar[transa]=='N') ...
      } // end if (ETranspChar[transb]=='N') ...
    } // end if (!BadArgument) ...
  } // end of GEMM


  template<typename OrdinalType, typename ScalarType>
  template <typename alpha_type, typename A_type, typename B_type, typename beta_type>
  void DefaultBLASImpl<OrdinalType, ScalarType>::SYMM(ESide side, EUplo uplo, const OrdinalType m, const OrdinalType n, const alpha_type alpha, const A_type* A, const OrdinalType lda, const B_type* B, const OrdinalType ldb, const beta_type beta, ScalarType* C, const OrdinalType ldc) const
  {
    OrdinalType izero = OrdinalTraits<OrdinalType>::zero();
    OrdinalType ione = OrdinalTraits<OrdinalType>::one();
    alpha_type alpha_zero = ScalarTraits<alpha_type>::zero();
    beta_type beta_zero = ScalarTraits<beta_type>::zero();
    ScalarType C_zero = ScalarTraits<ScalarType>::zero();
    beta_type beta_one = ScalarTraits<beta_type>::one();
    OrdinalType i, j, k, NRowA = m;
    ScalarType temp1, temp2;
    bool BadArgument = false;
    bool Upper = (EUploChar[uplo] == 'U');
    if (ESideChar[side] == 'R') { NRowA = n; }

    TEST_FOR_EXCEPTION(Teuchos::ScalarTraits<ScalarType>::isComplex, std::logic_error,
	    "Teuchos::BLAS::SYMM() does not currently support complex data types.");
    
    // Quick return.
    if ( (m==izero) || (n==izero) || ( (alpha==alpha_zero)&&(beta==beta_one) ) ) { return; }
    if( m < 0 ) { 
      std::cout << "BLAS::SYMM Error: M == "<< m << std::endl;
      BadArgument = true; }
    if( n < 0 ) {
      std::cout << "BLAS::SYMM Error: N == "<< n << std::endl;
      BadArgument = true; }
    if( lda < NRowA ) {
      std::cout << "BLAS::SYMM Error: LDA == "<<lda<<std::endl;
      BadArgument = true; }
    if( ldb < m ) {
      std::cout << "BLAS::SYMM Error: LDB == "<<ldb<<std::endl;
      BadArgument = true; }
    if( ldc < m ) {
      std::cout << "BLAS::SYMM Error: LDC == "<<ldc<<std::endl;
      BadArgument = true; }

    if(!BadArgument) {

      // Only need to scale C and return.
      if (alpha == alpha_zero) {
	if (beta == beta_zero ) {
	  for (j=izero; j<n; j++) {
	    for (i=izero; i<m; i++) {
	      C[j*ldc + i] = C_zero;
	    }
	  }
	} else {
	  for (j=izero; j<n; j++) {
	    for (i=izero; i<m; i++) {
	      C[j*ldc + i] *= beta;
	    }
	  }
	}
	return;
      }

      if ( ESideChar[side] == 'L') {
	// Compute C = alpha*A*B + beta*C

	if (Upper) {
	  // The symmetric part of A is stored in the upper triangular part of the matrix.
	  for (j=izero; j<n; j++) {
	    for (i=izero; i<m; i++) {
	      temp1 = alpha*B[j*ldb + i];
	      temp2 = C_zero;
	      for (k=izero; k<i; k++) {
		C[j*ldc + k] += temp1*A[i*lda + k];
		temp2 += B[j*ldb + k]*A[i*lda + k];
	      }
	      if (beta == beta_zero) {
		C[j*ldc + i] = temp1*A[i*lda + i] + alpha*temp2;
	      } else {
		C[j*ldc + i] = beta*C[j*ldc + i] + temp1*A[i*lda + i] + alpha*temp2;
	      }
	    }
	  }
	} else {
	  // The symmetric part of A is stored in the lower triangular part of the matrix.
	  for (j=izero; j<n; j++) {
	    for (i=m-ione; i>-ione; i--) {
	      temp1 = alpha*B[j*ldb + i];
	      temp2 = C_zero;
	      for (k=i+ione; k<m; k++) {
		C[j*ldc + k] += temp1*A[i*lda + k];
		temp2 += B[j*ldb + k]*A[i*lda + k];
	      }
	      if (beta == beta_zero) {
		C[j*ldc + i] = temp1*A[i*lda + i] + alpha*temp2;
	      } else {
		C[j*ldc + i] = beta*C[j*ldc + i] + temp1*A[i*lda + i] + alpha*temp2;
	      }
	    }
	  }
	}
      } else {
	// Compute C = alpha*B*A + beta*C.
	for (j=izero; j<n; j++) {
	  temp1 = alpha*A[j*lda + j];
	  if (beta == beta_zero) {
	    for (i=izero; i<m; i++) {
	      C[j*ldc + i] = temp1*B[j*ldb + i];
	    }
	  } else {
	    for (i=izero; i<m; i++) {
	      C[j*ldc + i] = beta*C[j*ldc + i] + temp1*B[j*ldb + i];
	    }
	  }
	  for (k=izero; k<j; k++) {
	    if (Upper) {
	      temp1 = alpha*A[j*lda + k];
	    } else {
	      temp1 = alpha*A[k*lda + j];
	    }
	    for (i=izero; i<m; i++) {
	      C[j*ldc + i] += temp1*B[k*ldb + i];
	    }
	  }
	  for (k=j+ione; k<n; k++) {
	    if (Upper) {
	      temp1 = alpha*A[k*lda + j];
	    } else {
	      temp1 = alpha*A[j*lda + k];
	    }
	    for (i=izero; i<m; i++) {
	      C[j*ldc + i] += temp1*B[k*ldb + i];
	    }
	  }
	}
      } // end if (ESideChar[side]=='L') ...
    } // end if(!BadArgument) ...
} // end SYMM
  
  template<typename OrdinalType, typename ScalarType>
  template <typename alpha_type, typename A_type>
  void DefaultBLASImpl<OrdinalType, ScalarType>::TRMM(ESide side, EUplo uplo, ETransp transa, EDiag diag, const OrdinalType m, const OrdinalType n, const alpha_type alpha, const A_type* A, const OrdinalType lda, ScalarType* B, const OrdinalType ldb) const
  {
    OrdinalType izero = OrdinalTraits<OrdinalType>::zero();
    OrdinalType ione = OrdinalTraits<OrdinalType>::one();
    alpha_type alpha_zero = ScalarTraits<alpha_type>::zero();
    A_type A_zero = ScalarTraits<A_type>::zero();
    ScalarType B_zero = ScalarTraits<ScalarType>::zero();
    ScalarType one = ScalarTraits<ScalarType>::one();
    OrdinalType i, j, k, NRowA = m;
    ScalarType temp;
    bool BadArgument = false;
    bool LSide = (ESideChar[side] == 'L');
    bool NoUnit = (EDiagChar[diag] == 'N');
    bool Upper = (EUploChar[uplo] == 'U');

    TEST_FOR_EXCEPTION(Teuchos::ScalarTraits<ScalarType>::isComplex && transa == CONJ_TRANS, std::logic_error,
	    "Teuchos::BLAS::TRMM() does not currently support CONJ_TRANS for complex data types.");

    if(!LSide) { NRowA = n; }

    // Quick return.
    if (n==izero || m==izero) { return; }
    if( m < 0 ) {
      std::cout << "BLAS::TRMM Error: M == "<< m <<std::endl;
      BadArgument = true; }
    if( n < 0 ) {
      std::cout << "BLAS::TRMM Error: N == "<< n <<std::endl;
      BadArgument = true; }
    if( lda < NRowA ) {
      std::cout << "BLAS::TRMM Error: LDA == "<< lda << std::endl;
      BadArgument = true; }
    if( ldb < m ) {
      std::cout << "BLAS::TRMM Error: M == "<< ldb << std::endl;
      BadArgument = true; }

    if(!BadArgument) {

      // B only needs to be zeroed out.
      if( alpha == alpha_zero ) {
	for( j=izero; j<n; j++ ) {
	  for( i=izero; i<m; i++ ) {
	    B[j*ldb + i] = B_zero;
	  }
	}
	return;
      }
      
      //  Start the computations. 
      if ( LSide ) {
	// A is on the left side of B.
	
	if ( ETranspChar[transa]=='N' ) {
	  // Compute B = alpha*A*B

	  if ( Upper ) {
	    // A is upper triangular
	    for( j=izero; j<n; j++ ) {
	      for( k=izero; k<m; k++) {
		if ( B[j*ldb + k] != B_zero ) {
		  temp = alpha*B[j*ldb + k];
		  for( i=izero; i<k; i++ ) {
		    B[j*ldb + i] += temp*A[k*lda + i];
		  }
		  if ( NoUnit )
		    temp *=A[k*lda + k];
		  B[j*ldb + k] = temp;
		}
	      }
	    }
	  } else {
	    // A is lower triangular
	    for( j=izero; j<n; j++ ) {
	      for( k=m-ione; k>-ione; k-- ) {
		if( B[j*ldb + k] != B_zero ) {
		  temp = alpha*B[j*ldb + k];
		  B[j*ldb + k] = temp;
		  if ( NoUnit )
		    B[j*ldb + k] *= A[k*lda + k];
		  for( i=k+ione; i<m; i++ ) {
		    B[j*ldb + i] += temp*A[k*lda + i];
		  }
		}
	      }
	    }
	  }
	} else {
	  // Compute B = alpha*A'*B
	  if( Upper ) {
	    for( j=izero; j<n; j++ ) {
	      for( i=m-ione; i>-ione; i-- ) {
		temp = B[j*ldb + i];
		if( NoUnit )
		  temp *= A[i*lda + i];
		for( k=izero; k<i; k++ ) {
		  temp += A[i*lda + k]*B[j*ldb + k];
		}
		B[j*ldb + i] = alpha*temp;
	      }
	    }
	  } else {
	    for( j=izero; j<n; j++ ) {
	      for( i=izero; i<m; i++ ) {
		temp = B[j*ldb + i];
		if( NoUnit ) 
		  temp *= A[i*lda + i];
		for( k=i+ione; k<m; k++ ) {
		  temp += A[i*lda + k]*B[j*ldb + k];
		}
		B[j*ldb + i] = alpha*temp;
	      }
	    }
	  }
	}
      } else {
	// A is on the right hand side of B.
	
	if( ETranspChar[transa] == 'N' ) {
	  // Compute B = alpha*B*A

	  if( Upper ) {
	    // A is upper triangular.
	    for( j=n-ione; j>-ione; j-- ) {
	      temp = alpha;
	      if( NoUnit )
		temp *= A[j*lda + j];
	      for( i=izero; i<m; i++ ) {
		B[j*ldb + i] *= temp;
	      }
	      for( k=izero; k<j; k++ ) {
		if( A[j*lda + k] != A_zero ) {
		  temp = alpha*A[j*lda + k];
		  for( i=izero; i<m; i++ ) {
		    B[j*ldb + i] += temp*B[k*ldb + i];
		  }
		}
	      }
	    }
	  } else {
	    // A is lower triangular.
	    for( j=izero; j<n; j++ ) {
	      temp = alpha;
	      if( NoUnit )
		temp *= A[j*lda + j];
	      for( i=izero; i<m; i++ ) {
		B[j*ldb + i] *= temp;
	      }
	      for( k=j+ione; k<n; k++ ) {
		if( A[j*lda + k] != A_zero ) {
		  temp = alpha*A[j*lda + k];
		  for( i=izero; i<m; i++ ) {
		    B[j*ldb + i] += temp*B[k*ldb + i];
		  }
		}
	      }
	    }
	  }
	} else {
	  // Compute B = alpha*B*A'

	  if( Upper ) {
	    for( k=izero; k<n; k++ ) {
	      for( j=izero; j<k; j++ ) {
		if( A[k*lda + j] != A_zero ) {
		  temp = alpha*A[k*lda + j];
		  for( i=izero; i<m; i++ ) {
		    B[j*ldb + i] += temp*B[k*ldb + i];
		  }
		}
	      }
	      temp = alpha;
	      if( NoUnit ) 
		temp *= A[k*lda + k];
	      if( temp != one ) {
		for( i=izero; i<m; i++ ) {
		  B[k*ldb + i] *= temp;
		}
	      }
	    }
	  } else {
	    for( k=n-ione; k>-ione; k-- ) {
	      for( j=k+ione; j<n; j++ ) {
		if( A[k*lda + j] != A_zero ) {
		  temp = alpha*A[k*lda + j];
		  for( i=izero; i<m; i++ ) {
		    B[j*ldb + i] += temp*B[k*ldb + i];
		  }
		}
	      }
	      temp = alpha;
	      if( NoUnit )
		temp *= A[k*lda + k];
	      if( temp != one ) {
		for( i=izero; i<m; i++ ) {
		  B[k*ldb + i] *= temp;
		}
	      }
	    }
	  }
	} // end if( ETranspChar[transa] == 'N' ) ...
      } // end if ( LSide ) ...
    } // end if (!BadArgument)
  } // end TRMM
  
  template<typename OrdinalType, typename ScalarType>
  template <typename alpha_type, typename A_type>
  void DefaultBLASImpl<OrdinalType, ScalarType>::TRSM(ESide side, EUplo uplo, ETransp transa, EDiag diag, const OrdinalType m, const OrdinalType n, const alpha_type alpha, const A_type* A, const OrdinalType lda, ScalarType* B, const OrdinalType ldb) const
  {
    OrdinalType izero = OrdinalTraits<OrdinalType>::zero();
    OrdinalType ione = OrdinalTraits<OrdinalType>::one();
    alpha_type alpha_zero = ScalarTraits<alpha_type>::zero();
    A_type A_zero = ScalarTraits<A_type>::zero();
    ScalarType B_zero = ScalarTraits<ScalarType>::zero();
    alpha_type alpha_one = ScalarTraits<alpha_type>::one();
    ScalarType B_one = ScalarTraits<ScalarType>::one();
    ScalarType temp;
    OrdinalType NRowA = m;
    bool BadArgument = false;
    bool NoUnit = (EDiagChar[diag]=='N');
    
    TEST_FOR_EXCEPTION(Teuchos::ScalarTraits<ScalarType>::isComplex && transa == CONJ_TRANS, std::logic_error,
	    "Teuchos::BLAS::TRSM() does not currently support CONJ_TRANS for complex data types.");

    if (!(ESideChar[side] == 'L')) { NRowA = n; }

    // Quick return.
    if (n == izero || m == izero) { return; }
    if( m < izero ) {
      std::cout << "BLAS::TRSM Error: M == "<<m<<std::endl;
      BadArgument = true; }
    if( n < izero ) {
      std::cout << "BLAS::TRSM Error: N == "<<n<<std::endl;
      BadArgument = true; }
    if( lda < NRowA ) {
      std::cout << "BLAS::TRSM Error: LDA == "<<lda<<std::endl;
      BadArgument = true; }
    if( ldb < m ) {
      std::cout << "BLAS::TRSM Error: LDB == "<<ldb<<std::endl;
      BadArgument = true; }

    if(!BadArgument)
      {
	int i, j, k;
	// Set the solution to the zero std::vector.
	if(alpha == alpha_zero) {
	    for(j = izero; j < n; j++) {
	    	for( i = izero; i < m; i++) {
		    B[j*ldb+i] = B_zero;
	      	}
	    }
	}
	else 
	{ // Start the operations.
	    if(ESideChar[side] == 'L') {
		//
	    	// Perform computations for OP(A)*X = alpha*B	    
		//
		if(ETranspChar[transa] == 'N') {
		    //
		    //  Compute B = alpha*inv( A )*B
		    //
		    if(EUploChar[uplo] == 'U') { 
			// A is upper triangular.
			for(j = izero; j < n; j++) {
	    		    // Perform alpha*B if alpha is not 1.
			  if(alpha != alpha_one) {
	    	    		for( i = izero; i < m; i++) {
		    		    B[j*ldb+i] *= alpha;
		    		}
			    }
			    // Perform a backsolve for column j of B.
			    for(k = (m - ione); k > -ione; k--) {
				// If this entry is zero, we don't have to do anything.
				if (B[j*ldb + k] != B_zero) {
				    if (NoUnit) {
					B[j*ldb + k] /= A[k*lda + k];
				    }
				    for(i = izero; i < k; i++) {
					B[j*ldb + i] -= B[j*ldb + k] * A[k*lda + i];
				    }
				}
			    }
			}
		    }
		    else 
		    { // A is lower triangular.
                        for(j = izero; j < n; j++) {
                            // Perform alpha*B if alpha is not 1.
                            if(alpha != alpha_one) {
                                for( i = izero; i < m; i++) {
                                    B[j*ldb+i] *= alpha;
                                }
                            }
                            // Perform a forward solve for column j of B.
                            for(k = izero; k < m; k++) {
                                // If this entry is zero, we don't have to do anything.
                                if (B[j*ldb + k] != B_zero) {   
                                    if (NoUnit) {
                                        B[j*ldb + k] /= A[k*lda + k];
                                    }
                                    for(i = k+ione; i < m; i++) {
                                        B[j*ldb + i] -= B[j*ldb + k] * A[k*lda + i];
                                    }
                                }
                            }
                        }
		    } // end if (uplo == 'U')
		}  // if (transa =='N')	
	    	else { 
		    //
		    //  Compute B = alpha*inv( A' )*B
		    //
		    if(EUploChar[uplo] == 'U') { 
			// A is upper triangular.
			for(j = izero; j < n; j++) {
	    	    	    for( i = izero; i < m; i++) {
		    		temp = alpha*B[j*ldb+i];
			    	for(k = izero; k < i; k++) {
				    temp -= A[i*lda + k] * B[j*ldb + k];
				}
				if (NoUnit) {
				    temp /= A[i*lda + i];
				}
				B[j*ldb + i] = temp;
			    }
			}
		    }
		    else
		    { // A is lower triangular.
                        for(j = izero; j < n; j++) {
                            for(i = (m - ione) ; i > -ione; i--) {
                                temp = alpha*B[j*ldb+i];
                            	for(k = i+ione; k < m; k++) {
				    temp -= A[i*lda + k] * B[j*ldb + k];
				}
				if (NoUnit) {
				    temp /= A[i*lda + i];
				}
				B[j*ldb + i] = temp;
                            }
                        }
		    }
		}
	    }  // if (side == 'L')
	    else { 
	       // side == 'R'
	       //
	       // Perform computations for X*OP(A) = alpha*B	    
	       //
	      if (ETranspChar[transa] == 'N') {
		    //
		    //  Compute B = alpha*B*inv( A )
		    //
		    if(EUploChar[uplo] == 'U') { 
			// A is upper triangular.
	    		// Perform a backsolve for column j of B.
			for(j = izero; j < n; j++) {
	    		    // Perform alpha*B if alpha is not 1.
	    		    if(alpha != alpha_one) {
	    	    		for( i = izero; i < m; i++) {
		    		    B[j*ldb+i] *= alpha;
		    		}
			    }
			    for(k = izero; k < j; k++) {
				// If this entry is zero, we don't have to do anything.
				if (A[j*lda + k] != A_zero) {
				    for(i = izero; i < m; i++) {
					B[j*ldb + i] -= A[j*lda + k] * B[k*ldb + i];
				    }
				}
			    }
			    if (NoUnit) {
				temp = B_one/A[j*lda + j];
				for(i = izero; i < m; i++) {
				    B[j*ldb + i] *= temp;
				}
			    }
			}
		    }
		    else 
		    { // A is lower triangular.
                        for(j = (n - ione); j > -ione; j--) {
                            // Perform alpha*B if alpha is not 1.
                            if(alpha != alpha_one) {
                                for( i = izero; i < m; i++) {
                                    B[j*ldb+i] *= alpha;
                                }
                            }
                            // Perform a forward solve for column j of B.
                            for(k = j+ione; k < n; k++) {
                                // If this entry is zero, we don't have to do anything.
				if (A[j*lda + k] != A_zero) {
				    for(i = izero; i < m; i++) {
                                        B[j*ldb + i] -= A[j*lda + k] * B[k*ldb + i]; 
                                    }
                                } 
                            }
			    if (NoUnit) {
				temp = B_one/A[j*lda + j];
				for(i = izero; i < m; i++) {
				    B[j*ldb + i] *= temp;
				}
			    }			
                        }
		    } // end if (uplo == 'U')
		}  // if (transa =='N')	
	    	else { 
		    //
		    //  Compute B = alpha*B*inv( A' )
		    //
		    if(EUploChar[uplo] == 'U') { 
			// A is upper triangular.
			for(k = (n - ione); k > -ione; k--) {
			    if (NoUnit) {
				temp = B_one/A[k*lda + k];
	    	    	    	for(i = izero; i < m; i++) {
		    		    B[k*ldb + i] *= temp;
				}
			    }
			    for(j = izero; j < k; j++) {
				if (A[k*lda + j] != A_zero) {
				    temp = A[k*lda + j];
				    for(i = izero; i < m; i++) {
					B[j*ldb + i] -= temp*B[k*ldb + i];
				    }
				}
			    }
			    if (alpha != alpha_one) {
				for (i = izero; i < m; i++) {
				    B[k*ldb + i] *= alpha;
				}
			    }
			}
		    }
		    else
		    { // A is lower triangular.
			for(k = izero; k < n; k++) {
			    if (NoUnit) {
				temp = B_one/A[k*lda + k];
				for (i = izero; i < m; i++) {
				    B[k*ldb + i] *= temp;
				}
			    }
			    for(j = k+ione; j < n; j++) {
				if(A[k*lda + j] != A_zero) {
				    temp = A[k*lda + j];
				    for(i = izero; i < m; i++) {
					B[j*ldb + i] -= temp*B[k*ldb + i];
				    }
				}
			    }
			    if (alpha != alpha_one) {
				for (i = izero; i < m; i++) {
				    B[k*ldb + i] *= alpha;
				}
			    }
                        }
		    }
		}		
	    }
	}
    }
  }

  // Explicit instantiation for template<int,float>

  template <>
  class TEUCHOS_LIB_DLL_EXPORT BLAS<int, float>
  {    
  public:
    inline BLAS(void) {}
    inline BLAS(const BLAS<int, float>& /*BLAS_source*/) {}
    inline virtual ~BLAS(void) {}
    void ROTG(float* da, float* db, float* c, float* s) const;
    void ROT(const int n, float* dx, const int incx, float* dy, const int incy, float* c, float* s) const;
    float ASUM(const int n, const float* x, const int incx) const;
    void AXPY(const int n, const float alpha, const float* x, const int incx, float* y, const int incy) const;
    void COPY(const int n, const float* x, const int incx, float* y, const int incy) const;
    float DOT(const int n, const float* x, const int incx, const float* y, const int incy) const;
    float NRM2(const int n, const float* x, const int incx) const;
    void SCAL(const int n, const float alpha, float* x, const int incx) const;
    int IAMAX(const int n, const float* x, const int incx) const;
    void GEMV(ETransp trans, const int m, const int n, const float alpha, const float* A, const int lda, const float* x, const int incx, const float beta, float* y, const int incy) const;
    void TRMV(EUplo uplo, ETransp trans, EDiag diag, const int n, const float* A, const int lda, float* x, const int incx) const;
    void GER(const int m, const int n, const float alpha, const float* x, const int incx, const float* y, const int incy, float* A, const int lda) const;
    void GEMM(ETransp transa, ETransp transb, const int m, const int n, const int k, const float alpha, const float* A, const int lda, const float* B, const int ldb, const float beta, float* C, const int ldc) const;
    void SYMM(ESide side, EUplo uplo, const int m, const int n, const float alpha, const float* A, const int lda, const float *B, const int ldb, const float beta, float *C, const int ldc) const;
    void TRMM(ESide side, EUplo uplo, ETransp transa, EDiag diag, const int m, const int n, const float alpha, const float* A, const int lda, float* B, const int ldb) const;
    void TRSM(ESide side, EUplo uplo, ETransp transa, EDiag diag, const int m, const int n, const float alpha, const float* A, const int lda, float* B, const int ldb) const;
  };

  // Explicit instantiation for template<int,double>

  template<>
  class TEUCHOS_LIB_DLL_EXPORT BLAS<int, double>
  {    
  public:
    inline BLAS(void) {}
    inline BLAS(const BLAS<int, double>& /*BLAS_source*/) {}
    inline virtual ~BLAS(void) {}
    void ROTG(double* da, double* db, double* c, double* s) const;
    void ROT(const int n, double* dx, const int incx, double* dy, const int incy, double* c, double* s) const;
    double ASUM(const int n, const double* x, const int incx) const;
    void AXPY(const int n, const double alpha, const double* x, const int incx, double* y, const int incy) const;
    void COPY(const int n, const double* x, const int incx, double* y, const int incy) const;
    double DOT(const int n, const double* x, const int incx, const double* y, const int incy) const;
    double NRM2(const int n, const double* x, const int incx) const;
    void SCAL(const int n, const double alpha, double* x, const int incx) const;
    int IAMAX(const int n, const double* x, const int incx) const;
    void GEMV(ETransp trans, const int m, const int n, const double alpha, const double* A, const int lda, const double* x, const int incx, const double beta, double* y, const int incy) const;
    void TRMV(EUplo uplo, ETransp trans, EDiag diag, const int n, const double* A, const int lda, double* x, const int incx) const;
    void GER(const int m, const int n, const double alpha, const double* x, const int incx, const double* y, const int incy, double* A, const int lda) const;
    void GEMM(ETransp transa, ETransp transb, const int m, const int n, const int k, const double alpha, const double* A, const int lda, const double* B, const int ldb, const double beta, double* C, const int ldc) const;
    void SYMM(ESide side, EUplo uplo, const int m, const int n, const double alpha, const double* A, const int lda, const double *B, const int ldb, const double beta, double *C, const int ldc) const;
    void TRMM(ESide side, EUplo uplo, ETransp transa, EDiag diag, const int m, const int n, const double alpha, const double* A, const int lda, double* B, const int ldb) const;
    void TRSM(ESide side, EUplo uplo, ETransp transa, EDiag diag, const int m, const int n, const double alpha, const double* A, const int lda, double* B, const int ldb) const;
  };

  // Explicit instantiation for template<int,complex<float> >

  template<>
  class TEUCHOS_LIB_DLL_EXPORT BLAS<int, std::complex<float> >
  {    
  public:
    inline BLAS(void) {}
    inline BLAS(const BLAS<int, std::complex<float> >& /*BLAS_source*/) {}
    inline virtual ~BLAS(void) {}
    void ROTG(std::complex<float>* da, std::complex<float>* db, float* c, std::complex<float>* s) const;
    void ROT(const int n, std::complex<float>* dx, const int incx, std::complex<float>* dy, const int incy, float* c, std::complex<float>* s) const;
    float ASUM(const int n, const std::complex<float>* x, const int incx) const;
    void AXPY(const int n, const std::complex<float> alpha, const std::complex<float>* x, const int incx, std::complex<float>* y, const int incy) const;
    void COPY(const int n, const std::complex<float>* x, const int incx, std::complex<float>* y, const int incy) const;
    std::complex<float> DOT(const int n, const std::complex<float>* x, const int incx, const std::complex<float>* y, const int incy) const;
    float NRM2(const int n, const std::complex<float>* x, const int incx) const;
    void SCAL(const int n, const std::complex<float> alpha, std::complex<float>* x, const int incx) const;
    int IAMAX(const int n, const std::complex<float>* x, const int incx) const;
    void GEMV(ETransp trans, const int m, const int n, const std::complex<float> alpha, const std::complex<float>* A, const int lda, const std::complex<float>* x, const int incx, const std::complex<float> beta, std::complex<float>* y, const int incy) const;
    void TRMV(EUplo uplo, ETransp trans, EDiag diag, const int n, const std::complex<float>* A, const int lda, std::complex<float>* x, const int incx) const;
    void GER(const int m, const int n, const std::complex<float> alpha, const std::complex<float>* x, const int incx, const std::complex<float>* y, const int incy, std::complex<float>* A, const int lda) const;
    void GEMM(ETransp transa, ETransp transb, const int m, const int n, const int k, const std::complex<float> alpha, const std::complex<float>* A, const int lda, const std::complex<float>* B, const int ldb, const std::complex<float> beta, std::complex<float>* C, const int ldc) const;
    void SYMM(ESide side, EUplo uplo, const int m, const int n, const std::complex<float> alpha, const std::complex<float>* A, const int lda, const std::complex<float> *B, const int ldb, const std::complex<float> beta, std::complex<float> *C, const int ldc) const;
    void TRMM(ESide side, EUplo uplo, ETransp transa, EDiag diag, const int m, const int n, const std::complex<float> alpha, const std::complex<float>* A, const int lda, std::complex<float>* B, const int ldb) const;
    void TRSM(ESide side, EUplo uplo, ETransp transa, EDiag diag, const int m, const int n, const std::complex<float> alpha, const std::complex<float>* A, const int lda, std::complex<float>* B, const int ldb) const;
  };

  // Explicit instantiation for template<int,complex<double> >

  template<>
  class TEUCHOS_LIB_DLL_EXPORT BLAS<int, std::complex<double> >
  {    
  public:
    inline BLAS(void) {}
    inline BLAS(const BLAS<int, std::complex<double> >& /*BLAS_source*/) {}
    inline virtual ~BLAS(void) {}
    void ROTG(std::complex<double>* da, std::complex<double>* db, double* c, std::complex<double>* s) const;
    void ROT(const int n, std::complex<double>* dx, const int incx, std::complex<double>* dy, const int incy, double* c, std::complex<double>* s) const;
    double ASUM(const int n, const std::complex<double>* x, const int incx) const;
    void AXPY(const int n, const std::complex<double> alpha, const std::complex<double>* x, const int incx, std::complex<double>* y, const int incy) const;
    void COPY(const int n, const std::complex<double>* x, const int incx, std::complex<double>* y, const int incy) const;
    std::complex<double> DOT(const int n, const std::complex<double>* x, const int incx, const std::complex<double>* y, const int incy) const;
    double NRM2(const int n, const std::complex<double>* x, const int incx) const;
    void SCAL(const int n, const std::complex<double> alpha, std::complex<double>* x, const int incx) const;
    int IAMAX(const int n, const std::complex<double>* x, const int incx) const;
    void GEMV(ETransp trans, const int m, const int n, const std::complex<double> alpha, const std::complex<double>* A, const int lda, const std::complex<double>* x, const int incx, const std::complex<double> beta, std::complex<double>* y, const int incy) const;
    void TRMV(EUplo uplo, ETransp trans, EDiag diag, const int n, const std::complex<double>* A, const int lda, std::complex<double>* x, const int incx) const;
    void GER(const int m, const int n, const std::complex<double> alpha, const std::complex<double>* x, const int incx, const std::complex<double>* y, const int incy, std::complex<double>* A, const int lda) const;
    void GEMM(ETransp transa, ETransp transb, const int m, const int n, const int k, const std::complex<double> alpha, const std::complex<double>* A, const int lda, const std::complex<double>* B, const int ldb, const std::complex<double> beta, std::complex<double>* C, const int ldc) const;
    void SYMM(ESide side, EUplo uplo, const int m, const int n, const std::complex<double> alpha, const std::complex<double>* A, const int lda, const std::complex<double> *B, const int ldb, const std::complex<double> beta, std::complex<double> *C, const int ldc) const;
    void TRMM(ESide side, EUplo uplo, ETransp transa, EDiag diag, const int m, const int n, const std::complex<double> alpha, const std::complex<double>* A, const int lda, std::complex<double>* B, const int ldb) const;
    void TRSM(ESide side, EUplo uplo, ETransp transa, EDiag diag, const int m, const int n, const std::complex<double> alpha, const std::complex<double>* A, const int lda, std::complex<double>* B, const int ldb) const;
  };

} // namespace Teuchos

#endif // _TEUCHOS_BLAS_HPP_
