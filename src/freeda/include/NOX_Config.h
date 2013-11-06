
/* #undef HAVE_MPI */

#define HAVE_NOX_DEBUG

/* #undef WITH_PRERELEASE */

/* #undef FINITE_VALUE_HAVE_GLOBAL_ISINF */
/* #undef FINITE_VALUE_HAVE_STD_ISINF */
/* #undef FINITE_VALUE_HAVE_GLOBAL_ISNAN */
/* #undef FINITE_VALUE_HAVE_STD_ISNAN */

/* #undef HAVE_LOCA_ANASAZI */

#define HAVE_NOX_AMESOS

#define HAVE_NOX_BELOS

#define HAVE_NOX_EPETRAEXT

#define HAVE_NOX_ML_EPETRA

#define HAVE_NOX_SACADO

#define HAVE_NOX_STRATIMIKOS

#define HAVE_NOX_THYRA

/* template qualifier required for calling template methods from non-template
   code */
/* #undef INVALID_TEMPLATE_QUALIFIER */

/* define if STL map key is required to be const */
/* #undef MUST_CONST_STL_MAP_KEY */

/* If optional MF TPL library is available */
/* #undef HAVE_LOCA_MF */

/* Hard-coding this until we get a check in Teuchos */
#define HAVE_LAPACK_GGEV
