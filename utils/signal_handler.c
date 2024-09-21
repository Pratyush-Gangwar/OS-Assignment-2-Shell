#include "shell_utils.h"

void history_handler(int signal) {
    print_history_entry_details();
    exit(0);
}

// different function prototype for sa_handler and sa_sigaction
void background_handler(int signal, siginfo_t* info, void* ucontext) {
    struct history_entry* entry = get_entry_by_pid(info->si_pid);

    
    // add_history_entry() is called after the wait
    // but SIGCHLD is handled before wait is called for foreground processes
    // so, get_entry_by_pid returns NULL for foreground processes
    // but background processes are already added to the history list before they terminate so they aren't NULL
    
    if (entry != NULL) {
        set_entry_end(entry);

        // reap the child
        if (waitpid(entry->pid, NULL, WNOHANG) == -1) {
            perror("waitpid error ");
            exit(1);
        } 
    }
}

void setup_signal_handlers() {
    // memset doesn't give errors

    struct sigaction history_sa;
    memset(&history_sa, 0, sizeof(struct sigaction));
    history_sa.sa_handler = history_handler;

    sigaction_wrapper(SIGINT, &history_sa, NULL);



    struct sigaction background_sa;
    memset(&background_sa, 0, sizeof(struct sigaction));

    // SA_SIGINFO so that siginfo_t struct is passed to handler
    // SA_RESTART so that interrupted syscalls (such as fgets) are resumed. otherwise, we may run into errors
    background_sa.sa_flags = SA_SIGINFO | SA_RESTART; 

    // since we used SA_SIGINFO, we must specify sa_sigaction and not sa_handler
    background_sa.sa_sigaction = background_handler;

    sigaction_wrapper(SIGCHLD, &background_sa, NULL);
}