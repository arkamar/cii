/* See LICENSE file for copyright and license details.
 *
 * Purpose of this program is to colorize output of ii connected to
 * bitlbee. It also filters out join statuses.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "arg.h"

#define TIME_FMT "%F %T"

typedef struct colors colors;
struct colors {
	char *cs;
	char *ce;
};

char *argv0;
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
	fprintf(stderr, "usage: %s [-cj] [-n nick]\n", argv0);
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
	FILE *fp = stdin;
	static char *buf = NULL;
	static size_t size = 0;
	int channel = 0, join = 0;
	char * ob;

	nick = getenv("USER");

	ARGBEGIN {
	case 'c':
		channel = 1;
		break;
	case 'j':
		join = 1;
		break;
	case 'n':
		nick = EARGF(usage());
		break;
	default:
		usage();
		break;
	} ARGEND;

	if (!strlen(nick)) {
		fprintf(stderr, "Error: nick name size\n");
		usage();
	}

	if (argc > 0)
		usage();

	while (getline(&buf, &size, fp) > 0) {
		char *cb = NULL;
		char *col = NULL;
		time_t timestamp = strtoul(buf, NULL, 0);
		char tbuf[32];
		if (join && (ob = strstr(buf, "-!-"))) {
			*ob = '\0';
			strftime(tbuf, sizeof tbuf, TIME_FMT, localtime(&timestamp));
			printf("%s %s-!-%s%s", tbuf, colornick, colorreset, ob + 3);
			continue;
		}
		ob = strchr(buf, '<');
		if (ob) {
			cb = strchr(ob + 1, '>');
			if (!cb)
				continue;
			col = strchr(cb + 1, ':');
			const colors *color = get_type(ob + 1);
			*cb = *ob = '\0';
			strftime(tbuf, sizeof tbuf, TIME_FMT, localtime(&timestamp));
			printf("%s %s<%s>", tbuf, color->cs, ob + 1);
			if (col && !channel) {
				*col = '\0';
				printf("%s%s%s:%s", color->ce, cb + 1, colorreset, col + 1);
			} else {
				printf("%s%s", colorreset, cb + 1);
			}
		}
	}
	free(buf);
	return EXIT_SUCCESS;
}
