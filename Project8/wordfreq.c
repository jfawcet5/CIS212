/*
 * wordfreq solution
 *
 * Author: Joshua Fawcett
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ADTs/stringADT.h"
#include "ADTs/arraylist.h"
#include "ADTs/cskmap.h"
#include "sort.h"
#define UNUSED __attribute__((unused))

int keycmp(void *x1, void *x2) {
	MEntry *m1 = (MEntry *)x1;
	MEntry *m2 = (MEntry *)x2;
	return strcmp((char *)cskmentry_key(m1), (char *)cskmentry_key(m2));
}

int keycmpi(void *x1, void *x2) {
	MEntry *m1 = (MEntry *)x1;
	MEntry *m2 = (MEntry *)x2;
	return strcmp((char *)cskmentry_key(m2), (char *)cskmentry_key(m1));
}

int valcmp(void *x1, void *x2) {
	MEntry *m1 = (MEntry *)x1;
	MEntry *m2 = (MEntry *)x2;
	return (long)cskmentry_value(m2) - (long)cskmentry_value(m1);
}

int valcmpi(void *x1, void *x2) {
	MEntry *m1 = (MEntry *)x1;
	MEntry *m2 = (MEntry *)x2;
	return (long)cskmentry_value(m1) - (long)cskmentry_value(m2);
}

void wordfreq(const Map *m, int aFlag, int fFlag, int iFlag) {
	long sz = m->size(m);
	MEntry **mArray = m->entryArray(m, &sz);
	if (iFlag) {
		if (aFlag)
			sort((void **)mArray, sz, keycmpi);
		if (fFlag)
			sort((void **)mArray, sz, valcmpi);
	}
	else {
		if (aFlag)
			sort((void **)mArray, sz, keycmp);
		if (fFlag)
			sort((void **)mArray, sz, valcmp);
	}
	for (long i = 0; i < sz; i++) {
		long l = (long)cskmentry_value(mArray[i]);
		char *s = (char *)cskmentry_key(mArray[i]);
		printf("%8ld %s\n", l, s);
	}
	free(mArray);
}

void fillMap(const Map *m, FILE *fp, int lFlag, int pFlag) {
	char buf[BUFSIZ];
	const String *st = String_create("");

	while (fgets(buf, BUFSIZ, fp) != NULL) {
		const ArrayList *al;
		if (pFlag) {
			for (int i = 0; buf[i] != '\0'; i++) {
				if (ispunct(buf[i]))
					buf[i] = ' ';
			}
		}
		st->append(st, buf);
		if (lFlag) 
			st->lower(st);
		al = st->split(st, "");
		if (al == NULL)
			continue;
		for (long i = 0; i < al->size(al); i++) {
			char *s;
			(void) al->get(al, i, (void **)&s);
			long value;
			if (m->containsKey(m, s)) {
				m->get(m, s, (void *)&value);
				value++;
			}
			else {
				value = 1;
			}
			long value2 = 0;
			m->put(m, s, (void *)value, (void *)&value2);
		}
		st->clear(st);
		al->destroy(al, free);
	}
	st->destroy(st);
}

int main(int argc, char *argv[]) {
	int i, retval = 1, aFlag = 0, fFlag = 0, iFlag = 0, lFlag = 0, pFlag = 0;
	FILE *fp = NULL;
	const Map *m = CSKMap_create(0L, 0.0);
	if (m == NULL) 
		goto cleanup;

	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			for (int j = 1; argv[i][j] != '\0'; j++) {
				if (argv[i][j] == 'a') //sort alphabetically
					aFlag = 1;
				else if (argv[i][j] == 'f') //sort high frequency to low frequency
					fFlag = 1;
				else if (argv[i][j] == 'i') //invert the printed order (only works with -a or -f)
					iFlag = 1;
				else if (argv[i][j] == 'l') //convert upper to lower
					lFlag = 1;
				else if (argv[i][j] == 'p') //punctuation separates words
					pFlag = 1;
				else {
					fprintf(stderr, "Illegal option: %s\n", argv[i]);
					goto cleanup;
				}
			}
		}
		else {
			break;
		}
	}
	if (aFlag && fFlag) {
		fprintf(stderr, "Flag error: Cannot use -a and -f together\n");
		goto cleanup;
	}
	if (iFlag && !(aFlag || fFlag)) {
		fprintf(stderr, "Flag error: Cannot use -i without -f or -a\n");
		goto cleanup;
	}
	if (i >= argc) 
		fillMap(m, stdin, lFlag, pFlag);

	else {
		for (; i < argc; i++) {
			fp = fopen(argv[i], "r");
			if (fp == NULL) {
				fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[i]);
				goto cleanup;
			}
			fillMap(m, fp, lFlag, pFlag);
			fclose(fp);
		}
	}
	wordfreq(m, aFlag, fFlag, iFlag);
	retval = 0;
cleanup:
	if (m != NULL)
		m->destroy(m, NULL);
	return retval;
}
