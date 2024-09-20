#include "shell_utils.h"

int launch_background(char* input) {
    return 0;
}

int launch_pipe(char* input) {
    return 0;

}

int launch_normal(char* input) {
    char* args[MAXLEN];
    split(input, ' ', args);

    if (execvp(args[0], args) == -1) {
        perror("launch_normal execvp error: ");
        exit(1);
    }

    int wstatus;
    wait(&wstatus);

    if (WIFEXITED(wstatus)) {
        return WEXITSTATUS(wstatus);
    }

    return 1;
}