#include "hsh.h"
/**
 * main - entry point of the shell
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t len = 0;
	char **args;
	char *full_path;
	int line_count;

	(void)argc;
	line_count = 0;
	while (1)
	{
		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "$ ", 2);
		if (getline(&line, &len, stdin) == -1)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);
			free(line);
			return (0);
		}
		line_count++;
		args = parse_line(line);
		if (args[0] == NULL)
		{
			free_args(args);
			continue;
		}
		if (handle_builtins(args, line))
			continue;
		full_path = find_path(args[0]);
		if (full_path == NULL)
		{
			fprintf(stderr, "%s: %d: %s: not found\n",
				argv[0], line_count, args[0]);
			free_args(args);
			continue;
		}
		args[0] = full_path;
		exec_cmd(args, argv);
		free(args[0]);
		args[0] = NULL;
		free_args(args);
	}
	return (0);
}
