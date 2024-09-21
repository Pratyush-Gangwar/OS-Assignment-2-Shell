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

void* malloc_wrapper(size_t size) {
    void* ptr = malloc(size);

    if (ptr == NULL) {
        perror("malloc error: ");
    }

    return ptr;
}

int dup2_wrapper(int oldfd, int newfd) {
    int fd = dup2(oldfd, newfd);

    if (fd == -1) {
        perror("dup2 error: ");
        exit(1);
    }

    return fd;
}

int close_wrapper(int fd) {
    int ret = close(fd);

    if (ret == -1) {
        perror("close error: ");
        exit(1);
    }

    return ret;
}

int gettimeofday_wrapper(struct timeval* tv, struct timezone* tz) {
    int ret = gettimeofday(tv, tz);

    if (ret == -1) {
        perror("gettimeofday error:");
        exit(1);
    }

    return ret;
}