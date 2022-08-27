#include "jsstring.h"
#include <stdlib.h>
#include "jsheap.h"
#include <time.h>
#include <sys/time.h>
#include "Accumulator.h"

char *jsStrchr(const char *s, int c) {
    struct timeval t1, t2;
    static int init = 1;
    static Accumulator ac = {"jsStrchr", 0LL, 0LL};
    if (init) {
	    accumulators[nAccumulators++] = &ac;
	    init = 0;
    }

    (void)gettimeofday(&t1, NULL);
    char *result;
    result = strchr(s, c);
    (void)gettimeofday(&t2, NULL);

    ac.musecs += 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
    ac.calls += 1LL;
    return result;
}

int jsStrcmp(const char *s1, const char *s2) {
    struct timeval t1, t2;
    static int init = 1;
    static Accumulator ac = {"jsStrcmp", 0LL, 0LL};
    if (init) {
	    accumulators[nAccumulators++] = &ac;
	    init = 0;
    }

    (void)gettimeofday(&t1, NULL);
    int result;
    result = strcmp(s1, s2);
    (void)gettimeofday(&t2, NULL);

    ac.musecs += 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec); 
    ac.calls += 1LL;
    return result;
}

char *jsStrcpy(char *dest, const char *src) {
    struct timeval t1, t2;
    static int init = 1;
    static Accumulator ac = {"jsStrcpy", 0LL, 0LL};
    if (init) {
	    accumulators[nAccumulators++] = &ac;
	    init = 0;
    }

    (void)gettimeofday(&t1, NULL);
    char *result;
    result = strcpy(dest, src);
    (void)gettimeofday(&t2, NULL);

    ac.musecs += 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
    ac.calls += 1LL;
    return result;
}

char *jsStrdup(const char *s) {
    struct timeval t1, t2;
    static int init = 1;
    static Accumulator ac = {"jsStrdup", 0LL, 0LL};
    if (init) {
	    accumulators[nAccumulators++] = &ac;
	    init = 0;
    
    }

    (void)gettimeofday(&t1, NULL);
    char *result;
    {
    size_t n = jsStrlen(s) + 1;
    result = (char *)jsMalloc(n);
    strcpy(result, s);
    }
    (void)gettimeofday(&t2, NULL);

    ac.musecs += 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
    ac.calls += 1LL;
    return result;
}

size_t jsStrlen(const char *s) {
    struct timeval t1, t2;
    static int init = 1;
    static Accumulator ac = {"jsStrlen", 0LL, 0LL};
    if (init) {
	    accumulators[nAccumulators++] = &ac;
	    init = 0;
    }

    (void)gettimeofday(&t1, NULL);
    size_t result;
    result = strlen(s);
    (void)gettimeofday(&t2, NULL);

    ac.musecs += 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
    ac.calls += 1LL;
    return result;
}

int jsStrncmp(const char *s1, const char *s2, size_t n) {
    struct timeval t1, t2;
    static int init = 1;
    static Accumulator ac = {"jsStrncmp", 0LL, 0LL};
    if (init) {
	    accumulators[nAccumulators++] = &ac;
	    init = 0;
    }

    (void)gettimeofday(&t1, NULL);
    int result;
    result = strncmp(s1, s2, n);
    (void)gettimeofday(&t2, NULL);

    ac.musecs += 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
    ac.calls += 1LL;
    return result;
}

char *jsStrstr(const char *haystack, const char *needle) {
    struct timeval t1, t2;
    static int init = 1;
    static Accumulator ac = {"jsStrstr", 0LL, 0LL};
    if (init) {
	    accumulators[nAccumulators++] = &ac;
	    init = 0;
    }

    (void)gettimeofday(&t1, NULL);
    char *result;
    result = strstr(haystack, needle);
    (void)gettimeofday(&t2, NULL);

    ac.musecs += 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec); 
    ac.calls += 1LL;
    return result;
}
