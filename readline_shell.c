
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_CMD_LENGTH 1024
#define MAX_ARGS 100


// Function to find the full path of the command
char *find_command(char *cmd) {
    char *path = getenv("PATH");
    char *path_copy = strdup(path);
    char *dir = strtok(path_copy, ":");
    char full_path[MAX_CMD_LENGTH];

    while (dir != NULL) {
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
        struct stat buffer;
        if (stat(full_path, &buffer) == 0) {
            free(path_copy);
            return strdup(full_path); // Return the full path if found
        }
        dir = strtok(NULL, ":");
    }

    free(path_copy);
    return NULL; // Command not found
}

void execute_command(char *cmd) {
    char *args[MAX_ARGS];
    char *token;
    int i = 0;

    // Tokenize the command string
    token = strtok(cmd, " \n");
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL; // Null-terminate the argument list

 // Find the full path of the command
    char *full_path = find_command(args[0]);
    if (full_path == NULL) {
        // fprintf(stderr, "Command not found: %s\n", args[0]);
        // return;
      full_path = args[0];
    }

    // Fork a child process
    pid_t pid = fork();
    if (pid == 0) {
        // In child process
        execve(full_path, args, NULL);
        perror("execve failed"); // If execve returns, it must have failed
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork failed");
    } else {
        // In parent process, wait for child to finish
        wait(NULL);
    }
}

void init_env(){
    // Add an extra directory to the PATH
    const char *extra_path = "/bin"; // Change this to your desired path
    char *current_path = getenv("PATH");
    char new_path[MAX_CMD_LENGTH];

    // Create a new PATH string
    snprintf(new_path, sizeof(new_path), "%s:%s", current_path, extra_path);

    printf("PATH=%s\n", new_path);

    setenv("PATH", new_path, 1); // Update the PATH environment variable
    setenv("TERM", "xterm-256color", 1);
    setenv("HOME", "/home/wschrep", 1);
}


int main() {
    char *input;
    init_env();

    while (1) {
        // Read input using readline
        input = readline("$ ");
        if (input == NULL) {
            // Handle EOF (Ctrl+D)
            printf("\n");
            break;
        }

        if (strlen(input) > 0) {
            // Add input to history
            add_history(input);
            execute_command(input);
        }

        free(input); // Free the allocated memory for input
    }

    return 0;
}
