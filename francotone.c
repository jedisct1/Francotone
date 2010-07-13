/* $Id */
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include "common.h"
#include "tools.h"
#include "francotone.h"
#include "francotone_p.h"

static void filter_chars(char *txt, const char * const chars)
{
    size_t strlen_txt;
    size_t spn;
    
    if ((strlen_txt = strlen(txt)) <= (size_t) 0U) {
        return;
    }
    for (;;) {
        spn = strspn(txt, chars);
        txt += spn;
        strlen_txt -= spn;
        if (*txt == 0) {
            break;
        }
        memmove(txt, txt + 1, strlen_txt);
    }
}

static int francotone_rule_init_pcre(FrancotoneRule * const francotone_rule)
{
    const char *rx_err;
    int rx_erroffset;    
    
    if (francotone_rule->rx_compiled != NULL) {
        return 0;
    }
    if ((francotone_rule->rx_compiled = pcre_compile
         (francotone_rule->txt1, PCRE_CASELESS | PCRE_NO_UTF8_CHECK,
          &rx_err, &rx_erroffset, NULL)) == NULL) {
        fprintf(stderr, "rx_compile: [%s]\n", rx_err);
        return -1;
    }
    francotone_rule->rx_extra =
        pcre_study(francotone_rule->rx_compiled, 0, &rx_err);
    
    return 0;
}

static int noseq_1(char *keyword, const char *chars)
{
    char *ptr = keyword;
    int lastchar = EOF;
    int c;
    
    while ((c = *keyword) != 0) {
        if (lastchar != c || strchr(chars, c) == NULL) {
            *ptr++ = c;
            lastchar = c;
        }
        keyword++;
    }
    *ptr = 0;
    
    return 0;
}

static int noseq_2(char *keyword, const char *chars)
{
    (void) keyword;
    (void) chars;
    
    return 0;
}

static int preg_replace(char *keyword, size_t sizeof_keyword,
                        FrancotoneRule * const francotone_rule)
{
    int rx_matches[3];
    size_t strlen_txt1;
    size_t strlen_txt2;
    size_t shift;
    size_t gap;
    
    if (francotone_rule_init_pcre(francotone_rule) != 0) {
        return -1;
    }
    strlen_txt2 = strlen(francotone_rule->txt2);
    gap = sizeof_keyword - strlen(keyword) - (size_t) 1U;
    
    again:
    if (*keyword == 0 ||
        pcre_exec(francotone_rule->rx_compiled, francotone_rule->rx_extra,
                  keyword, strlen(keyword), 0, PCRE_NOTEMPTY, rx_matches,
                  sizeof rx_matches / sizeof rx_matches[0]) < 0) {
        return 0;
    }
    if (rx_matches[0] < 0 || rx_matches[1] < 0) {
        FATAL();
    }
    keyword += rx_matches[0];
    strlen_txt1 = (size_t) (rx_matches[1] - rx_matches[0]);
    if (strlen_txt1 > strlen_txt2) {
        shift = strlen_txt1 - strlen_txt2;
        memmove(keyword, keyword + shift,
                strlen(keyword + shift) + (size_t) 1U);
    } else if (strlen_txt1 < strlen_txt2) {
        shift = strlen_txt2 - strlen_txt1;
        if (shift > gap) {
            return -1;
        }
        gap -= shift;
        memmove(keyword + shift, keyword,
                strlen(keyword) + (size_t) 1U);
    }
    if (strlen_txt2 > (size_t) 0U) {
        memcpy(keyword, francotone_rule->txt2, strlen_txt2);
        keyword += strlen_txt2;
    }
    goto again;
}

void francotone_free(void)
{
    FrancotoneRule *francotone_rule = francotone_rules;

    while (francotone_rule->func != FRANCOTONEFUNC_END) {
        if (francotone_rule->rx_compiled != NULL) {
            pcre_free(francotone_rule->rx_compiled);
            francotone_rule->rx_compiled = NULL;
        }
        if (francotone_rule->rx_extra != NULL) {
            pcre_free(francotone_rule->rx_extra);
            francotone_rule->rx_extra = NULL;
        }
        francotone_rule++;        
    }
}

