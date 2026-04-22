#include "hsh.h"

/**
 * get_path - get PATH environment variable
 * Return: copy of PATH string, or NULL if not found
 */
char *get_path(void)
{
	int i;

	i = 0;
	while (environ[i])
	{
		if (strncmp(environ[i], "PATH=", 5) == 0)
			return (strdup(environ[i] + 5));
		i++;
	}
	return (NULL);
}

/**
 * check_cmd - does command exists in directory?
 * @dir: directory to check
 * @cmd: command to look for
 * Return: full path if found, else NULL
 */
char *check_cmd(char *dir, char *cmd)
{
	char *full_path;
	int len;

	len = strlen(dir) + strlen(cmd) + 2;
	full_path = malloc(sizeof(char) * len);
	if (full_path == NULL)
	{
		perror("malloc");
		exit(1);
	}
	strcpy(full_path, dir);
	strcat(full_path, "/");
	strcat(full_path, cmd);
	if (access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

/**
 * find_path - find full path of a command
 * @cmd: command to find
 * Return: full path if found, else NULL
 */
char *find_path(char *cmd)
{
	char *path;
	char *dir;
	char *full_path;

	if (strchr(cmd, '/') != NULL)
	{
		if (access(cmd, X_OK) == 0)
			return (strdup(cmd));
		return (NULL);
	}
	path = get_path();
	if (path == NULL)
		return (NULL);
	dir = strtok(path, ":");
	while (dir != NULL)
	{
		full_path = check_cmd(dir, cmd);
		if (full_path != NULL)
		{
			free(path);
			return (full_path);
		}
		dir = strtok(NULL, ":");
	}
	free(path);
	return (NULL);
}
