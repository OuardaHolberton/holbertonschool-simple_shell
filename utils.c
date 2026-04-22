#include "hsh.h"

/**
 * parse_line - split a line into an array of arguments
 * @line: the line to parse
 * Return: array of strings, NULL-terminated
 */
char **parse_line(char *line)
{
	char **args;
	char *token;
	int i;

	args = malloc(sizeof(char *) * 1024);
	if (args == NULL)
	{
		perror("malloc");
		exit(1);
	}
	i = 0;
	token = strtok(line, " \t\n");
	while (token != NULL)
	{
		args[i] = token;
		i++;
		token = strtok(NULL, " \t\n");
	}
	args[i] = NULL;
	return (args);
}

/**
 * free_args - free an array of arguments
 * @args: the array to free
 */
void free_args(char **args)
{
	free(args);
}
