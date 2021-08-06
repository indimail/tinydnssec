#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([curvedns],m4_normalize(m4_include([conf-version])),m4_normalize(m4_include([conf-email])),curvedns,[https://github.com/mbhangui/tinydnssec])
AC_CONFIG_SRCDIR([curvedns.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AM_INIT_AUTOMAKE([foreign])
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB

# Checks for libraries.
AC_CHECK_LIB(ev, ev_io_start, [AC_SUBST([LIB_EV], ["-lev"]) AC_DEFINE([HAVE_LIBEV], [1],[libevent])],,-lev)
AC_CHECK_LIB(sodium, crypto_box_curve25519xsalsa20poly1305_beforenm, [AC_SUBST([LIB_SODIUM], ["-lsodium"]) AC_DEFINE([HAVE_LIBSODIUM], [1],[sodium library])],,-lsodium)
AC_CHECK_LIB(sodium, crypto_box_curve25519xsalsa20poly1305_beforenm)

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdint.h stdlib.h string.h sys/param.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MMAP
AC_CHECK_FUNCS([clock_gettime gettimeofday memset mkdir select socket strerror])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
