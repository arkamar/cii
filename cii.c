/* See LICENSE file for copyright and license details.
 *
 * Purpose of this program is to colorize output of ii connected to
 * bitlbee. It also filters out join statuses.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct colors colors;
struct colors {
	char *cs;
	char *ce;
};

static char *argv0;
static char *nick = NULL;
static char colorother[]   = "\x1b[1;31m";
static char colorrootend[] = "\x1b[0;32m";
static char colorroot[]    = "\x1b[1;33m";
static char colornick[]    = "\x1b[1;34m";
static char colorreset[]   = "\x1b[0m";

static const colors color[] = {
	{ colornick,   colorother },
	{ colorother,  colornick },
	{ colorroot,   colorrootend },
};

static void
usage(void) {
	fprintf(stderr, "usage: %s [nick]\n", argv0);
	exit(1);
}

static const colors *
get_type(const char *str) {
	if (!strncmp(str, "root", 4) && str[4] == '>')
		return &color[2];
	else if (!strncmp(str, nick, strlen(nick)) && str[strlen(nick)] == '>')
		return &color[0];
	else
		return &color[1];
}

int
main(int argc, char *argv[]) {
	argv0 = argv[0];
	FILE *fp = stdin;
	static char *buf = NULL;
	static size_t size = 0;
	nick = getenv("USER");
	/* TODO: add parameter for private chate (withou colons) */
	/* TODO: read from file */
	if (argc == 2)
		nick = argv[1];
	if (!strlen(nick)) {
		fprintf(stderr, "Error: nick name size\n");
		exit(1);
	}
	if (argc > 2)
		usage();

	while (getline(&buf, &size, fp) > 0) {
		char *cb = NULL;
		char *col = NULL;
		char *ob = strstr(buf, "<");
		if (ob) {
			cb = strstr(ob + 1, ">");
			if (!cb)
				continue;
			col = strstr(cb + 1, ":");
			const colors *color = get_type(ob + 1);
			*cb = *ob = '\0';
			printf("%s%s<%s>", buf, color->cs, ob + 1);
			if (col) {
				*col = '\0';
				printf("%s%s%s:%s", color->ce, cb + 1, colorreset, col + 1);
			} else {
				printf("%s%s", colorreset, cb + 1);
			}
		}
	}
	return EXIT_SUCCESS;
}
