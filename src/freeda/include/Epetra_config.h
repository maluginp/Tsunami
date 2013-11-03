/* $Header$ */

/* Copy of the following file, edited to be used with CMake */
/* src/Epetra_config.h.in.  Generated from configure.ac by autoheader.  */

/* Define the Fortran name mangling to be used for the BLAS */
#define F77_BLAS_MANGLE(name,NAME) name ## _

/* Define to dummy `main' function (if any) required to link to the Fortran
   libraries. */
/* #undef F77_DUMMY_MAIN */

/* Define to a macro mangling the given C identifier (in lower and upper
   case), which must not contain underscores, for linking with Fortran. */
#define F77_FUNC(name,NAME) name ## _

/* As F77_FUNC, but for C identifiers containing underscores. */
#define F77_FUNC_(name,NAME) NAME

/* Define if F77 and FC dummy `main' functions are identical. */
/* #undef FC_DUMMY_MAIN_EQ_F77 */

/* Define to 1 if you have the <assert.h> header file. */
/* #undef HAVE_ASSERT_H */

/* Define if you have a BLAS library. */
/* #undef HAVE_BLAS */

/* #undef EPETRA_ADDRESS64BIT */

/* Define if want to build epetra-abc */
#define HAVE_EPETRA_ARRAY_BOUNDS_CHECK

/* Define if want to build epetra-examples */
/* #undef HAVE_EPETRA_EXAMPLES */

/* Define if want to build epetra-tests */
/* #undef HAVE_EPETRA_TESTS */

/* Define if want to build with epetra enabled */
/* #undef HAVE_EPETRA_TEUCHOS */

/* Define if want to build examples */
/* #undef HAVE_EXAMPLES */

/* Define if you want to build export makefiles. */
/* #undef HAVE_EXPORT_MAKEFILES */

/* Define if want to build with fatal_messages enabled */
/* #undef HAVE_FATAL_MESSAGES */

/* Define if want to build with format_io enabled */
/* #undef HAVE_FORMAT_IO */

/* Define if want to build with fortran enabled */
/* #undef HAVE_FORTRAN_SUPPORT */

/* Define if you are using gnumake - this will shorten your link lines. */
/* #undef HAVE_GNUMAKE */

/* Define if you have LAPACK library. */
/* #undef HAVE_LAPACK */

/* Define if want to build libcheck */
/* #undef HAVE_LIBCHECK */

/* Define to 1 if you have the <math.h> header file. */
/* #undef HAVE_MATH_H */

/* Define to 1 if you have the <memory.h> header file. */
/* #undef HAVE_MEMORY_H */

/* define if we want to use MPI */
/* #undef HAVE_MPI */

/* define if we want to use OpenMP */
/* #undef Epetra_HAVE_OMP */

/* Define if want to build with oski enabled */
/* #undef HAVE_OSKI */

/* Define to 1 if you have the <sys/stat.h> header file. */
/* #undef HAVE_SYS_STAT_H */

/* Define to 1 if you have the <sys/types.h> header file. */
/* #undef HAVE_SYS_TYPES_H */

/* Define if want to build tests */
/* #undef HAVE_TESTS */

/* Define if want to build with threads enabled */
/* #undef HAVE_THREADS */

/* Define to 1 if you have the <unistd.h> header file. */
/* #undef HAVE_UNISTD_H */

/* Define if want to build with warning_messages enabled */
/* #undef HAVE_WARNING_MESSAGES */

/* Define to 1 if you have the ANSI C header files. */
/* #undef STDC_HEADERS */

/* #undef Epetra_ENABLE_CASK */
