AC_INIT([hello], [1.0]) # good
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC

# Use either new LT_INIT or old AC_DISABLE_STATIC/AC_PROG_LIBTOOL macros
m4_ifdef([LT_INIT],
  [LT_INIT([disable-static])],
  [AC_DISABLE_STATIC
   AC_PROG_LIBTOOL])

AC_ARG_WITH([pthread], [Path to pthread])
AS_IF([test -n "$with_pthread"],
    [LDFLAGS+=" -L$with_pthread/lib"]
)
AC_CHECK_LIB([pthread], [pthread_mutex_lock])
AC_CHECK_LIB([pthreadGC2], [pthread_mutex_lock])
AS_IF([test "$ac_cv_lib_pthread" = "no" && test "$ac_cv_lib_pthreadGC2" = "no"],
[
  AC_MSG_ERROR([Could not find pthreads library])
])
AC_CHECK_FUNCS([pthread_mutexattr_settype])
AC_CHECK_FUNC([pthread_mutexattr_setrobust],
[
    AC_MSG_CHECKING([whether pthread_mutexattr_setrobust can be used on this platform])
    AS_CASE([$target_os],
    [*solaris*],
    [
        # Disable pthread_mutexattr_setrobust on Solaris, it is reported as
        # existing on Solaris 11, but known to be broken.
        AC_MSG_RESULT([no])
    ],
    [
        AC_DEFINE([HAVE_PTHREAD_MUTEXATTR_SETROBUST])
        AC_MSG_RESULT([yes])
    ])
])

AC_CONFIG_SRCDIR([mdb.c])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
