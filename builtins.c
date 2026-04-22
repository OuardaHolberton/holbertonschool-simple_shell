#include "hsh.h"

/**
 * handle_env - print all environment variables
 * Return: nothing
 */
void handle_env(void)
{
	int i;

	i = 0;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
}



/**
 * handle_exit - exit the shell
 * @args: array of arguments
 * @line: input line to free
 * Return: nothing
 */
void handle_exit(char **args, char *line)
{
	free_args(args);
	free(line);
	exit(0);
}


/**
 * handle_builtins - check and execute built-ins
 * @args: array of arguments
 * @line: input line to free
 * Return:	1 if built-in executed
 *			0 if not
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
