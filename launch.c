#include "shell_utils.h"

void exec_wrapper(char* args[], struct history_entry* history_entries[]) {

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

int launch_normal(char* input, struct history_entry* history_entries[]) {

    // split modifies the original string so we must add the command to the history entry before splitting
    // since char* input is being reused in main.c, future commands will be reflected in past entries!
    // we must create a copy

    char* input_copy = malloc(MAXLEN);
    strcpy(input_copy, input);

    struct history_entry* entry = malloc(sizeof(struct history_entry));
    entry->command = input_copy;

    char* args[MAXLEN];
    split(input, " ", args);

    time_t start_time = time(NULL);
    entry->start_time = start_time;

    int pid = fork();
    if (pid == 0) {
        exec_wrapper(args, history_entries);
    } 
    
    // no need for else because forked children call exec   

    int wstatus;
    wait(&wstatus);

    time_t end_time = time(NULL);
    entry->end_time = end_time;

    add_history_entry(entry, history_entries);

    if (WIFEXITED(wstatus)) {
        return WEXITSTATUS(wstatus);
    }

    return 1;
}