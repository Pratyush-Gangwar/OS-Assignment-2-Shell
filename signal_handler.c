#include "shell_utils.h"

void handler(int signal) {
    print_history_entry_details();
}

void setup_signal_handlers() {
    struct sigaction history_sa;
    memset(&history_sa, 0, sizeof(struct sigaction));
    history_sa.sa_handler = handler;

    sigaction(SIGINT, &history_sa, NULL);
    sigaction(SIGHUP, &history_sa, NULL);
}