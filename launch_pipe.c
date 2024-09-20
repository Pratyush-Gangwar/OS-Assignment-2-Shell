#include "shell_utils.h"

int fd[MAXLEN/2][2];

int launch_first_pipe(char* first_command, int num_pipes) {
    int pid = fork();
    if (pid == 0) {
        dup2( fd[0][1], STDOUT_FILENO );
        close(fd[0][0]);

        for(int i = 1; i < num_pipes; i++) {
            close(fd[i][0]);
            close(fd[i][0]);
        }
        
        char* args[MAXLEN];
        split(first_command, " ", args);
        exec_wrapper(args);
    }

    return pid;
}

void launch_last_pipe(char* last_command, int num_pipes) {
    if (fork() == 0) {
        dup2( fd[num_pipes - 1][0], STDIN_FILENO );
        close( fd[num_pipes - 1][1] );

        for(int i = 0; i < num_pipes; i++) {
            if (i != num_pipes - 1) {
                close(fd[i][0]);
                close(fd[i][1]);
            }
        }

        char* args[MAXLEN];
        split(last_command, " ", args);
        exec_wrapper(args);
    }
} 

void launch_intermediate_pipes(char* pipe_commands[], int num_pipe_commands, int num_pipes) {
    for(int i = 1; i < num_pipe_commands - 1; i++) {
        if (fork() == 0) {
            dup2( fd[i - 1][0], STDIN_FILENO );
            close( fd[i - 1][1] ); 

            dup2( fd[i][1], STDOUT_FILENO );
            close( fd[i][0] );

            for(int j = 0; j < num_pipes; j++) {
                if (j != i && j != i - 1) {
                    close(fd[j][0]);
                    close(fd[j][1]);
                }
            }

            char* args[MAXLEN];
            split(pipe_commands[i], " ", args);
            exec_wrapper(args);

        }

    }
}

int wait_for_all(int num_pipe_commands) {
    int status = 0;
    for(int i = 0; i < num_pipe_commands; i++) {
        int wstatus;
        wait(&wstatus);

        if (WIFEXITED(wstatus)) {

            if (WEXITSTATUS(wstatus) != 0) {
                wstatus = WEXITSTATUS(wstatus);
            }

        }
    }

    return status;
}

int launch_pipe(char* input) {

    struct history_entry* entry = malloc(sizeof(struct history_entry));
    set_entry_command(input, entry);

    char* pipe_commands[MAXLEN];
    int num_pipe_commands = split(input, "|", pipe_commands);
    int num_pipes = num_pipe_commands - 1;

    for(int i = 0; i < num_pipes; i++) {
        pipe(fd[i]);
    }

    set_entry_start(entry);

    int pid = launch_first_pipe(pipe_commands[0], num_pipes);
    entry->pid = pid;

    launch_intermediate_pipes(pipe_commands, num_pipe_commands, num_pipes);
    launch_last_pipe(pipe_commands[num_pipe_commands - 1], num_pipes);

    for(int i = 0; i < num_pipes; i++) {
        close(fd[i][0]);
        close(fd[i][1]);
    }

    int status = wait_for_all(num_pipe_commands);
    
    set_entry_end(entry);
    add_history_entry(entry);

    return status;
}