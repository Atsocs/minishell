// include guard
#ifndef CAIOSH
#define CAIOSH

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <unistd.h>

#include <sys/wait.h>

char **parse_get_tokens(char *cmd);

char **parse_get_args(char **tokens);

char *parse_get_stdin(char **tokens);

char *parse_get_stdout(char **tokens);

bool is_malformed(char *cmd);

void read_command(char *cmd);

void caiosh_loop();

void caiosh_print_welcome();

void caiosh_print_prompt();

int redirect(char *path, int flags, int default_fd);

#endif