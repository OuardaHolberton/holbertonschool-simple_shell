#  Simple Shell — hsh

> A simple UNIX command line interpreter written in C.  
> Project by **Ouarda** & **Ulysse** — Holberton School.

---

## Table of Contents

- [Description](#description)
- [Flowchart](#flowchart)
- [File Structure](#file-structure)
- [Compilation](#compilation)
- [Usage](#usage)
- [Built-ins](#built-ins)
- [Examples](#examples)
- [Requirements](#requirements)
- [Authors](#authors)

---

## Description

`hsh` is a simple UNIX command line interpreter that mimics the behavior of `/bin/sh`.  
It runs as an infinite loop — reading, analyzing, and executing commands — until the user exits or sends EOF (`Ctrl+D`).

---

### Phase 1 — Reading and parsing

When the shell starts, it first checks whether it is running in **interactive mode** (connected to a terminal) or **non-interactive mode** (receiving input from a pipe or file), using `isatty(STDIN_FILENO)`.

- If **interactive**: the prompt `$ ` is displayed and the shell waits for the user to type a command.
- If **non-interactive**: no prompt is shown and commands are read directly from the input stream.

The input line is read with `getline`. If it returns `-1`, this means EOF was reached (`Ctrl+D`) and the shell exits cleanly.

Once the line is read, it is split into an array of arguments using `strtok` with the delimiters `" \t\n"`.  
For example: `ls -l /tmp` becomes `args[] = ["ls", "-l", "/tmp", NULL]`.  
The array is always terminated by `NULL`.

---

### Phase 2 — Locating the command

Before creating any process, the shell checks whether the command is a **built-in** (like `exit` or `env`) using `strcmp`. If it is, it is executed directly — no `fork` is called.

If the command is not a built-in, the shell searches for its full executable path:

1. If the command already contains a `/` (e.g. `/bin/ls`), it is used directly after checking with `access`.
2. Otherwise, the shell retrieves the `PATH` environment variable, splits it by `:`, and checks each directory using `access(path, X_OK)`.

If the command is not found anywhere in `PATH`, an error message is printed and the shell returns to the prompt — **without forking**.

---

### Phase 3 — Execution

Once the full path is found, the shell calls `fork()` to duplicate the current process:

- The **child process** calls `execve(path, args, environ)` — it becomes the command. If `execve` fails, `perror` is called and the child exits with status `1`.
- The **parent process** calls `wait(NULL)` — it blocks until the child finishes, then frees memory and loops back to Phase 1.

This cycle repeats indefinitely until `exit` is called or EOF is reached.

---

## Flowchart

![Simple Shell Flowchart](flowchart.webp)

---

## File Structure

```
holbertonschool-simple_shell/
├── main.c
├── shell.c
├── utils.c
├── path.c
├── builtins.c
├── hsh.h
├── man_1_simple_shell
├── AUTHORS
├── README.md
└── flowchart.webp
```

---

### `hsh.h` — Header file

Contains all includes, the `extern char **environ` declaration, and all function prototypes used across the project.

| Prototype | Description |
|-----------|-------------|
| `int main(int argc, char **argv)` | Entry point of the shell |
| `void exec_cmd(char **args, char **argv)` | Forks and executes a command |
| `char **parse_line(char *line)` | Tokenizes an input line into arguments |
| `void free_args(char **args)` | Frees the argument array |
| `char *get_path(void)` | Returns a copy of the PATH variable |
| `char *check_cmd(char *dir, char *cmd)` | Checks if a command exists in a directory |
| `char *find_path(char *cmd)` | Finds the full path of a command |
| `int handle_builtins(char **args, char *line)` | Detects and executes built-in commands |
| `void handle_env(void)` | Prints all environment variables |
| `void handle_exit(char **args, char *line)` | Exits the shell cleanly |

---

### `main.c` — Entry point

Contains the `main` function which drives the entire shell loop.

**How it works:**
1. Checks if the shell is running in interactive mode with `isatty` — if yes, displays the prompt `$ `
2. Reads user input using `getline`
3. Handles EOF (`Ctrl+D`) by exiting cleanly
4. Parses the input line into an argument array with `parse_line`
5. Checks for built-in commands with `handle_builtins`
6. Resolves the full path of the command with `find_path`
7. Replaces `args[0]` with the full path, then calls `exec_cmd`
8. Frees all allocated memory before the next iteration

| Function | Description |
|----------|-------------|
| `main` | Main loop of the shell |

---

### `shell.c` — Command execution

Handles the creation of a child process and the execution of a command.

**How it works:**
1. Calls `fork()` to create a child process
2. In the **child process**: calls `execve` with the full path, the arguments array, and the environment — the child process becomes the command
3. In the **parent process**: calls `wait(NULL)` to wait for the child to finish before returning to the prompt
4. If `fork` fails, prints an error with `perror`

| Function | Description |
|----------|-------------|
| `exec_cmd(char **args, char **argv)` | Forks and executes a command via `execve` |

---

### `utils.c` — Input parsing

Handles tokenizing the raw input line and freeing memory.

**How it works:**
1. `parse_line` allocates an array of 1024 `char *` pointers
2. Uses `strtok` with delimiters `" \t\n"` to split the line into tokens (words)
3. Stores each token in the array and terminates it with `NULL`
4. `free_args` frees only the array — not the tokens themselves, since they point directly into the original `line` buffer

| Function | Description |
|----------|-------------|
| `parse_line(char *line)` | Splits a line into a NULL-terminated array of strings |
| `free_args(char **args)` | Frees the argument array |

---

### `path.c` — PATH resolution

Handles finding the full executable path of a command.

**How it works:**
1. `get_path` retrieves the `PATH` environment variable and returns a `strdup` copy (necessary because `strtok` modifies the string)
2. `check_cmd` builds a full path string (`dir + "/" + cmd`) and uses `access(X_OK)` to check if it is executable
3. `find_path` first checks if the command is already an absolute path, then iterates through each directory in `PATH` using `strtok`, calling `check_cmd` on each one

| Function | Description |
|----------|-------------|
| `get_path(void)` | Returns a copy of the `PATH` environment variable |
| `check_cmd(char *dir, char *cmd)` | Returns the full path if the command exists in the given directory |
| `find_path(char *cmd)` | Finds and returns the full executable path of a command |

---

### `builtins.c` — Built-in commands

Handles commands that are executed directly by the shell without forking.

**How it works:**
1. `handle_builtins` compares `args[0]` against known built-in names using `strcmp`
2. If `exit` is detected, calls `handle_exit` which frees memory and calls `exit(0)`
3. If `env` is detected, calls `handle_env` which iterates over `environ` and prints each variable

| Function | Description |
|----------|-------------|
| `handle_env(void)` | Prints all environment variables from `environ` |
| `handle_exit(char **args, char *line)` | Frees memory and exits the shell |
| `handle_builtins(char **args, char *line)` | Checks for built-ins and executes them — returns `1` if a built-in was found, `0` otherwise |

---

## Compilation

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

---

## Usage

### Interactive mode

```bash
$ ./hsh
$ /bin/ls
$ ls -la
$ exit
```

### Non-interactive mode

```bash
$ echo "/bin/ls" | ./hsh
$ cat commands.txt | ./hsh
```

---

## Built-ins

| Command | Description |
|---------|-------------|
| `exit` | Exit the shell |
| `env` | Print all current environment variables |

---

## Examples

```bash
$ ./hsh
$ ls
hsh  main.c  shell.c  utils.c  path.c  builtins.c  hsh.h
$ /bin/echo Hello World
Hello World
$ env
USER=ouarda
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
...
$ exit
```

Error handling:

```bash
$ echo "qwerty" | ./hsh
./hsh: 1: qwerty: not found
```

---

## Requirements

- Ubuntu 20.04 LTS
- gcc with flags: `-Wall -Werror -Wextra -pedantic -std=gnu89`
- Betty style compliant
- No memory leaks
- Max 5 functions per file
- All header files include-guarded

---

## Authors

- **Ouarda**
- **Ulysse**

  