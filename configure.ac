AC_INIT([subleq], [0.0.0], [rcook@rcook.org])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CXX
AC_PROG_RANLIB
AM_PROG_AR
AC_SUBST([AM_CXXFLAGS], ["-std=c++11 -I.. -Werror -Wall"])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
  src/libsqasm/Makefile
  src/libsqrun/Makefile
  src/sqasm/Makefile
  src/sqexec/Makefile
  src/sqrun/Makefile
])
AC_OUTPUT

