#include "shell_utils.h"

void exec_wrapper(char* args[]) {

    if (strcmp(args[0], "history") == 0) {
        print_history_entry_names();
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

int launch_normal(char* input) {

    // split modifies the original string so we must add the command to the history entry before splitting
    // since char* input is being reused in main.c, future commands will be reflected in past entries!
    // we must create a copy

    char* input_copy = malloc(MAXLEN);
    strcpy(input_copy, input);

    struct history_entry* entry = malloc(sizeof(struct history_entry));
    entry->command = input_copy;

    char* args[MAXLEN];
    split(input, " ", args);

    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    entry->start_time = start_time;

    int pid = fork();
    entry->pid = pid;
    
    if (pid == 0) {
        exec_wrapper(args);
    } 
    
    // no need for else because forked children call exec   

    int wstatus;
    wait(&wstatus);

    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    entry->end_time = end_time;

    add_history_entry(entry);

    if (WIFEXITED(wstatus)) {
        return WEXITSTATUS(wstatus);
    }

    return 1;
}