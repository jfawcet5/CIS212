#include "jsheap.h"
#include "Accumulator.h"
#include <time.h>
#include <sys/time.h>

void *jsMalloc(size_t nBytes) {
    struct timeval t1, t2;
    static int init = 1;
    static Accumulator ac = {"jsMalloc", 0LL, 0LL};

    if (init) {
	    accumulators[nAccumulators++] = &ac;
	    init = 0;
    }

    (void)gettimeofday(&t1, NULL);
    void *result;
    result = malloc(nBytes);
    (void)gettimeofday(&t2, NULL);
    
    ac.musecs += 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
    ac.calls += 1LL;
    return result;
}

void jsFree(void *heapChunk) {
    struct timeval t1, t2;
    static int init = 1;
    static Accumulator ac = {"jsFree", 0LL, 0LL};

    if (init) {
	    accumulators[nAccumulators++] = &ac;
	    init = 0;
    }

    (void)gettimeofday(&t1, NULL);
    free(heapChunk);
    (void)gettimeofday(&t2, NULL);
    
    ac.musecs += 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
    ac.calls += 1LL;
}
