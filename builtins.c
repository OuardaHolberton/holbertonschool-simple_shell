#include "hsh.h"

/**
 * handle_env - print all environment variables
 */
void handle_env(void)
{
	int i;

	i = 0;
	while (environ[i])
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

/**
 * handle_exit - exit the shell properly
 * @args: array of arguments
 * @line: the input line to free
 */
void handle_exit(char **args, char *line)
{
	free(line);
	free(args);
	exit(0);
}

/**
 * handle_builtins - check and execute builtins
 * @args: array of arguments
 * @line: the input line to free
 * Return: 1 if builtin executed, 0 otherwise
 */
int handle_builtins(char **args, char *line)
{
	if (strcmp(args[0], "exit") == 0)
	{
		handle_exit(args, line);
		return (1);
	}
	if (strcmp(args[0], "env") == 0)
	{
		handle_env();
		return (1);
	}
	return (0);
}
