/*
 * All the yes(1) implementation you would ever want.
 *
 * When building, define what you'd like to have:
 *
 * TEE:
 *   Make `tee` swallow it's own tail.
 *
 * TAIL:
 *   Make sure the compiler does it's tail-call optimisations,
 *   and the reader understands that C ain't C++.
 *
 * NOBODY:
 *   Extension on TAIL, that will never reach the body of main(). This relies on
 *   a fairly common implementation specified prototype for main() which takes
 *   additional `char **envp` argument, in order to have access to `argv`.
 *
 * JAVA:
 *   Extension on NOBODY, that aspires to become Java when it grows up. However,
 *   it's a bit shy about making its ambitions public.
 */

#if TEE

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	char *expletive = argc > 1 ? argv[1] : "y";
	char *cmd[] = { "tee", "/dev/stderr", NULL };
	int io[2];
	if (pipe(io) != 0)
		return 1;
	write(io[1], expletive, strlen(expletive));
	write(io[1], "\n", 1);
	dup2(io[0], 0);
	dup2(io[1], 2);
	execvp(cmd[0], cmd);
	return 1;
}

#elif TAIL

#include <stdio.h>

int main(int argc, char **argv)
{
	return argc < 0 || main(puts(argv[1] ? argv[1] : "y"), argv);
}

#elif NOBODY

#include <stdio.h>

int main(int, char **y, char **);
int main(int, char **y, char *[main(puts(y[1] ? y[1] : "y"), y, y)]){}

#elif JAVA

#include <stdio.h>

int main(int, char **y, char **);
int main(int, char **y, char *[
static! (void*) main(puts(y[1] ? y[1] : "y"), y, y)]){}

#else

#include <stdio.h>

int main(void)
{
	return puts("no");
}

#endif
