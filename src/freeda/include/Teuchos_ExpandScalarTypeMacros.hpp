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

#ifndef TEUCHOS_EXPAND_SCALAR_TYPE_MACROS_HPP
#define TEUCHOS_EXPAND_SCALAR_TYPE_MACROS_HPP


/*! \file Teuchos_UnitTestHelpers.hpp

\brief Macros for expanding code
*/


#include "Teuchos_ConfigDefs.hpp"

#ifdef HAVE_TEUCHOS_FLOAT
#  define TEUCHOS_MACRO_EXPAND_FLOAT(INSTANT_MACRO)\
    INSTANT_MACRO(float)
#  define TEUCHOS_CLASS_TEMPLATE_INSTANT_FLOAT(CLASSNAME)\
    template class CLASSNAME<float>;
#else
#  define TEUCHOS_MACRO_EXPAND_FLOAT(INSTANT_MACRO)
#  define TEUCHOS_CLASS_TEMPLATE_INSTANT_FLOAT(CLASSNAME)
#endif


#define TEUCHOS_MACRO_EXPAND_DOUBLE(INSTANT_MACRO)\
  INSTANT_MACRO(double)
#define TEUCHOS_CLASS_TEMPLATE_INSTANT_DOUBLE(CLASSNAME)\
  template class CLASSNAME<double>;

#if defined(HAVE_TEUCHOS_COMPLEX) && defined(HAVE_TEUCHOS_FLOAT)
#  define TEUCHOS_MACRO_EXPAND_COMPLEX_FLOAT(INSTANT_MACRO)\
     INSTANT_MACRO(std::complex<float>)
#  define TEUCHOS_CLASS_TEMPLATE_INSTANT_COMPLEX_FLOAT(CLASSNAME)\
     template class CLASSNAME<std::complex<float> >;
#else
#  define TEUCHOS_MACRO_EXPAND_COMPLEX_FLOAT(INSTANT_MACRO)
#  define TEUCHOS_CLASS_TEMPLATE_INSTANT_COMPLEX_FLOAT(CLASSNAME)
#endif


#ifdef HAVE_TEUCHOS_COMPLEX
#  define TEUCHOS_MACRO_EXPAND_COMPLEX_DOUBLE(INSTANT_MACRO)\
     INSTANT_MACRO(std::complex<double>)
#  define TEUCHOS_CLASS_TEMPLATE_INSTANT_COMPLEX_DOUBLE(CLASSNAME)\
     template class CLASSNAME<std::complex<double> >;
#else
#  define TEUCHOS_MACRO_EXPAND_COMPLEX_DOUBLE(INSTANT_MACRO)
#  define TEUCHOS_CLASS_TEMPLATE_INSTANT_COMPLEX_DOUBLE(CLASSNAME)
#endif


/** \brief Instantiate a macro template for the set of supported real scalar
 * types.
 */
#define TEUCHOS_MACRO_EXPAND_REAL_SCALAR_TYPES(MACRONAME) \
  TEUCHOS_MACRO_EXPAND_FLOAT(MACRONAME) \
  TEUCHOS_MACRO_EXPAND_DOUBLE(MACRONAME)


/** \brief Instantiate a macro template for the set of supported real and
 * complex scalar types.
 */
#define TEUCHOS_MACRO_EXPAND_SCALAR_TYPES(MACRONAME)\
  TEUCHOS_MACRO_EXPAND_FLOAT(MACRONAME) \
  TEUCHOS_MACRO_EXPAND_DOUBLE(MACRONAME) \
  TEUCHOS_MACRO_EXPAND_COMPLEX_FLOAT(MACRONAME) \
  TEUCHOS_MACRO_EXPAND_COMPLEX_DOUBLE(MACRONAME)


#endif  // TEUCHOS_EXPAND_SCALAR_TYPE_MACROS_HPP
