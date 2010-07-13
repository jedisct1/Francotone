/* $Id$ */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include "common.h"
#include "tools.h"
#include "tools_p.h"

#ifdef WITH_DMALLOC
# include <dmalloc.h>
#endif

char *strcasestr(const char *s, const char *find)
{
    char c;
    char sc;
    size_t len;
    
    if ((c = *find++) == 0) {
        return (char *) s;
    }
    c = (char) tolower((unsigned char) c);
    len = strlen(find);
    do {
        do {
            if ((sc = *s++) == 0) {
                return NULL;
            }
        } while ((char) tolower((unsigned char) sc) != c);
    } while (strncasecmp(s, find, len) != 0);
    s--;
    
    return (char *) s;
}

int remove_accent(const int c)
{
    switch (c) {
    case '�':    case '�':    case '�':    case '�':
        return 'e';
    case '�':    case '�':    case '�':    case '�':
        return 'E';
    case '�':    case '�':    case '�':    case '�':
    case '�':    case '�':
        return 'a';
    case '�':    case '�':    case '�':    case '�':    case '�':    case '�':
        return 'A';
    case '�':    case '�':    case '�':    case '�':
        return 'i';
    case '�':    case '�':    case '�':    case '�':
        return 'I';
    case '�':
        return 'c';
    case '�':        
        return 'C';
    case '�':    case '�':    case '�':    case '�':    case '�':    case '�':
    case '�':
        return 'o';
    case '�':    case '�':    case '�':    case '�':    case '�':    case '�':
        return 'O';
    case '�':    case '�':    case '�':    case '�':
        return 'u';
    case '�':    case '�':    case '�':    case '�':
        return 'U';
    case '�':
        return 'n';
    case '�':
        return 'N';
    case '�':
        return 'z';
    case '�':
        return 'Z';        
    case '�':    case '�':
        return 'y';
    case '�':    case '�':
        return 'Y';
    case '�':    case '�':
        return '"';
    case '�':    case '�':
        return '-';
    case '�':
        return '*';
    case '�':
        return '/';
    case '\x82':
        return '\'';
    case '\x83':
        return 'f';
    case '\x84':
        return '"';
    case '\x85':
        return '.';
    case '\x86':    case '\x87':
        return '*';
    case '\x89':
        return '^';
    case '\x8a':
        return 'S';
    case '\x8b':
        return '<';
    case '\x8c':
        return '�';
    case '\x8e':
        return 'Z';
    case '\x91':    case '\x92':        
        return '\'';
    case '\x93':    case '\x94':
        return '\"';
    case '\x95':    case '\x96':
    case '\x97':
        return '-';
    case '\x98':
        return '~';
    case '\x99':
        return ' ';
    case '\x9a':
        return 's';
    case '\x9b':
        return '>';
    case '\x9c':
        return '�';
    case '\x9e':
        return 'z';
    case '\x9f':
        return 'Y';
    }
    return c;
}

int entity_to_latin15(const char * const xhtml)
{
    const XHTMLEntities *scanned_entity = xhtml_entities;
    
    do {
        if (strncmp(xhtml, scanned_entity->entity,
                    strlen(scanned_entity->entity)) == 0) {            
            return (int) (unsigned char) *scanned_entity->latin15;
        }
        scanned_entity++;
    } while (scanned_entity->entity != NULL);
    
    return ' ';
}

#ifndef HAVE_STRLCPY
void strlcpy(char * const dst, const char * const src, size_t size)
{
    if (size <= (size_t) 1U) {
        *dst = 0;
        return;
    }
    size--;
    strncpy(dst, src, size);
    dst[size] = 0;
}
#endif

int string_replace(char *keyword, size_t sizeof_keyword,
                   const char *str1, const char *str2)
{
    char *tmp;
    const size_t strlen_str1 = strlen(str1);
    const size_t strlen_str2 = strlen(str2);
    const size_t strlen_keyword = strlen(keyword);
    size_t gap;
    size_t shift;
    
    if (strlen_keyword >= sizeof_keyword) {
        FATAL();
    }
    gap = sizeof_keyword - strlen_keyword - (size_t) 1U;
    tmp = keyword;
    while ((tmp = strstr(tmp, str1)) != NULL) {
        if (strlen_str1 > strlen_str2) {
            shift = strlen_str1 - strlen_str2;
            memmove(tmp, tmp + shift, strlen(tmp + shift) + (size_t) 1U);
        } else if (strlen_str1 < strlen_str2) {
            shift = strlen_str2 - strlen_str1;
            if (shift > gap) {
                return -1;
            }
            gap -= shift;
            memmove(tmp + shift, tmp, strlen(tmp) + (size_t) 1U);
        }
        memcpy(tmp, str2, strlen_str2);
        tmp += strlen_str2;
    }    
    return 0;
}

