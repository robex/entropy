#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "argparse.h"

void usage()
{
	printf("usage: ./ent [-sn] [file]\n");
	printf("\noptional arguments:\n");
	printf("\t-s [size]: chunk size (default 4096)\n");
	printf("\t-n (no-percentage): show entropy as number in bits (8 = max entropy)\n");
	fflush(stdout);
}

struct _args parse_args(int argc, char **argv)
{
	struct _args args;
	// default values
	args.chunksz = 4096;
	args.percent = 1;

	// remove program name
	argc--;
	argv++;

	if (argc < 1) {
		usage();
		_Exit(0);
	}

	while (argc >= 2) {
		if (!strcmp(argv[0], "-s")) {
			argc--;
			argv++;
			args.chunksz = atoi(argv[0]);
		} else if (!strcmp(argv[0], "-n")) {
			args.percent = 0;
		}
		argc--;
		argv++;
	}

	if (args.chunksz < 1) {
		printf("error: chunk size must be above 0\n");
		_Exit(0);
	}
	args.file = argv[0];
	return args;
}
