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

    (void)argc;
    while (1)
    {
        if (isatty(STDIN_FILENO))
            write(STDOUT_FILENO, "$ ", 1);
        if (getline(&line, &len, stdin) == -1)
        {
            write(STDOUT_FILENO, "\n", 1);
            free(line);
            return (0);
        }
        exec_cmd(line, argv);
    }
    free(line);
    return (0);
}
