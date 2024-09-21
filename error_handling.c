#include "shell_utils.h"

pid_t wait_wrapper(int* wstatus) {
    pid_t pid = wait(wstatus);

    if ( pid == -1 ) {
        perror("launch_normal wait error: ");
        exit(1);
    }

    return pid;
}

pid_t fork_wrapper() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("split_and_run fork error: ");
        exit(1);
    }

    return pid;
}