dnl
dnl @synopsis CHECK_ZLIB
dnl
dnl @summary check zlib library
dnl
dnl TDYa127 <a127a127@gmail.com>
dnl Base on protobuf's configure.ac.

AC_DEFUN([CHECK_ZLIB],
[dnl
#  Configure zlibs.
AC_ARG_WITH([zlib],
            [AS_HELP_STRING([--with-zlib],
               [include classes for streaming compressed data in and out @<:@default=check@:>@])],
            [],
            [with_zlib=check])

# Check for zlib.
HAVE_ZLIB=0
AS_IF([test "$with_zlib" != no], [
  AC_MSG_CHECKING([zlib version])

  # First check the zlib header version.
  AC_COMPILE_IFELSE(
    [AC_LANG_PROGRAM([[
        #include <zlib.h>
        #if !defined(ZLIB_VERNUM) || (ZLIB_VERNUM < 0x1204)
        # error zlib version too old
        #endif
        ]], [])], [
    AC_MSG_RESULT([ok (1.2.0.4 or later)])

    # Also need to add -lz to the linker flags and make sure this succeeds.
    AC_SEARCH_LIBS([zlibVersion], [z], [
      AC_DEFINE([HAVE_ZLIB], [1], [Enable classes using zlib compression.])
      HAVE_ZLIB=1
    ], [
      AS_IF([test "$with_zlib" != check], [
        AC_MSG_FAILURE([--with-zlib was given, but no working zlib library was found])
      ])
    ])
  ], [
    AS_IF([test "$with_zlib" = check], [
      AC_MSG_RESULT([headers missing or too old (requires 1.2.0.4)])
    ], [
      AC_MSG_FAILURE([--with-zlib was given, but zlib headers were not present or were too old (requires 1.2.0.4)])
    ])
  ])
])
AM_CONDITIONAL([HAVE_ZLIB], [test $HAVE_ZLIB = 1])
])
