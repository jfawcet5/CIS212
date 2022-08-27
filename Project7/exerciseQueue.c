/* exerciseQueue Solution
 *
 * Author: Joshua Fawcett
 *
 * Usage: ./exerciseQueue <FILE>
 *
 */
#include <stdio.h>
#include <string.h>
#include "ADTs/queue.h"

void printQueue(const Queue *q, FILE *fp) {
	int i = 0;
	long element;
	const Queue *q2 = Queue_create(0L);
	
	if (q2 == NULL) {
		printf("Unable to create Queue ADT instance\n");
		return;
	}
	if (q->isEmpty(q)) {
		fprintf(fp, "Empty\n");
		q2->destroy(q2, NULL);
		return;
	}
	while (! q->isEmpty(q)) {
		q->dequeue(q, (void **)&element);
		printf("%s%ld", (i == 0) ? "" : " ", element);
		q2->enqueue(q2, (void *)element);
		i++;
	}
	while (! q2->isEmpty(q2)) {
		q2->dequeue(q2, (void **)&element);
		q->enqueue(q, (void *)element);
	}
	q2->destroy(q2, NULL);
	printf("\n");
}

void exerciseQueue(const Queue *q, FILE *fp) {
	char buf[BUFSIZ];
	int N, i;

	(void)fgets(buf, BUFSIZ, fp);
	(void)sscanf(buf, "%d", &N);
	for (i = 0; i < N; i++) {
		(void)fgets(buf, BUFSIZ, fp);
		if (strstr(buf, "enqueue") != NULL) {
			long eq;
			sscanf(buf + 8, "%ld", &eq);
			(void)q->enqueue(q, (void *)eq);
		}
		if (strstr(buf, "dequeue") != NULL) {
			long dq;
			if (! q->dequeue(q, (void **)&dq)) {
				printf("QueueError\n");
				continue;
			}
			else
				printf("%ld\n", dq);
		}
		if (strstr(buf, "print") != NULL) {
			printQueue(q, stdout);
		}
	}
}

int main(int argc, char *argv[]) {
	const Queue *q = Queue_create(0L);
	int status = 1;
	FILE *fp = NULL;
	
	if (argc != 2) {
		fprintf(stderr, "USAGE: %s <FILE>\n", argv[0]);
		goto cleanup;
	}

	if ((fp = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "%s: %s: No such file or directory\n", argv[0], argv[1]);
		goto cleanup;
	}

	if (q  == NULL) {
		fprintf(stderr, "%s: Cannot create Queue ADT instance\n", argv[0]);
		goto cleanup;
	}
	exerciseQueue(q, fp);
	status = 0;
cleanup:
	if (fp != NULL) 
		fclose(fp);
	if (q != NULL)
		q->destroy(q, NULL);
	return status;	
}
