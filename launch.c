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
    split(input, " ", args);

    set_entry_start(entry);

    int pid = fork();
    entry->pid = pid;

    if (pid == 0) {
        exec_wrapper(args);
    } 

    else if (pid < 0) {
        perror("split_and_run fork error: ");
        exit(1);
    }
    
    // no need for else because forked children call exec   
}

int launch_background(char* input) {
    struct history_entry* entry = malloc(sizeof(struct history_entry));
    set_entry_command(input, entry);
    add_history_entry(entry);
    split_and_run(input, entry);

    // no wait

    return 0;
}

int launch_normal(char* input) {

    struct history_entry* entry = malloc(sizeof(struct history_entry));
    set_entry_command(input, entry);
    split_and_run(input, entry);

    int wstatus;
    if (wait(&wstatus) == -1) {
        perror("launch_normal wait error: ");
        exit(1);
    }

    set_entry_end(entry);
    add_history_entry(entry);

    if (WIFEXITED(wstatus)) {
        return WEXITSTATUS(wstatus);
    }

    return 1;
}