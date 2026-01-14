#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>

/* * CONSTANTS 
 * Based on Source [24]: Split by spaces/tabs
 */
#define DELIMITERS " \t\r\n\a"

/* GLOBAL VARIABLES */
extern char **environ; // Required for 'env' and passing env to children

/* FUNCTION PROTOTYPES */
void loop(void);
char *read_line(void);
char **split_line(char *line);
int execute(char **args);
int launch(char **args);

/* BUILTIN FUNCTIONS */
int nan_cd(char **args);
int nan_exit(char **args);
int nan_pwd(char **args);
int nan_env(char **args);

char *builtin_str[] = { "cd", "exit", "pwd", "env" };
int (*builtin_func[]) (char **) = { &nan_cd, &nan_exit, &nan_pwd, &nan_env };

int nan_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/* * MAIN
 * Entry point of the program
 */
int main(void) {
    // Start the Read-Eval-Print Loop
    loop(); 
    return EXIT_SUCCESS;
}

/* * LOOP
 * Displays prompt, reads input, parses, and executes.
 * Source [11]: Display prompt
 * Source [36]: Handle Ctrl-D (EOF)
 */
void loop(void) {
    char *line;
    char **args;
    int status = 1;

    do {
        // 1. Display Prompt [cite: 18]
        printf("?> ");
        
        // 2. Read Line
        line = read_line();
        if (line == NULL) {
            // EOF (Ctrl+D) detected, exit gracefully [cite: 36]
            printf("\n");
            exit(EXIT_SUCCESS);
        }

        // 3. Parse Line [cite: 24]
        args = split_line(line);
        
        // 4. Execute Command
        status = execute(args);

        // Cleanup
        free(line);
        free(args);
    } while (status);
}

char *read_line(void) {
    char *line = NULL;
    size_t bufsize = 0;
    
    // getline handles memory allocation automatically
    if (getline(&line, &bufsize, stdin) == -1) {
        if (feof(stdin)) {
            free(line);
            return NULL; // Return NULL on EOF
        } else {
            perror("nanoshell: readline error");
            exit(EXIT_FAILURE);
        }
    }
    return line;
}

char **split_line(char *line) {
    int bufsize = 64;
    int position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "nanoshell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, DELIMITERS);
    while (token != NULL) {
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "nanoshell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, DELIMITERS);
    }
    tokens[position] = NULL;
    return tokens;
}

/* * EXECUTE
 * Dispatches to builtin or launch
 */
int execute(char **args) {
    if (args[0] == NULL) {
        // Empty command (user just pressed enter) [cite: 27]
        return 1;
    }

    for (int i = 0; i < nan_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return launch(args);
}

/* * LAUNCH
 * Handles fork, execve, waitpid, and PATH resolution.
 * Source [7]: fork/execve/waitpid usage
 * Source [28-34]: PATH resolution logic
 */
int launch(char **args) {
    pid_t pid;
    int status;
    char *command = args[0];
    char *full_path = NULL;

    // PATH RESOLUTION
    if (strchr(command, '/') != NULL) {
        // Case 1: Command has a path (e.g. /bin/ls or ./prog) [cite: 29]
        if (access(command, X_OK) == 0) {
            full_path = strdup(command);
        }
    } else {
        // Case 2: Search in PATH variable [cite: 31]
        char *path_env = getenv("PATH");
        if (path_env != NULL) {
            char *path_copy = strdup(path_env);
            char *dir = strtok(path_copy, ":"); // Split PATH by ':'
            
            while (dir != NULL) {
                // Build path: dir + "/" + command
                char buffer[PATH_MAX];
                snprintf(buffer, sizeof(buffer), "%s/%s", dir, command);
                
                if (access(buffer, X_OK) == 0) {
                    // Found it! [cite: 33]
                    full_path = strdup(buffer);
                    break;
                }
                dir = strtok(NULL, ":");
            }
            free(path_copy);
        }
    }

    // ERROR HANDLING
    if (full_path == NULL) {
        // Specific error message required by Source [19]
        printf("nanoshell: weird, %s is not here... :/\n", command);
        return 1; 
    }

    // PROCESS EXECUTION
    pid = fork();
    if (pid == 0) {
        // Child Process
        if (execve(full_path, args, environ) == -1) {
            perror("nanoshell");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error Forking
        perror("nanoshell");
    } else {
        // Parent Process: Wait for child [cite: 7, 52]
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    free(full_path);
    return 1;
}

/* --- BUILTINS --- */

/* Source [20]: Handle simple cd, cd ~, cd dir */
int nan_cd(char **args) {
    char *target;
    if (args[1] == NULL || strcmp(args[1], "~") == 0) {
        target = getenv("HOME"); // Default to HOME
    } else {
        target = args[1];
    }
    
    if (chdir(target) != 0) {
        perror("nanoshell");
    }
    return 1;
}

int nan_exit(char **args) {
    (void)args; // unused
    exit(0);
    return 0;
}

int nan_pwd(char **args) {
    (void)args;
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("nanoshell");
    }
    return 1;
}

int nan_env(char **args) {
    (void)args;
    for (char **env = environ; *env != 0; env++) {
        printf("%s\n", *env);
    }
    return 1;
}