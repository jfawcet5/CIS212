/* exerciseStack solution
 * 
 * Author: Joshua Fawcett
 *
 * Usage: ./exerciseStack <FILE>
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ADTs/stack.h"

void printStack(const Stack *st, FILE *fp) {
	const Stack *st2 = Stack_create(0L);
	long lvalue;
	int i = 0;

	if (st->isEmpty(st)) {
		fprintf(fp, "Empty\n");
		st2->destroy(st2, free);
		return;
	}
	while (! st->isEmpty(st)) {
		st->pop(st, (void **)&lvalue);
		fprintf(fp, "%s%ld", (i == 0) ? "" : " ", lvalue);
		st2->push(st2, (void *)lvalue);
		i++;
	}
	while (! st2->isEmpty(st2)) {
		st2->pop(st2, (void **)&lvalue);
		st->push(st, (void *)lvalue);
	}
	st2->destroy(st2, free);
	printf("\n");
}

int main(int argc, char *argv[]) {
	const Stack *st = Stack_create(0L);
	char buf[BUFSIZ];
	long lvalue;

	if (argc != 2) {
		st->destroy(st, free);
		printf("Usage: ./exerciseStack FILE\n");
		return 1;
	}

	FILE *f = fopen(argv[1], "r");
	if (f != NULL) {
		while (fgets(buf, BUFSIZ, f) != NULL) {
			if (strstr(buf, "push") != NULL) {
				sscanf(buf + 5, "%ld", &lvalue);
				st->push(st, (void *)lvalue);
			}
			else if (strstr(buf, "pop") != NULL) {
				if (! st->isEmpty(st)) {
					st->pop(st, (void **)&lvalue);
					printf("%ld\n", lvalue);
				}
				else {
					printf("StackError\n");
				}
			}
			else if (strstr(buf, "print") != NULL) {
				printStack(st, stdout);
			}
		}
		fclose(f);
	}
	else {
		fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[1]);
	}
	st->destroy(st, free);
	return 0;
}
