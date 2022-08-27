#include "set.h"
#include "sort.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int ncmp(void *x1, void *x2) {
    return ((long)x1 - (long)x2);
}

int scmp(void *x1, void *x2) {
    return strcmp((char *)x1, (char *)x2);
}

long nhash(void *x, long N) {
    return ((long)x) % N;
}

#define A 31L
long shash(void *x, long N) {
    int i;
    long sum = 0L;
    char *s = (char *)x;

    for (i = 0; s[i] != '\0'; i++)
        sum = A * sum + (long)s[i];
    return sum % N;
}

int main(int argc, char *argv[]) {
    int i;

    for (i = 1; i < argc; i++) {
        int test;
        sscanf(argv[i], "%d", &test);
        switch(test) {
          case 1: {
            printf("Test creation and destruction of a set ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            if (s != NULL) {
                printf("success\n");
                s->destroy(s, NULL);
            } else
                printf("failure\n");
            break;
          }
          case 2: {
            printf("Test addition of a single value ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            if (s->add(s, (void *)42))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, NULL);
            break;
          }
          case 3: {
            printf("Test addition of a duplicate value ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            (void)s->add(s, (void *)42);
            if (! s->add(s, (void *)42))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, NULL);
            break;
          }
          case 4: {
            printf("Test isEmpty() on an empty set ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            if (s->isEmpty(s))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, NULL);
            break;
          }
          case 5: {
            printf("Test isEmpty() on a non-empty set ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            (void)s->add(s, (void *)42);
            if (! s->isEmpty(s))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, NULL);
            break;
          }
          case 6: {
            printf("Test contains() on an empty set ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            if (! s->contains(s, (void *)42))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, NULL);
            break;
          }
          case 7: {
            printf("Test contains() on a non-empty set and value is present ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            (void)s->add(s, (void *)42);
            if (s->contains(s, (void *)42))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, NULL);
            break;
          }
          case 8: {
            printf("Test contains() on a non-empty set and value is not present ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            (void)s->add(s, (void *)42);
            if (! s->contains(s, (void *)99))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, NULL);
            break;
          }
          case 9: {
            printf("Test remove() on an empty set ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            if (! s->remove(s, (void *)99, NULL))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, NULL);
            break;
          }
          case 10: {
            printf("Test remove() on a non-empty set and value is present ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            (void)s->add(s, (void *)42);
            if (s->remove(s, (void *)42, NULL))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, NULL);
            break;
          }
          case 11: {
            printf("Test remove() on a non-empty set and value is not present ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            (void)s->add(s, (void *)42);
            if (! s->remove(s, (void *)99, NULL))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, NULL);
            break;
          }
          case 12: {
            printf("Test size() on an empty set ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            if (s->size(s) == 0L)
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, NULL);
            break;
          }
          case 13: {
            printf("Test size() on a non-empty set ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            (void)s->add(s, (void *)42);
            if (s->size(s) != 0L)
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, NULL);
            break;
          }
          case 14: {
            printf("Test addition of multiple, unique values ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            long values[] = {1, 2, 3, 4, 5};
            int i, success = 1;
            for (i = 0; i < 5; i++) {
                if (! s->add(s, (void *)values[i])) {
                    success = 0;
                    break;
                }
            }
            if (success && s->size(s) == 5L)
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, NULL);
            break;
          }
          case 15: {
            printf("Test toArray() ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            long len, values[] = {1, 2, 3, 4, 5};
            int i;
            long *array;
            for (i = 0; i < 5; i++)
                (void)s->add(s, (void *)values[i]);
            array = (long *)s->toArray(s, &len);
            if (array != NULL && len == 5L)
                printf("success\n");
            else
                printf("failure\n");
            free(array);
            s->destroy(s, NULL);
            break;
          }
          case 16: {
            printf("Test itCreate() ... ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            long values[] = {1, 2, 3, 4, 5};
            long v;
            int i;
            const Iterator *it;
            for (i = 0; i < 5; i++)
                (void)s->add(s, (void *)values[i]);
            it = s->itCreate(s);
            if (it != NULL) {
                i = 0;
                while (it->hasNext(it)) {
                    (void)it->next(it, (void **)&v);
                    i++;
                }
            }
            if (it != NULL && i == 5)
                printf("success\n");
            else
                printf("failure\n");
            if (it != NULL)
                it->destroy(it);
            s->destroy(s, NULL);
            break;
          }
          case 17: {
            printf("Show set elements sorted ");
            const Set *s = Set_create(0L, ncmp, 0.0, nhash);
            long len, values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
            long i;
            long *array;
            for (i = 0; i < 10; i++)
                (void)s->add(s, (void *)values[i]);
            array = (long *)s->toArray(s, &len);
            sort((void **)array, len, ncmp);
            for (i = 0; i < len; i++)
                printf("%ld ", array[i]);
            printf("... success\n");
            free(array);
            s->destroy(s, NULL);
            break;
          }
          default: {
            printf("Undefined test\n");
            break;
          }
        }
    }
}
