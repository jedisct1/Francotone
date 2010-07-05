/* $Id: common.h 4180 2006-08-01 15:26:53Z j $ */
#ifndef __COMMON_H__
#define __COMMON_H__ 1

#include <stdio.h>
#ifdef STDC_HEADERS
# include <stdlib.h>
# include <stddef.h>
# include <stdarg.h>
#else
# ifdef HAVE_STDLIB_H
#  include <stdlib.h>
# endif
#endif
#if HAVE_STRING_H
# if !STDC_HEADERS && HAVE_MEMORY_H
#  include <memory.h>
# endif
# include <string.h>
#else
# if HAVE_STRINGS_H
#  include <strings.h>
# endif
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#ifdef HAVE_SETLOCALE
# include <locale.h>
#endif
#include <ctype.h>
#include <pcre.h>

#ifndef errno
extern int errno;
#endif

#ifdef HAVE_ALLOCA
# ifdef HAVE_ALLOCA_H
#  include <alloca.h>
# endif
# define ALLOCA(X) alloca(X)
# define ALLOCA_FREE(X) do { } while (0)
#else
# define ALLOCA(X) malloc(X)
# define ALLOCA_FREE(X) free(X)
#endif

#ifndef HAVE_STRTOULL
# ifdef HAVE_STRTOQ
#  define strtoull(X, Y, Z) strtoq(X, Y, Z)
# else
#  define strtoull(X, Y, Z) strtoul(X, Y, Z)
# endif
#endif

#ifndef ULONG_LONG_MAX
# define ULONG_LONG_MAX (1ULL << 63)
#endif

#ifdef WITH_DMALLOC
# define _exit(X) exit(X)
#endif

#define FATAL() do { fprintf(stderr, "*FATAL* [" __FILE__ "] [%d]\n", \
                                 __LINE__); abort(); } while (0)

#endif
