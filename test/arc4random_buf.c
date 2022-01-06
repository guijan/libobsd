#include <err.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

enum {
	BUFLEN = 1024
};

int
main(void)
{
	const unsigned char zeroes[BUFLEN] = {0};
	unsigned char _buf[BUFLEN * 3] = {0};
	unsigned char *buflow = _buf;
	unsigned char *bufmid = buflow + BUFLEN;
	unsigned char *bufhigh = bufmid + BUFLEN;

	arc4random_buf(bufmid, BUFLEN);

	/* Careful, the first test is different. */
	if (memcmp(bufmid, zeroes, BUFLEN) == 0)
		errx(1, "memcmp(bufmid, zeroes, BUFLEN) == 0");

	if (memcmp(buflow, zeroes, BUFLEN) != 0)
		errx(1, "memcmp(buflow, zeroes, BUFLEN) != 0");
	if (memcmp(bufhigh, zeroes, BUFLEN) != 0)
		errx(1, "memcmp(bufhigh, zeroes, BUFLEN) != 0");

	/* XX: Add randomness analysis and check for thread and fork safety. */
	return (0);
}
