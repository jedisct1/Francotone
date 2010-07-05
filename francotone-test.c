/* $Id: francotone-test.c 4184 2006-08-01 15:40:10Z j $ */
#define DEFINE_GLOBALS 1
#ifdef HAVE_CONFIG_H
# include <config.h>
#endif
#include "common.h"
#include "francotone.h"

#ifdef WITH_DMALLOC
# include <dmalloc.h>
#endif

static void usage(void)
{
    puts("\nUsage: francotone-test <keyword>\n");
    exit(1);
}

int main(int argc, char *argv[])
{
    char *phonetic;
    
    if (argc < 2) {
        usage();
    }
    if ((phonetic = francotone(argv[1])) == NULL) {
        fprintf(stderr, "Sorry\n");
        return 1;
    }
    printf("[%s] => [%s]\n", argv[1], phonetic);
    free(phonetic);
    francotone_free();
    
    return 0;
}
