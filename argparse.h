#ifndef ARGPARSE_H
#define ARGPARSE_H

struct _args {
	int chunksz;
	short percent;
	char *file;
};

struct _args parse_args(int argc, char **argv);

#endif
