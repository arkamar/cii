#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TIME_FMT "%F %T"

int
main(int argc, char * argv[]) {
	char * buf = NULL;
	size_t size = 0;
	time_t timestmap;
	char tbuf[32];
	char * endptr;

	while (getline(&buf, &size, stdin) > 0) {
		timestmap = strtoul(buf, &endptr, 0);
		strftime(tbuf, sizeof tbuf, TIME_FMT, localtime(&timestmap));
		fputs(tbuf, stdout);
		fputs(endptr, stdout);
		fflush(stdout);
	}

	free(buf);

	return 0;
}
