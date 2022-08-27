/*
 * wordperline - generate a word per line for all of the specified files; if
 *               no files are specified, work on standard input
 *
 * words by default are separated by white space
 * if -p is specified, punctuation characters also separate words
 * if -l is specified, convert upper to lower case
 *
 * outputs words, one per line, on standard output
 *
 * Author: Joe Sventek
 * usage: ./wordperline [-lp] [FILE ...]
 */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "stringADT.h"
#include "jsheap.h"
#include "Accumulator.h"

#define USESTR "usage: %s [-lp] [FILE ...]"

long nAccumulators = 0L;
Accumulator *accumulators[1000];

void dumpAccumulators(FILE *fd) {
	int i;
	
	for (i = 0; accumulators[i] != NULL; i++) {
		long long musecs = accumulators[i]->musecs, m1 = (musecs / 1000);
		int m2 = (musecs % 1000);
		char *name = accumulators[i]->name;
		long long calls = accumulators[i]->calls;
		fprintf(fd, "%s %15Ld calls%15Ld.%03dms\n", name, calls, m1, m2);
	}

}


void wordPerLine(const String *stADT, int punct, int lower, FILE *fp) {
    char buf[4096];

    while (fgets(buf, sizeof buf, fp) != NULL) {
        const ArrayList *al;
        long i;

        if (punct) {
            for (i = 0; buf[i] != '\0'; i++)
                if (ispunct(buf[i]) || buf[i] == '\n')
                    buf[i] = ' ';
        }
        stADT->clear(stADT);
        stADT->append(stADT, buf);
        if (lower)
            stADT->lower(stADT);
        al = stADT->split(stADT, "");
        if (al == NULL) {            /* line contains no words */
            continue;
        }
        for (i = 0; i < al->size(al); i++) {
            char *s;
            (void) al->get(al, i, (void **)&s);
            printf("%s\n", s);
        }
        al->destroy(al, jsFree);
    }
}

int main(int argc, char *argv[]) {
    int punct = 0;
    int lower = 0;
    int i, retval = 0;
    const String *stADT = String_create("");

    if (stADT == NULL) {
        fprintf(stderr, "Unable to create String instance\n");
        retval = 1;
        goto cleanup;
    }
    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            int j;
            for (j = 1; argv[i][j] != '\0'; j++) {
                if (argv[i][j] == 'p')
                    punct = 1;
                else if (argv[i][j] == 'l')
                    lower = 1;
                else {
                    fprintf(stderr, "invalid flag, ");
                    fprintf(stderr, USESTR, argv[0]);
                    fputc('\n', stderr);
                    retval = 1;
                    goto cleanup;
                }
            }
        } else
            break;
    }
    if (i >= argc)
        wordPerLine(stADT, punct, lower, stdin);
    else {
        for ( ; i < argc; i++) {
            FILE *fp = fopen(argv[i], "r");
            if (fp == NULL) {
                fprintf(stderr, "Unable to open file: %s\n", argv[i]);
                retval = 1;
                goto cleanup;
            }
            wordPerLine(stADT, punct, lower, fp);
            fclose(fp);
        }
    }
    dumpAccumulators(stderr);
cleanup:
    if (stADT != NULL)
        stADT->destroy(stADT);
    return retval;
}
