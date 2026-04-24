#include "hsh.h"
/**
 * exec_cmd - fork and execute a command
 * @args: array of arguments (args[0] is the command path)
 * @argv: argument vector of the shell (for error messages)
 */
void exec_cmd(char **args, char **argv)
{
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		perror(argv[0]);
		return;
	}
	if (pid == 0)
	{
		if (execve(args[0], args, environ) == -1)
		{
			fprintf(stderr, "%s: 1: %s: not found\n",
				argv[0], args[0]);
			exit(1);
		}
	}
	else
	{
		wait(NULL);
	}
}
