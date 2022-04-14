#include <err.h>
#include <errno.h>
#include <stdlib.h>

int
main(void)
{
	int ret;
	ret = daemon(0, 0);
#if defined(__EMSCRIPTEN__)
	/* fork() is not implemented and returns ENOSYS. */
	if (ret == -1 && errno != ENOSYS)
		exit(1);
#else
	if (ret == -1)
		exit(1);
#endif

#if defined(_WIN32) && !defined(__CYGWIN__) || defined(__EMSCRIPTEN__)
	/* The concept of a daemon is different on Windows. There's no forking.
	 *
	 * Emscripten doesn't have forking either.
	 */
	exit(0);
#else
	/* If we actually return 1, we haven't forked. */
	exit(1);
#endif
}
