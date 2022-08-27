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
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            if (s != NULL) {
                printf("success\n");
                s->destroy(s, free);
            } else
                printf("failure\n");
            break;
          }
          case 2: {
            printf("Test addition of a single value ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            if (s->add(s, (void *)strdup("42")))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, free);
            break;
          }
          case 3: {
            printf("Test addition of a duplicate value ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            char *sp = strdup("42");
            (void)s->add(s, (void *)sp);
            if (! s->add(s, (void *)sp))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, free);
            break;
          }
          case 4: {
            printf("Test isEmpty() on an empty set ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            if (s->isEmpty(s))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, free);
            break;
          }
          case 5: {
            printf("Test isEmpty() on a non-empty set ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            (void)s->add(s, (void *)strdup("42"));
            if (! s->isEmpty(s))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, free);
            break;
          }
          case 6: {
            printf("Test contains() on an empty set ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            if (! s->contains(s, "42"))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, free);
            break;
          }
          case 7: {
            printf("Test contains() on a non-empty set and value is present ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            (void)s->add(s, (void *)strdup("42"));
            if (s->contains(s, "42"))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, free);
            break;
          }
          case 8: {
            printf("Test contains() on a non-empty set and value is not present ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            (void)s->add(s, (void *)strdup("42"));
            if (! s->contains(s, "99"))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, free);
            break;
          }
          case 9: {
            printf("Test remove() on an empty set ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            if (! s->remove(s, "99", free))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, free);
            break;
          }
          case 10: {
            printf("Test remove() on a non-empty set and value is present ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            (void)s->add(s, (void *)strdup("42"));
            if (s->remove(s, "42", free))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, free);
            break;
          }
          case 11: {
            printf("Test remove() on a non-empty set and value is not present ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            (void)s->add(s, (void *)strdup("42"));
            if (! s->remove(s, "99", free))
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, free);
            break;
          }
          case 12: {
            printf("Test size() on an empty set ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            if (s->size(s) == 0L)
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, free);
            break;
          }
          case 13: {
            printf("Test size() on a non-empty set ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            (void)s->add(s, (void *)strdup("42"));
            if (s->size(s) != 0L)
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, free);
            break;
          }
          case 14: {
            printf("Test addition of multiple, unique values ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            char *values[] = {"1", "2", "3", "4", "5"};
            int i, success = 1;
            for (i = 0; i < 5; i++) {
                if (! s->add(s, (void *)strdup(values[i]))) {
                    success = 0;
                    break;
                }
            }
            if (success && s->size(s) == 5L)
                printf("success\n");
            else
                printf("failure\n");
            s->destroy(s, free);
            break;
          }
          case 15: {
            printf("Test toArray() ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            char *values[] = {"1", "2", "3", "4", "5"};
            long len;
            int i;
            char **array;
            for (i = 0; i < 5; i++)
                (void)s->add(s, (void *)strdup(values[i]));
            array = (char **)s->toArray(s, &len);
            if (array != NULL && len == 5L)
                printf("success\n");
            else
                printf("failure\n");
            free(array);
            s->destroy(s, free);
            break;
          }
          case 16: {
            printf("Test itCreate() ... ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            char *values[] = {"1", "2", "3", "4", "5"};
            char *sp;
            int i;
            const Iterator *it;
            for (i = 0; i < 5; i++)
                (void)s->add(s, (void *)strdup(values[i]));
            it = s->itCreate(s);
            if (it != NULL) {
                i = 0;
                while (it->hasNext(it)) {
                    (void)it->next(it, (void **)&sp);
                    i++;
                }
            }
            if (it != NULL && i == 5)
                printf("success\n");
            else
                printf("failure\n");
            if (it != NULL)
                it->destroy(it);
            s->destroy(s, free);
            break;
          }
          case 17: {
            printf("Show set elements sorted ");
            const Set *s = Set_create(0L, scmp, 0.0, shash);
            long len, i;
            char *values[] = {" 1", " 2", " 3", " 4", " 5", " 6",
                                  " 7", " 8", " 9", "10" };
            char **array;
            for (i = 0; i < 10; i++)
                (void)s->add(s, (void *)strdup(values[i]));
            array = (char **)s->toArray(s, &len);
            sort((void **)array, len, scmp);
            for (i = 0; i < len; i++)
                printf("%s ", array[i]);
            printf("... success\n");
            free(array);
            s->destroy(s, free);
            break;
          }
          default: {
            printf("Undefined test\n");
            break;
          }
        }
    }
}
