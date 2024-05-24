#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT_SIZE 1024

void executeInternalCommand(char **args) {
    if (strcmp(args[0], "echo") == 0) {
        for (int i = 1; args[i] != NULL; i++) {
            printf("%s ", args[i]);
        }
        printf("\n");
    } else if (strcmp(args[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("getcwd() error");
        }
    } else if (strcmp(args[0], "exit") == 0) {
        exit(0);
    } else {
        printf("Command not found: %s\n", args[0]);
    }
}

void executeExternalCommand(char **args) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
    } else if (pid == 0) {
        execvp(args[0], args);
        perror("Exec failed");
        exit(1);
    } else {
        wait(NULL);
    }
}

int main() {
    char input[MAX_INPUT_SIZE];
    char *args[MAX_INPUT_SIZE / 2 + 1];

    while (1) {
        printf("myshell> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("fgets error");
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        int i = 0;
        args[i] = strtok(input, " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }

        if (args[0] == NULL) {
            continue;
        }

        if (strcmp(args[0], "echo") == 0 || strcmp(args[0], "pwd") == 0 || strcmp(args[0], "exit") == 0) {
            executeInternalCommand(args);
        } else {
            executeExternalCommand(args);
        }
    }

    return 0;
}