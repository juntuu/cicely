#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>

int pipe_repeat(int in, int out, int n, char* cmd, char** argv)
{
	if (n < 1)
		return 0;

	int child_stat = 0;

	if (n > 1) {
		int io[2];
		if (pipe(io) != 0)
			return 0;
		child_stat = pipe_repeat(in, io[1], n-1, cmd, argv);
		if (child_stat == 0)
			return 0;
		in = io[0];
	}

	pid_t pid = fork();
	if (pid == 0) {
		dup2(in, 0);
		dup2(out, 1);
		execvp(cmd, argv);
		_exit(1);
	}
	close(in);
	close(out);
	return (pid != -1) + child_stat;
}

int wait_n(int n)
{
	int res = 0;
	for (;n > 0; n--) {
		int stat = 0;
		wait(&stat);
		if (WIFEXITED(stat))
			res |= WEXITSTATUS(stat);
		else if (WIFSIGNALED(stat))
			res |= WTERMSIG(stat);
		else
			res |= -1;
	}
	return res;
}

int proc_limit(int ceil)
{
	struct rlimit nproc;
	if (getrlimit(RLIMIT_NPROC, &nproc) == 0) {
		// fprintf(stderr, "soft: %llu, hard: %llu\n", nproc.rlim_cur, nproc.rlim_max);
		if (nproc.rlim_cur < (unsigned)ceil)
			ceil = (int) nproc.rlim_cur;
	}
	return ceil;
}

int main(int argc, char **argv)
{
	if (argc < 3 || *argv[1] == '\0') {
		fprintf(stderr,
				"usage: %s <count> <program> [args...]\n"
				"  count      how many times to repeat the program\n"
				"  program    program to run\n"
				"  args       arguments to be passed to the program\n"
				"\n"
				"For example the following lines are equivalent:\n"
				"  %s 3 cat -n\n"
				"  cat -n | cat -n | cat -n\n",
				argv[0], argv[0]);
		return 1;
	}

	char* end;
	long count = strtol(argv[1], &end, 0);
	if (count < 1 || *end != '\0') {
		fprintf(stderr, "error: %s: count must be positive number (got: %s)\n", argv[0], argv[1]);
		return 1;
	}

	int max_lim = proc_limit(INT_MAX);
	if (count >= max_lim) {
		fprintf(stderr, "error: %s: count is too big (maximum is: <%d)\n", argv[0], max_lim);
		return 1;
	}

	char *cmd = argv[2];
	char **args = argv+2;

	int spawned = pipe_repeat(STDIN_FILENO, STDOUT_FILENO, (int)count, cmd, args);
	int status = wait_n(spawned);
	if (spawned != count)
		fprintf(stderr, "error: only %d / %ld pieces of the pipe could be created\n", spawned, count);
	return status;
}
