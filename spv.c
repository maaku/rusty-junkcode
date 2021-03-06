#include <stdio.h>
#include <err.h>
#include <stdlib.h>
#include <ccan/isaac/isaac64.h>
#include <ccan/isaac/isaac64.c>

int main(int argc, char *argv[])
{
	struct isaac64_ctx isaac;
	int num, i, seed, *dist;

	if (argc < 2)
		errx(1, "Usage: %s <blockheight> [<seed>]\n"
		     "  Prints optimal compact SPV length to genesis", argv[0]);
	num = atoi(argv[1]);
	seed = atoi(argv[2] ? argv[2] : "0");
	isaac64_init(&isaac, (void *)&seed, sizeof(seed));

	dist = calloc(sizeof(*dist), num);
	for (i = 1; i < num; i++) {
		/* We can skip more if we're better than required. */
		uint64_t skip = -1ULL / isaac64_next_uint64(&isaac);
		int j, best;

		if (skip > i)
			skip = i;

		/* We can always get back there one step at a time. */
		best = i;
		for (j = i-1; j >= (int)(i-skip); j--)
			if (1 + dist[j] < best)
				best = 1 + dist[j];
		dist[i] = best;
		printf("%i: %u steps\n", i, best);
	}
	return 0;
}
