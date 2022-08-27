/* wordperline.c working solution
 * 
 * Author: Joshua Fawcett
 *
 * Usage: ./wordperline [-lp] [FILE ...]
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "stringADT.h"

typedef struct FileTuple{
	FILE *file;
	char fname[2048];
} fileTuple;

int wordperline(int lower, int punct, fileTuple files[]) {
	int i;
	char buf[BUFSIZ];

	for (i = 0; files[i].file != NULL; i++) {
		for (int x = 0; fgets(buf, BUFSIZ, files[i].file) != NULL; x++) {
			const String *st = String_create(buf);
			if (lower) {
				//convert upper to lower
				st->lower(st);
			}
			if (punct) {
				//replace punctuation characters with whitespace
				for (int y = 0; y < st->len(st); y++) {
					char ch = st->convert(st)[y];
					if (ispunct(ch)) {
						st->assign(st, ' ', y);
					}
				}	
			}
			st->strip(st);
			if (st->len(st) == 0) {
				st->destroy(st);
				continue;
			}
			const ArrayList *a1 = st->split(st, "");
			for (long z = 0; z < a1->size(a1); z++) {
				char *sp;
				(void) a1->get(a1, z, (void **)&sp);
				printf("%s\n", sp);
			}
			a1->destroy(a1, free);
			st->destroy(st);
		}
		if (files[i].fname != "") {
			fclose(files[i].file);
		}
	}
	return 0;
}

int main(int argc, char *argv[]) {
	int lower = 0; /* -l flag. Converts upper-case characters to lower-case */
	int punct = 0; /* -p flag. Punctuation characters also separate words */
	int i, fc = 0;
	fileTuple files[512] = { NULL, "" };

	for (i = 1; i <= (argc - 1); i++) {
		if (argv[i][0] == '-') {
			// Check for illegal flags: ex. -llll -p are legal, -lx -m are not
			for (int x = 1; x < strlen(argv[i]); x++){
				if ((argv[i][x] != 'l') && (argv[i][x] != 'p')){
					fprintf(stderr, "Illegal flag: %s\n", argv[i]);
					return 1;
				}
			}
			if (strstr(argv[i], "l") != NULL) {
				lower = 1;
			}
			if (strstr(argv[i], "p") != NULL) {
				punct = 1;
			}
		}
		else {
			FILE *fd = fopen(argv[i], "r");
			if (fd != NULL) {
				files[fc].file = fd;
				strcpy(files[fc++].fname, argv[i]);
			}
			else {
				fprintf(stderr, "./wordperline: %s: No such file or directory\n", argv[i]);
				return 1;
			}	
		}
	}
	if (fc == 0) {
		files[fc].file = stdin;
		strcpy(files[fc].fname, "");
	}
	wordperline(lower, punct, files);
	return 0;
}
