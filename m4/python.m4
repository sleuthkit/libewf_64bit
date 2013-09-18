dnl Functions for Python bindings
dnl
dnl Version: 20130303

dnl Function to check if the python binary is available
dnl "python python# python#.#"
AC_DEFUN([AX_PROG_PYTHON],
 [AC_CHECK_PROGS(
  [PYTHON],
  [python python2 python3 python3.2 python3.1 python3.0 python2.7 python2.6 python2.5])
 AS_IF(
  [test x"$PYTHON" != x],
  [ax_prog_python_version=`$PYTHON -c "import sys; sys.stdout.write(sys.version[[:3]])" 2>/dev/null`;
  AC_SUBST(
   [PYTHON_VERSION],
   [$ax_prog_python_version])

  ax_prog_python_platform=`$PYTHON -c "import sys; sys.stdout.write(sys.platform)" 2>/dev/null`;
  AC_SUBST(
   [PYTHON_PLATFORM],
   [$ax_prog_python_platform])
  ],
  [AC_MSG_ERROR(
   [Unable to find python])
  ])
 AC_SUBST(
  [PYTHON],
  [$PYTHON])
 ])

dnl Function to check if the python-config binary is available
dnl "python$PYTHON_VERSION-config python-config"
AC_DEFUN([AX_PROG_PYTHON_CONFIG],
 [AS_IF(
  [test x"$PYTHON" != x],
  [AC_CHECK_PROGS(
   [PYTHON_CONFIG],
   [python$PYTHON_VERSION-config python-config])
  ])
 AS_IF(
  [test x"$PYTHON_CONFIG" = x],
  [AC_MSG_ERROR(
   [Unable to find python-config])
  ])
 AC_SUBST(
  [PYTHON_CONFIG],
  [$PYTHON_CONFIG])
 ])

dnl Function to detect if Python build environment is available
AC_DEFUN([AX_PYTHON_CHECK],
 [AX_PROG_PYTHON
 AX_PROG_PYTHON_CONFIG

 AS_IF(
  [test x"$PYTHON_CONFIG" != x],
  [dnl Check for Python includes
  PYTHON_INCLUDES=`$PYTHON_CONFIG --includes 2>/dev/null`;

  AC_MSG_CHECKING(
   [for Python includes])
  AC_MSG_RESULT(
   [$PYTHON_INCLUDES])

  dnl Check for Python libraries
  PYTHON_LIBS=`$PYTHON_CONFIG --libs 2>/dev/null`;

  AC_MSG_CHECKING(
   [for Python libraries])
  AC_MSG_RESULT(
   [$PYTHON_LIBS])

  dnl Check for the existence of Python.h
  BACKUP_CPPFLAGS="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $PYTHON_INCLUDES"

  AC_CHECK_HEADERS(
   [Python.h],
   [ac_cv_header_python_h=yes],
   [ac_cv_header_python_h=no])

  CPPFLAGS="$BACKUP_CPPFLAGS"
 ])

 AS_IF(
  [test x"$ac_cv_header_python_h" != xyes],
  [ac_cv_enable_python=no],
  [ac_cv_enable_python=$PYTHON_VERSION
  AC_SUBST(
   [PYTHON_CPPFLAGS],
   [$PYTHON_INCLUDES])

  AC_SUBST(
   [PYTHON_LDFLAGS],
   [$PYTHON_LIBS])

  dnl Check for Python prefix
  AS_IF(
   [test "x$ac_cv_with_pyprefix" = x || test "x$ac_cv_with_pyprefix" = xno],
   [ax_python_prefix="\${prefix}"],
   [ax_python_prefix=`$PYTHON_CONFIG --prefix 2>/dev/null`])

  AC_SUBST(
   [PYTHON_PREFIX],
   [$ax_python_prefix])

  dnl Check for Python exec-prefix
  AS_IF(
   [test "x$ac_cv_with_pyprefix" = x || test "x$ac_cv_with_pyprefix" = xno],
   [ax_python_exec_prefix="\${exec_prefix}"],
   [ax_python_exec_prefix=`$PYTHON_CONFIG --exec-prefix 2>/dev/null`])

  AC_SUBST(
   [PYTHON_EXEC_PREFIX],
   [$ax_python_exec_prefix])

  dnl Check for Python library directory
  ax_python_pythondir_suffix=`$PYTHON -c "import sys; from distutils import sysconfig; sys.stdout.write(sysconfig.get_python_lib(0,0,prefix=''))" 2>/dev/null`;

  AS_IF(
   [test "x$ac_cv_with_pyprefix" = x || test "x$ac_cv_with_pyprefix" = xno],
   [ax_python_pythondir="$ax_python_prefix/$ax_python_pythondir_suffix"],
   [ax_python_pythondir=`$PYTHON -c "import distutils.sysconfig;print distutils.sysconfig.get_python_lib() " 2>/dev/null`])

  AC_SUBST(
   [pythondir],
   [$ax_python_pythondir])

  dnl Check for Python platform specific library directory
  ax_python_pyexecdir_suffix=`$PYTHON -c "import sys; from distutils import sysconfig; sys.stdout.write(sysconfig.get_python_lib(1,0,prefix=''))" 2>/dev/null`;
  ax_python_library_dir=`$PYTHON -c "import distutils.sysconfig;print distutils.sysconfig.get_python_lib(True) " 2>/dev/null`;

  AS_IF(
   [test "x$ac_cv_with_pyprefix" = x || test "x$ac_cv_with_pyprefix" = xno],
   [ax_python_pyexecdir="$ax_python_exec_prefix/$ax_python_pyexecdir_suffix"],
   [ax_python_pyexecdir=$ax_python_library_dir])

  AC_SUBST(
   [pyexecdir],
   [$ax_python_pyexecdir])

  AC_SUBST(
   [PYTHON_LIBRARY_DIR],
   [$ax_python_pyexecdir_suffix])
  AC_SUBST(
   [PYTHON_PACKAGE_DIR],
   [$ax_python_library_dir])
  ])
 ])

dnl Function to detect if to enable Python
AC_DEFUN([AX_PYTHON_CHECK_ENABLE],
 [dnl Deprecated way of enabling Python bindings
 AX_COMMON_ARG_ENABLE(
  [python],
  [python],
  [build Python bindings],
  [no])
 AX_COMMON_ARG_WITH(
  [pyprefix],
  [pyprefix],
  [use `python-config --prefix' to determine the Python directories],
  [no],
  [no])

 AS_IF(
  [test "x$ac_cv_enable_python" != xno],
  [AX_PYTHON_CHECK])

 AS_IF(
  [test "x$ac_cv_enable_python" != xno],
  [AC_DEFINE(
   [HAVE_PYTHON],
   [1],
   [Define to 1 if you have Python])
  ])

 AM_CONDITIONAL(
  HAVE_PYTHON,
  [test "x$ac_cv_enable_python" != xno])
])

