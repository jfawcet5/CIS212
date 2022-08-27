/*
 * Implementation of a set ADT using a hash table with bucket hashing
 *
 * Author: Joshua Fawcett
 *
 */

#include "set.h"  /* the .h file does NOT reside in /usr/local/include/ADTs */
#include <stdlib.h>
#define MAX_CAPACITY 134217728L
#define SENTINEL(sd, i) (sd->buckets[i]->sentinel)

typedef struct node {
	struct node *next;
	struct node *prev;
	void *value;
} Node;

typedef struct listhead { /* doubly linked nodes with a sentinel to 
			     simplify the add and remove methods */
	Node sentinel;
} ListHead;

typedef struct s_data {
	/* definitions of the data members of self */
	int (*cmpFxn)(void *m1, void *m2); /* Compare function provided by the user */
	long capacity;                     /* Number of buckets in the bucket array */
	long size;                         /* Number of elements currently in the set */
	double increment;                  /* How much to increment load when adding an element */
	long (* hash)(void *m, long N);    /* Hash function provided by the user */
	double load;                       /* Current load, use with loadFactor */
	double loadFactor;                 /* Used to determine when to resize bucket array */
	ListHead **buckets;                /* Array of listheads/buckets -- Bucket array */
} SData;

static void freeList(SData *sd) {
	/* Empty the bucket array by freeing each of the nodes in the 
	 * buckets and then freeing each of the buckets */
	for (long i = 0; i < sd->capacity; i++) {
		if (sd->buckets[i] != NULL) {
			Node *p = SENTINEL(sd, i).next;
			Node *q;
			while (p != &SENTINEL(sd, i)) {
				q = p->next;
				free(p);
				p = q;
			}
			free(sd->buckets[i]);
			sd->buckets[i] = NULL;
		}
	}
}

static void purge(SData *sd, void (*freeFxn)(void *member)) {
	/* implementation of the application of freeFxn to the set members */
	for (long i = 0; i < sd->capacity; i++) {
		if (sd->buckets[i] != NULL) {
			Node *p = SENTINEL(sd, i).next;
			for (; p != &SENTINEL(sd, i); p = p->next) {
				if (freeFxn != NULL) {
					freeFxn(p->value);
				}
			}
		}
	}
}

static void s_destroy(const Set *s, void (*freeFxn)(void *member)) {
	/* implement the destroy() method */
	SData *sd = (SData *)s->self;

	purge(sd, freeFxn);
	freeList(sd);
	free(sd->buckets);
	free(sd);
	free((void *)s);
}

static void s_clear(const Set *s, void (*freeFxn)(void *member)) {
	/* implement the clear() method */
	SData *sd = (SData *)s->self;
	purge(sd, freeFxn);
	freeList(sd);
	sd->size = 0L;
}

static void link(Node *before, Node *p, Node *after) {
	/* Add a node between 'before' and 'after' */
	p->next = after;
	p->prev = before;
	after->prev = p;
	before->next = p;
}

static void unlink(Node *p) {
	/* remove node p from bucket*/
	p->prev->next = p->next;
	p->next->prev = p->prev;
}

static int inSet(SData *sd, long index, void *member) {
	/* Helper function. Determines if 'member' is in the set */ 
	if (sd->buckets[index] != NULL) {
		for (Node *p = SENTINEL(sd, index).next; p != &SENTINEL(sd, index); p = p->next) {
			if (! sd->cmpFxn(p->value, member)) 
				return 1;
		}
	}
	return 0;
}

static void resize(SData *sd) {
	/* Increase the number of buckets in the bucket array and
	 * move the contents of the old bucket array into the new one */
	int N;
	long j;
	N = sd->capacity * 2;

	if (N > MAX_CAPACITY)
		return;
	ListHead **array = (ListHead **)malloc(N * sizeof(ListHead *));
	if (array == NULL) 
		return;
	for (j = 0; j < N; j++) {
		array[j] = NULL;
	}
	for (int i = 0; i < sd->capacity; i++) {
		if(sd->buckets[i] == NULL)
			continue;
		Node *p, *q;
		for (p = SENTINEL(sd, i).next; p != &SENTINEL(sd, i); p = q) {
			q = p->next;
			j = sd->hash(p->value, N);
			if (array[j] == NULL) {
				ListHead *lh = (ListHead *)malloc(sizeof(ListHead));
				link(&lh->sentinel, p, &lh->sentinel);
				array[j] = lh;
			}
			else {
				link(array[j]->sentinel.prev, p, &array[j]->sentinel);
			}
		}
		free(sd->buckets[i]);
	}
	free(sd->buckets);
	sd->buckets = array;
	sd->capacity = N;
	sd->load /= 2.0;
	sd->increment = 1.0 / N;
}

