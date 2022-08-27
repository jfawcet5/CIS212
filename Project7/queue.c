/*
 * implementation for generic FIFO queue using a singly-linked list
 *
 * Author: Joshua Fawcett
 */

#include <stdio.h>
#include <stdlib.h>
#include "ADTs/queue.h"
#define UNUSED __attribute__((unused))

/* any other includes needed by your code */

typedef struct node {
	struct node *next;
	void *value;
}Node;

typedef struct q_data {
   Node *head;
   Node *tail;
   long count;
} QData;

static void purge(QData *qd, void (*freeFxn)(void *element)) {
        /* implementation of the application of freeFxn to the elements */
	Node *tmp = qd->head;
	for (long i = 0L; i < qd->count; i++) {
		if (freeFxn != NULL) {
		freeFxn(tmp->value);
		tmp = tmp->next;
	        }
	}
}

static void freeList(QData *qd) {
	Node *tmp;
	for (long i = 0; i < qd->count; i++) {
		tmp = qd->head->next;
		free(qd->head);
		qd->head = tmp;
	}
}

static void q_destroy(const Queue *q, void (*freeFxn)(void *element)) {
	/* implement the destroy() method */
	QData *qd = (QData *)q->self;
	
	purge(qd, freeFxn);
	freeList(qd);
	free(qd);
	free((void *)q);
}

static void q_clear(const Queue *q, void (*freeFxn)(void *element)) {
	/* implement the clear() method */
	QData *qd = (QData *)q->self;

	purge(qd, freeFxn);
	freeList(qd);
	qd->head = qd->tail = NULL;
}

static int q_enqueue(const Queue *q, void *element) {
	/* implement the enqueue() method */
	/* return whether it was successful or not */
	QData *qd = (QData *)q->self;

	Node *new_node = (Node *)malloc(sizeof(Node));
    
	int status = 0;
	if (new_node != NULL) {
	    	new_node->next = NULL;
	    	new_node->value = element;
	    	if (qd->count == 0) {
			    qd->head = qd->tail = new_node;
		    	qd->count++;
	    	}
	    	else {
		    qd->tail->next = new_node;
		    qd->tail = new_node;
		    qd->count++;
	    	}
	    	status = 1;
    	}
    	return status;
}

static int q_front(const Queue *q, void **element) {
	/* implement the front() method */
	/* return whether it was successful or not */
	QData *qd = (QData *)q->self;
	if (qd->count == 0) {
		return 0;
	}
	Node *p = qd->head;
	if (p != NULL) {
		*element = p->value;
	}
	return 1;
}

static int q_dequeue(const Queue *q, void **element) {
	/* implement the dequeue() method */
	/* return whether it was successful or not */
	QData *qd = (QData *)q->self;

	if (qd->count == 0) {
		return 0;
	}
	Node *p = qd->head;
	if (p != NULL) {
		*element = p->value;
		qd->head = p->next;
		free(p);
		qd->count -= 1L;
	}
	return 1;
}

static long q_size(const Queue *q) {
	/* implement the size() method */
	/* return the number of items in the queue */
	QData *qd = (QData *)q->self;
	return qd->count;
}

static int q_isEmpty(const Queue *q) {
	/* implement the isEmpty() method */
	/* return !0 if queue is empty, 0 if it has elements */
	QData *qd = (QData *)q->self;
	return (qd->count == 0L);
}

static void **toArray(QData *qd) {
	void **tmp = NULL;

	if (qd->count > 0L) {
		tmp = (void **)malloc(qd->count * sizeof(void *));
		if (tmp != NULL) {
			long i = 0;
			for (Node *p = qd->head; p != NULL; p = p->next) {
				if (p->value != NULL) {
					tmp[i] = p->value;
				}
				i++;
			}
		}
	}
	return tmp;
}

static void **q_toArray(const Queue *q, long *len) {
	/* implement the toArray() method */
	/* return a pointer to the array and the number of elements in *len */
	QData *qd = (QData *)q->self;
	void **tmp = toArray(qd);
	if (tmp != NULL) {
		*len = qd->count;
	}
	return tmp;
}

static const Iterator *q_itCreate(const Queue *q) {
	/* implement the itCreate() method */
	/* return a pointer to the iterator or NULL if error */
	QData *qd = (QData *)q->self;
	const Iterator *it = NULL;
	void **tmp = toArray(qd);

	if (tmp != NULL) {
		it = Iterator_create(qd->count, tmp);
		if (it == NULL) {
			free(tmp);
		}
	}
	return it;
}

static Queue template = {
    NULL, q_destroy, q_clear, q_enqueue, q_front, q_dequeue, q_size,
    q_isEmpty, q_toArray, q_itCreate
};

const Queue *Queue_create(UNUSED long capacity) {
    /* construct a Queue instance and return to the caller */
    Queue *q = (Queue *)malloc(sizeof(Queue));
    
    if (q != NULL) {
        QData *qd = NULL;
        qd = (QData *)malloc(sizeof(QData));
        if (qd != NULL) {
	        qd->count = 0;
	        qd->head = qd->tail = NULL;
		*q = template;
		q->self = qd;
        }
	else {
		free(q);
		q = NULL;
	}
    }
    return q;
}
