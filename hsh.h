#ifndef HSH_H
#define HSH_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

extern char **environ;

int main(int argc, char **argv);
void exec_cmd(char **args, char **argv);
char **parse_line(char *line);
void free_args(char **args);
char *get_path(void);
char *check_cmd(char *dir, char *cmd);
char *find_path(char *cmd);
int handle_builtins(char **args, char *line);
void handle_env(void);
void handle_exit(char **args, char *line);

#endif