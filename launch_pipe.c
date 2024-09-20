#include "shell_utils.h"

int fd[MAXLEN/2][2];

void launch_first_pipe(char* first_command, int num_pipes) {
    if (fork() == 0) {
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

int launch_pipe(char* input) {

    char* pipe_commands[MAXLEN];
    int num_pipe_commands = split(input, "|", pipe_commands);
    int num_pipes = num_pipe_commands - 1;

    for(int i = 0; i < num_pipes; i++) {
        pipe(fd[i]);
    }

    launch_first_pipe(pipe_commands[0], num_pipes);

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

    launch_last_pipe(pipe_commands[num_pipe_commands - 1], num_pipes);

    for(int i = 0; i < num_pipes; i++) {
        close(fd[i][0]);
        close(fd[i][1]);
    }

    for(int i = 0; i < num_pipe_commands; i++) {
        wait(NULL);
    }

    return 0;
}