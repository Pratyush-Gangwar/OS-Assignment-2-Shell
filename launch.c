#include "shell_utils.h"

void exec_wrapper(char* args[], struct history_entry history_entries[]) {

    if (strcmp(args[0], "history") == 0) {
        print_history_entries(history_entries);
        exit(0);
    }

    // execvp handles both /bin/ executables and custom user-made executables
    else if (execvp(args[0], args) == -1) { 
        printf("Unknown command.\n");
        exit(0);
    }
}

int launch_background(char* input) {
    return 0;
}

int launch_pipe(char* input) {
    return 0;

}

int launch_normal(char* input, struct history_entry history_entries[]) {
    char* args[MAXLEN];
    split(input, " ", args);

    if (fork() == 0) {
        exec_wrapper(args, history_entries);
    }

    int wstatus;
    wait(&wstatus);

    if (WIFEXITED(wstatus)) {
        return WEXITSTATUS(wstatus);
    }

    return 1;
}