#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([sqluz], [0.0.1], [supmacro@foxmail.com])
AC_CONFIG_SRCDIR([src/lz_xgsql.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-ldl':
AC_CHECK_LIB([dl], [main])
# FIXME: Replace `main' with a function in `-lodbc':
AC_CHECK_LIB([odbc], [main])
# FIXME: Replace `main' with a function in `-lodbcinst':
AC_CHECK_LIB([odbcinst], [main])
# FIXME: Replace `main' with a function in `-lxgci':
AC_CHECK_LIB([xgci], [main])

# Checks for header files.
AC_CHECK_HEADERS([stddef.h stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([bzero strchr strdup strstr])

AC_CONFIG_FILES([Makefile
                 internal/Makefile
                 sh/Makefile
                 src/Makefile])
AC_OUTPUT
