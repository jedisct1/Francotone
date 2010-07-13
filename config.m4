dnl $Id$
dnl config.m4 for extension francotone

PHP_ARG_ENABLE(francotone, whether to enable francotone support,
[  --enable-francotone     Enable francotone support])

AC_CHECK_FUNCS(strlcpy)

for i in /usr/local /usr; do
  if test -f "$i/include/pcre.h"; then
    PHP_PCRE_PREFIX="$i"
    break
  fi
done
if test -z "$PHP_PCRE_PREFIX"; then
  PHP_PCRE_PREFIX="/usr"
fi

CFLAGS="-I$PHP_PCRE_PREFIX/include $CFLAGS"
LDFLAGS="-L$PHP_PCRE_PREFIX/$PHP_LIBDIR $LDFLAGS"

PHP_CHECK_LIBRARY(pcre, pcre_free, [],
  [AC_MSG_ERROR([PCRE library is required.])])
          
PHP_ADD_LIBRARY(pcre,, FRANCOTONE_SHARED_LIBADD)          
PHP_ADD_INCLUDE($PCRE_DIR/include)

PHP_SUBST(FRANCOTONE_SHARED_LIBADD)

if test "$PHP_FRANCOTONE" != "no"; then
  PHP_NEW_EXTENSION(francotone, francotone.c tools.c francotone-php.c, $ext_shared)
fi


