#include "caiosh.h"

#define BUFFER_SIZE 1024

#define MAX_ARGS 256

char **parse_get_tokens(char *cmd)
{
    int i, argc = 0;
    char **argv = (char **)malloc(sizeof(char *) * MAX_ARGS);
    char *token = strtok(cmd, " \t\n");
    while (token != NULL && argc < MAX_ARGS - 1)
    {
        argv[argc] = (char *)malloc(sizeof(char) * (strlen(token) + 1));
        strcpy(argv[argc], token);
        argc++;
        token = strtok(NULL, " \t\n");
    }
    argv[argc] = NULL;
    return argv; // todo: free all this allocated memory
}

char **parse_get_args(char **tokens)
{
    // return a copy of tokens until first "<" or ">"

    int i = 0;
    char **args = (char **)malloc(sizeof(char *) * MAX_ARGS);
    while (tokens[i] != NULL)
    {
        if (strcmp(tokens[i], "<") == 0 || strcmp(tokens[i], ">") == 0)
        {
            args[i] = NULL;
            return args;
        }
        args[i] = (char *)malloc(sizeof(char) * (strlen(tokens[i]) + 1));
        strcpy(args[i], tokens[i]);
        i++;
    }
    args[i] = NULL;
    return args;
}

char *parse_get_stdin(char **tokens)
{
    // find token that is "<", and then the next token is the stdin

    int i = 0;
    while (tokens[i] != NULL)
    {
        if (strcmp(tokens[i], "<") == 0)
        {
            return tokens[i + 1];
        }
        i++;
    }
    return NULL;
}

char *parse_get_stdout(char **tokens)
{
    // find token that is ">", and then the next token is the stdout

    int i = 0;
    while (tokens[i] != NULL)
    {
        if (strcmp(tokens[i], ">") == 0)
        {
            return tokens[i + 1];
        }
        i++;
    }
    return NULL;
}

bool is_malformed(char *cmd)
{
    // todo: treat this in the second bimester
    return false;
}

int redirect(char *path, int flags, int default_fd)
{
    if (path == NULL)
    {
        return default_fd;
    }

    int fd = open(path, flags, S_IRUSR | S_IWUSR);

    if (fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    return fd;
}

void read_command(char *cmd)
{
    if (fgets(cmd, BUFFER_SIZE, stdin) == NULL)
    {
        exit(EXIT_SUCCESS);
    }
}

void caiosh_loop()
{
    char cmd[BUFFER_SIZE];
    char *prog;
    char **tokens;
    char **args;
    char *in;
    char *out;

    while (1)
    {
        caiosh_print_prompt();
        read_command(cmd);
        tokens = parse_get_tokens(cmd);
        in = parse_get_stdin(tokens);
        out = parse_get_stdout(tokens);
        args = parse_get_args(tokens);
        prog = args[0];

        if (is_malformed(cmd))
        {
            printf("Malformed command\n");
            continue;
        }

        if (strcmp(prog, "exit") == 0)
        {
            break;
        }

        int child_pid = fork();

        int status;

        if (child_pid) // parent
        {
            wait(&status);
        }
        else // child
        {
            int fd_in = redirect(in, O_RDONLY, STDIN_FILENO);
            int fd_out = redirect(out, O_RDWR | O_CREAT | O_TRUNC, STDOUT_FILENO);
            dup2(fd_in, STDIN_FILENO);
            dup2(fd_out, STDOUT_FILENO);
            execvp(prog, args);
            close(fd_in);
            close(fd_out);
        }
    }
}

void caiosh_print_welcome()
{
    printf("Hello from Caio!\n");
}

void caiosh_print_prompt()
{
    printf("caiosh> ");
}

int main(int argc, char **argv)
{
    caiosh_print_welcome();
    caiosh_loop();

    return EXIT_SUCCESS;
}
