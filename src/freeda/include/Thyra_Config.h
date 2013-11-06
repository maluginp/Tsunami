
#define HAVE_THYRA_EPETRA

#define HAVE_THYRA_EPETRAEXT

/* #undef HAVE_THYRA_TEUCHOS_BLASFLOAT */

/* #undef HAVE_THYRA_FLOAT */

/* #undef HAVE_THYRA_COMPLEX */

#define HAVE_THYRA_DEBUG

/* #undef HAVE_THYRA_EXPLICIT_INSTANTIATION */

/* #undef HAVE_THYRA_ME_POLYNOMIAL */

#ifndef THYRA_DEPRECATED
#  if (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#    define THYRA_DEPRECATED  __attribute__((__deprecated__))
#  else
#    define THYRA_DEPRECATED
#  endif
#endif

