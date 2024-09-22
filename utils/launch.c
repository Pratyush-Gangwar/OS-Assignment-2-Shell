#include "shell_utils.h"

void exec_wrapper(char* args[]) {

    // strcmp doesn't give errors
    if (strcmp(args[0], "history") == 0) {
        print_history_entry_names();
        exit(0);
    }

    // execvp handles both /bin/ executables and custom user-made executables
    // if execvp gives an error, just print that the command was unknown instead of giving a perror
    else if (execvp(args[0], args) == -1) { 
        printf("Unknown command.\n");
        exit(0);
    }
}

void split_and_run(char* input, struct history_entry* entry) {
    char* args[MAXLEN];
    split(input, " ", args); // split at spaces to get command arguments and command name

    set_entry_start(entry); // set start_time attribute of entry

    int pid = fork_wrapper(); 

    if (pid == 0) {
        exec_wrapper(args);
    } 
    
    // no need for else because forked children call exec
    entry->pid = pid;
       
}

int launch_background(char* input) {
    struct history_entry* entry = malloc_wrapper(sizeof(struct history_entry));
    set_entry_command(input, entry);
    add_history_entry(entry);
    split_and_run(input, entry);

    // no wait

    return 0;
}

int launch_normal(char* input) {

    struct history_entry* entry = malloc_wrapper(sizeof(struct history_entry)); // need to malloc because we want it to persist outside the function
    set_entry_command(input, entry); // set command member of entry
    split_and_run(input, entry);

    int wstatus;
    wait_wrapper(&wstatus); // wait for the child to terminate
    
    set_entry_end(entry); // set end_time member of entry
    add_history_entry(entry); // add entry to the global array

    if (WIFEXITED(wstatus)) {
        return WEXITSTATUS(wstatus);
    }

    return 1; // if the child didn't use exit(), then WIFEXITED is false and we will land here. this represents abnormal termination
}