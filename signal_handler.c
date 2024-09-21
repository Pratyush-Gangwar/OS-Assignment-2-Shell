#include "shell_utils.h"

void history_handler(int signal) {
    print_history_entry_details();
    exit(0);
}

void background_handler(int signal, siginfo_t* info, void* ucontext) {
    struct history_entry* entry = get_entry_by_pid(info->si_pid);

    
    // add_history_entry() is called after the wait
    // but SIGCHLD is handled before wait is called for foreground processes
    // so, get_entry_by_pid returns NULL for foreground processes
    // but background processes are already added to the history list before they terminate so they aren't NULL
    
    if (entry != NULL) {
        set_entry_end(entry);
        waitpid(entry->pid, NULL, WNOHANG); // reap the child
    }
}

void setup_signal_handlers() {
    struct sigaction history_sa;
    memset(&history_sa, 0, sizeof(struct sigaction));
    history_sa.sa_handler = history_handler;

    sigaction(SIGINT, &history_sa, NULL);
    sigaction(SIGHUP, &history_sa, NULL);

    struct sigaction background_sa;
    memset(&background_sa, 0, sizeof(struct sigaction));
    background_sa.sa_flags = SA_SIGINFO | SA_RESTART;
    background_sa.sa_sigaction = background_handler;

    sigaction(SIGCHLD, &background_sa, NULL);
}