static int s_add(const Set *s, void *member) {
	/* implement the add() method */
	SData *sd = (SData *)s->self;
	long index;
	Node *new_node = (Node *)malloc(sizeof(Node));
	
	if (new_node != NULL) {
		new_node->value = member;
		if (sd->load >= sd->loadFactor)
			resize(sd);
		index = sd->hash(member, sd->capacity);
		if (inSet(sd, index, member)) {
			free(new_node);
			return 0;
		}
		if (sd->buckets[index] == NULL) {
			ListHead *lh = (ListHead *)malloc(sizeof(ListHead));
			link(&lh->sentinel, new_node, &lh->sentinel);
			sd->buckets[index] = lh;
		}
		else 
			link(SENTINEL(sd, index).prev, new_node, &SENTINEL(sd, index));
		sd->load += sd->increment;
		sd->size++;
	}
	return 1;
}

static int s_contains(const Set *s, void *member) {
	/* implement the contains() method */
	SData *sd = (SData *)s->self;
	long index = sd->hash(member, sd->capacity);
	return inSet(sd, index, member);
}

static int s_isEmpty(const Set *s) {
	/* implement the isEmpty() method */
	SData *sd = (SData *)s->self;
	return (sd->size == 0L);
}

static int s_remove(const Set *s, void *member, void (*freeFxn)(void *member)) {
	/* implement the remove() method */
	SData *sd = (SData *)s->self;
	long index = sd->hash(member, sd->capacity);
	int status = 0;

	if (inSet(sd, index, member)) {
		Node *p = SENTINEL(sd, index).next;
		while (p != &SENTINEL(sd, index)) {
			Node *q = p->next;
			if (p->value == member) {
				unlink(p);
				sd->size--;
				sd->load -= sd->increment;
				if (freeFxn != NULL)
					freeFxn(p->value);
				free(p);
				break;
			}
			p = q;
		}
		status = 1;
	}
	return status;
}

static long s_size(const Set *s) {
    /* implement the size() method */
	SData *sd = (SData *)s->self;
	return sd->size;
}

static void **toArray(SData *sd) {
	/* Helper function. Creates an array of void pointers 
	 * containing the values from each of the nodes in the
	 * bucket array
	 */
	long n = 0;
	Node *p;
	void **tmp = NULL;

	if (sd->size > 0) {
		tmp = (void **)malloc(sd->size * sizeof(void *));
		if (tmp != NULL) {
			for (long i = 0; i < sd->capacity; i++) {
				if (sd->buckets[i] != NULL) {
					p = SENTINEL(sd, i).next;
					for (; p != &SENTINEL(sd, i); p = p->next) {
						if (p != NULL) {
							tmp[n++] = p->value;
						}
					}
				}
			}
		}
	}
	return tmp;
}

static void **s_toArray(const Set *s, long *len) {
	/* implement the toArray() method */
	SData *sd = (SData *)s->self;
	void **tmp = toArray(sd);

	if (tmp != NULL) {
		*len = sd->size;
	}
	return tmp;
}

const Iterator *s_itCreate(const Set *s) {
	/* implement the itCreate() method */
	SData *sd = (SData *)s->self;
	const Iterator *it = NULL;
	void **tmp = (void **)toArray(sd);

	if (tmp != NULL) {
		it = Iterator_create(sd->size, tmp);
		if (it == NULL) {
			free(tmp);
		}
	}
	return it;
}

static Set template = {
    NULL, s_destroy, s_clear, s_add, s_contains, s_isEmpty, s_remove,
    s_size, s_toArray, s_itCreate
};

const Set *Set_create(long capacity, int (*cmpFxn)(void *m1, void *m2),
                      double loadFactor, long (*hashFxn)(void *m, long N)
                     )
{
	/* construct a Set instance and return to the caller */
	Set *s = (Set *)malloc(sizeof(Set));
	long N;
	double lf;
	ListHead **array;

	if (s != NULL) {
		SData *sd = (SData *)malloc(sizeof(SData));

		if (sd != NULL) {
			N = ((capacity > 0) ? capacity : DEFAULT_CAPACITY);
			if (N > MAX_CAPACITY) 
				N = MAX_CAPACITY;
			lf = ((loadFactor > 0.000001) ? loadFactor : DEFAULT_LOAD_FACTOR);

			array = (ListHead **)malloc(N * sizeof(ListHead *));
			if (array != NULL) {
				sd->cmpFxn = cmpFxn;
				sd->capacity = N;
				sd->increment = 1.0 / (double)N;
				sd->loadFactor = lf;
				sd->load = 0.0;
				sd->size = 0L;
				sd->hash = hashFxn;
				sd->buckets = array;
				for (long i = 0; i < N; i++) {
					array[i] = NULL;
				}
				*s = template;
				s->self = sd;
			}
			else {
				free(sd);
				free(s);
				s = NULL;
			}
		}
		else {
			free(s);
			s = NULL;
		}
	}
	return s;
}
