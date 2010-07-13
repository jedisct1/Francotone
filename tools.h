/* $Id$ */
#ifndef __TOOLS_H__
#define __TOOLS_H__ 1

char *strcasestr(const char *s, const char *find);
int remove_accent(const int c);
int entity_to_latin15(const char * const xhtml);

#ifndef HAVE_STRLCPY
void strlcpy(char * const dst, const char * const src, const size_t size);
#endif

int string_replace(char *keyword, size_t sizeof_keyword,
                   const char *str1, const char *str2);

#endif
