/* $Id$ */
#ifndef __FRANCOTONE_P__
#define __FRANCOTONE_P__ 1

#define FRANCOTONE_EXPANSION_RATIO 3U

typedef enum FrancotoneFunc_ {
    FRANCOTONEFUNC_NOSEQ_1,
    FRANCOTONEFUNC_NOSEQ_2,
    FRANCOTONEFUNC_REPLACE,
    FRANCOTONEFUNC_PREG_REPLACE,
    FRANCOTONEFUNC_PREG_EXT_REPLACE,
    FRANCOTONEFUNC_FILTERCHARS,        
    FRANCOTONEFUNC_END
} FrancotoneFunc;

typedef struct FrancotoneRule_ {
    FrancotoneFunc func;
    const char *txt1;
    const char *txt2;
    pcre *rx_compiled;
    pcre_extra *rx_extra;
} FrancotoneRule;

static FrancotoneRule francotone_rules[] = {
    { FRANCOTONEFUNC_NOSEQ_1, "aeiouy", NULL, NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "0", "o", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "1", "un", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "2", "de", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "4", "katr", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "5", "cink", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "6", "sis", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "7", "set", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "8", "uit", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "9", "neuf", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "+", "plus", NULL, NULL },
    { FRANCOTONEFUNC_FILTERCHARS, "abcdefghijklmnopqrstuvwxyz", "", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "qu", "k", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "sce", "se", NULL, NULL },
    { FRANCOTONEFUNC_PREG_REPLACE, "(cs|ks|gs|qs|cz|kz|gz|qz)", "x", NULL, NULL },
    { FRANCOTONEFUNC_PREG_EXT_REPLACE, "[o]+[u]+([aeiouy])", "w$1", NULL, NULL },
    { FRANCOTONEFUNC_PREG_REPLACE, "^[h]+", "", NULL, NULL },
    { FRANCOTONEFUNC_PREG_EXT_REPLACE, "^[ckq]([aou])", "k$1", NULL, NULL },
    { FRANCOTONEFUNC_PREG_EXT_REPLACE, "^[cs]([eiy])", "s$1", NULL, NULL },
    { FRANCOTONEFUNC_PREG_EXT_REPLACE, "^[gj]([eiy])", "j$1", NULL, NULL },
    { FRANCOTONEFUNC_PREG_EXT_REPLACE, "[c]+([ei])", "s$1", NULL, NULL },
    { FRANCOTONEFUNC_PREG_REPLACE, "[i]+[l][l]+", "y", NULL, NULL },
    { FRANCOTONEFUNC_PREG_EXT_REPLACE, "(ai|ss|ch|nn)ent$", "$1", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "en", "an", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "eau", "o", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "au", "o", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "oe", "e", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "ai", "e", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "ph", "f", NULL, NULL },
    { FRANCOTONEFUNC_REPLACE, "in", "un", NULL, NULL },
    { FRANCOTONEFUNC_PREG_EXT_REPLACE, "([aiour])[t]$", "$1", NULL, NULL },
    { FRANCOTONEFUNC_PREG_EXT_REPLACE, "([aeiouy])[s]$", "$1", NULL, NULL },
    { FRANCOTONEFUNC_PREG_EXT_REPLACE, "([aeiouy])[h]+$", "$1", NULL, NULL },
    { FRANCOTONEFUNC_PREG_REPLACE, "(er|ez|ai|ais|aient)$", "e", NULL, NULL },
    { FRANCOTONEFUNC_PREG_EXT_REPLACE, "(en|ou|on)[ts]$", "$1", NULL, NULL },
    { FRANCOTONEFUNC_PREG_EXT_REPLACE, "(eu|ou)[x]+$", "$1", NULL, NULL },
    { FRANCOTONEFUNC_PREG_EXT_REPLACE, "([ao])[p]$", "$1", NULL, NULL },
    { FRANCOTONEFUNC_PREG_REPLACE, "an[sdt]$", "an", NULL, NULL },
    { FRANCOTONEFUNC_PREG_EXT_REPLACE, "([aeiouy])[s]$", "$1", NULL, NULL },
    { FRANCOTONEFUNC_NOSEQ_1, "aeiouy", NULL, NULL, NULL },
    { FRANCOTONEFUNC_END, NULL, NULL, NULL, NULL }
};

#endif
