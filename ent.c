#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "argparse.h"

/*#define PRINT_ALL*/

int calc_freq(uint8_t *str, int len, int *nsymbols)
{
	int ndif = 0;
	
	for (int i = 0; i < len; i++) {
		nsymbols[(uint8_t)str[i]]++;
	}
	for (int i = 0; i < 256; i++) {
		if (nsymbols[i] != 0)
			ndif++;
	}
	return ndif;
}

uint64_t get_file_size(FILE *f)
{
	fseek(f, 0, SEEK_END);
	uint64_t len = ftell(f);
	rewind(f);
	return len;
}

double calc_entropy(int *nsymbols, int size)
{
	double tmp;
	double entropy = 0.0f;
	for (int i = 0; i < 256; i++) {
		if (nsymbols[i] != 0) {
			tmp = (double)nsymbols[i] / size;
			entropy += tmp * log2(tmp);
#ifdef PRINT_ALL
			/*printf("%c (hex %x): %d\n", i, i, nsymbols[i]);*/
			printf("%c: %f\n", i, tmp);
#endif
		}
	}
	return -entropy;
}

int main(int argc, char **argv)
{
	struct _args args;
	args = parse_args(argc, argv);

	int nsymbols[256] = {0};
	double entropy = 0.0f;

	FILE *f = fopen(args.file, "rb");
	if (!f) {
		printf("fatal: error opening file\n");
		return 1;
	}

	int64_t len = get_file_size(f);
	uint8_t *bytes = malloc(args.chunksz);
	int chunklen = args.chunksz;

	int64_t chunkstart, chunkend;
	chunkstart = 0;

	while (len > 0) {
		chunklen = len > args.chunksz ? args.chunksz : len;
		len -= args.chunksz;

		chunkend = chunkstart + chunklen;

		fread(bytes, chunklen, 1, f);
		calc_freq(bytes, chunklen, nsymbols);

		entropy = calc_entropy(nsymbols, chunklen);
		memset(nsymbols, 0, 256 * sizeof(int));
		if (args.percent)
			printf("[%ld-%ld]: %f%%\n", chunkstart,
				chunkend, 100.0f * entropy / 8.0f);
		else
			printf("[%ld-%ld]: %f\n", chunkstart,
				chunkend, entropy);

		chunkstart = chunkend;
	}

	free(bytes);
	fclose(f);
}