static int preg_ext_replace(char *keyword, size_t sizeof_keyword,
                            FrancotoneRule * const francotone_rule)
{
    int rx_matches[6];
    char *tmp;
    char *tmp2;
    char *subreg;
    size_t strlen_subreg;
    size_t strlen_match;    
    size_t strlen_tmp;
    size_t strlen_diff;
    int offset = 0;
    const size_t sizeof_tmp = sizeof_keyword;
    const size_t sizeof_subreg = sizeof_keyword;

    if ((tmp = ALLOCA(sizeof_keyword)) == NULL) {
        return -1;
    }
    if ((tmp2 = ALLOCA(sizeof_keyword)) == NULL) {
        ALLOCA_FREE(tmp);
        return -1;
    }
    if ((subreg = ALLOCA(sizeof_keyword)) == NULL) {
        ALLOCA_FREE(tmp);
        ALLOCA_FREE(tmp2);        
        return -1;
    }
    if (francotone_rule_init_pcre(francotone_rule) != 0) {
        ALLOCA_FREE(tmp);
        ALLOCA_FREE(tmp2);
        ALLOCA_FREE(subreg);
        return -1;
    }    
    again:
    if (*keyword == 0 ||
        pcre_exec(francotone_rule->rx_compiled, francotone_rule->rx_extra,
                  keyword, strlen(keyword), offset, PCRE_NOTEMPTY, rx_matches,
                  sizeof rx_matches / sizeof rx_matches[0]) < 0) {
        ALLOCA_FREE(tmp);
        ALLOCA_FREE(tmp2);
        ALLOCA_FREE(subreg);
        return 0;
    }
    if (rx_matches[0] < 0 || rx_matches[1] <= 0 ||
        rx_matches[2] < 0 || rx_matches[3] <= 0 ||
        rx_matches[1] <= rx_matches[0] ||
        rx_matches[3] < rx_matches[2]) {
        FATAL();
    }
    strlen_subreg = rx_matches[3] - rx_matches[2];
    if (strlen_subreg >= sizeof_subreg) {
        strlen_subreg = sizeof_subreg - (size_t) 1U;
    }
    strncpy(subreg, keyword + rx_matches[2], strlen_subreg);
    subreg[strlen_subreg] = 0;
    strlcpy(tmp, francotone_rule->txt2, sizeof_tmp);
    string_replace(tmp, sizeof_tmp, "$1", subreg);
    strlen_match = rx_matches[1] - rx_matches[0];
    strlen_tmp = strlen(tmp);
    if (strlen_match < strlen_tmp) {
        FATAL();
    }
    memcpy(keyword + rx_matches[0], tmp, strlen_tmp);
    if ((strlen_diff = strlen_match - strlen_tmp) > (size_t) 0U) {
        memmove(keyword + rx_matches[0] + strlen_tmp,
                keyword + rx_matches[0] + strlen_tmp + strlen_diff,
                strlen(keyword + rx_matches[0] +
                       strlen_tmp + strlen_diff) + (size_t) 1U);
    }
    offset = rx_matches[1] - strlen_diff;
    goto again;
}

char *francotone(const char * const txt)
{
    char *phonetic;
    FrancotoneRule *francotone_rule = francotone_rules;
    size_t sizeof_phonetic;
    const size_t sizeof_txt = strlen(txt) + (size_t) 1U;
    
    if (*txt == 0) {
        FATAL();
    }
    if ((sizeof_phonetic = sizeof_txt * FRANCOTONE_EXPANSION_RATIO)
        < sizeof_txt || (phonetic = malloc(sizeof_phonetic)) == NULL) {
        return NULL;
    }
    memcpy(phonetic, txt, sizeof_txt);
    while (francotone_rule->func != FRANCOTONEFUNC_END) {
        switch (francotone_rule->func) {
        case FRANCOTONEFUNC_NOSEQ_1:
            noseq_1(phonetic, francotone_rule->txt1);
            break;
        case FRANCOTONEFUNC_NOSEQ_2:
            noseq_2(phonetic, francotone_rule->txt1);
            break;
        case FRANCOTONEFUNC_REPLACE:
            string_replace(phonetic, sizeof_phonetic,
                           francotone_rule->txt1, francotone_rule->txt2);
            break;
        case FRANCOTONEFUNC_PREG_REPLACE:
            preg_replace(phonetic, sizeof_phonetic, francotone_rule);
            break;
        case FRANCOTONEFUNC_PREG_EXT_REPLACE:
            preg_ext_replace(phonetic, sizeof_phonetic, francotone_rule);
            break;
        case FRANCOTONEFUNC_FILTERCHARS:
            filter_chars(phonetic, francotone_rule->txt1);
            break;
        default:
            FATAL();
        }
        francotone_rule++;
    }
    return phonetic;
}
