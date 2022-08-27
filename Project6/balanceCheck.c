/* balanceCheck solution
 *
 * Author: Joshua Fawcett
 * 
 * Usage: ./balanceCheck <FILE>
 */

#include <stdio.h>
#include <string.h>
#include "ADTs/stack.h"

int balanceCheck(const Stack *st, FILE *fp) {
	char exp[BUFSIZ];
	char balanced[4];
	int i;

	fgets(exp, BUFSIZ, fp);
	while (fgets(exp, BUFSIZ, fp) != NULL) {
		st->clear(st, NULL);
		strcpy(balanced, "YES");
		for (i = 0; exp[i] != '\0'; i++) {
			char sym = exp[i];
			long s;
			if (sym == '<' || sym == '[' || sym == '{' || sym == '(') {
				st->push(st, (void *)(long)sym);
			}
			else if (sym == '>' || sym == ']' || sym == '}' || sym == ')') {
				st->pop(st, (void **)&s);
				if (sym == '>' && s != '<') {
					strcpy(balanced, "NO");
					break;
				}
				if (sym == ']' && s != '[') {
					strcpy(balanced, "NO");
					break;
				}
				if (sym == '}' && s != '{') {
					strcpy(balanced, "NO");
					break;
				}
				if (sym == ')' && s != '(') {
					strcpy(balanced, "NO");
					break;
				}
			}
		}
		if (((i - 1) % 2) != 0)
			strcpy(balanced, "NO");
		printf("%s\n", balanced);
	}
	return 0;
}

int main(int argc, char *argv[]) { //check if brackets are balanced
	const Stack *st = Stack_create(0L);
	int status = 0;

	if (argc != 2) {
		fprintf(stderr, "Usage: ./balanceCheck FILE\n");
		status = 1;
		goto cleanup;
	}

	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[1]);
		status = 1;
		goto cleanup;
	}
	balanceCheck(st, fp);
cleanup:
	if (fp != NULL) {
		fclose(fp);
	}
	st->destroy(st, NULL);
	return status;
}
