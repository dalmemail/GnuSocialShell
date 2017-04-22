AC_INIT([GnuSocialShell], [1.2.0], [DalmeGNU])

AM_INIT_AUTOMAKE([subdir-objects])

AC_PROG_CC

AC_HEADER_STDC
AC_TYPE_SIZE_T

# curl is required
AC_CHECK_HEADER([curl/curl.h],
                [AC_DEFINE(HAVE_LIBCURL_H, 1, [Presence of libcurl headers (required)] )],
                [AC_MSG_ERROR([libcurl headers not found])])
AC_LIB_LINKFLAGS([curl])

# readline is optional but very much recommended
AC_ARG_WITH([readline],
  [AS_HELP_STRING([--with-readline],
    [compile with readline @<:@default=check@:>@])],
  [],
  [with_readline=check])
AS_CASE(["$with_readline"],
  [yes], 
    [AC_CHECK_LIB([readline], [readline],
      [HAVE_READLINE=1])],
  [no],
    [HAVE_READLINE=0],
  [AC_CHECK_LIB([readline], [readline],
    [HAVE_READLINE=1],
    [HAVE_READLINE=0])])

AM_CONDITIONAL([USE_READLINE], [test "$with_readline" != no -a "$HAVE_READLINE" -eq 1])

AM_COND_IF([USE_READLINE],
   [AC_CHECK_HEADERS([readline/readline.h readline/history.h],
    [AC_DEFINE(HAVE_READLINE_H, 1, [Presence of readline (if requested)] )]
   )
    AC_LIB_LINKFLAGS([readline])], [])

AC_CONFIG_HEADERS([config.h])

# AC_SUBST([LIBS])
# AC_SUBST([CFLAGS])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
