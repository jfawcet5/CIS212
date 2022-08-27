typedef struct accumulator {
	char *name;
	long long calls;
	long long musecs;
} Accumulator;

extern long nAccumulators;
extern Accumulator *accumulators[1000];
