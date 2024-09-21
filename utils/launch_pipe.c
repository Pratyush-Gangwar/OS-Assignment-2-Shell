#include "shell_utils.h"

int fd[MAXLEN/2][2]; // global fd array

int launch_first_pipe(char* first_command, int num_pipes) {
    // uses the pipe fd[0]

    int pid = fork_wrapper();
    if (pid == 0) {

        // reads from STDIN
        // writes to fd[0][1] 
        // other end, i.e, fd[0][0] is unused and so close that
        dup2_wrapper( fd[0][1], STDOUT_FILENO ); 
        close_wrapper(fd[0][0]);


        // remaining pipes are unused. close both ends
        for(int i = 1; i < num_pipes; i++) {
            close_wrapper(fd[i][0]);
            close_wrapper(fd[i][1]);
        }
        
        char* args[MAXLEN];
        split(first_command, " ", args);
        exec_wrapper(args);
    }

    return pid; // to populate the pid attribute in history_entry struct
}

void launch_last_pipe(char* last_command, int num_pipes) {
    // uses fd[num_pipes - 1]

    if (fork_wrapper() == 0) {

        // reads from fd[num_pipes - 1][0]
        // writes to STDOUT
        // other end (i.e, fd[num_pipes - 1][1]) is unused so close that
        dup2_wrapper( fd[num_pipes - 1][0], STDIN_FILENO );
        close_wrapper( fd[num_pipes - 1][1] );


        // close all other pipes
        for(int i = 0; i < num_pipes; i++) {
            if (i != num_pipes - 1) {
                close_wrapper(fd[i][0]);
                close_wrapper(fd[i][1]);
            }
        }

        char* args[MAXLEN];
        split(last_command, " ", args);
        exec_wrapper(args);
    }
} 

void launch_intermediate_pipes(char* pipe_commands[], int num_pipe_commands, int num_pipes) {
    // uses both fd[i] and fd[i - 1]

    for(int i = 1; i < num_pipe_commands - 1; i++) {
        if (fork_wrapper() == 0) {

            // reads from fd[i - 1][0]
            // close the other end - fd[i - 1][1]
            dup2_wrapper( fd[i - 1][0], STDIN_FILENO );
            close_wrapper( fd[i - 1][1] ); 

            // writes to fd[i][1]
            // close the other end - fd[i][0]
            dup2_wrapper( fd[i][1], STDOUT_FILENO );
            close_wrapper( fd[i][0] );


            // close all other pipes. be careful not to change fd[i] or fd[i - 1]
            for(int j = 0; j < num_pipes; j++) {
                if (j != i && j != i - 1) {
                    close_wrapper(fd[j][0]);
                    close_wrapper(fd[j][1]);
                }
            }

            char* args[MAXLEN];
            split(pipe_commands[i], " ", args);
            exec_wrapper(args);

        }

    }
}

// returns 0 on success. if any of the commands fails, then it returns the exit code of the last command that failed
int wait_for_all(int num_pipe_commands) {
    
    int status = 0; // value to return

    for(int i = 0; i < num_pipe_commands; i++) {
        int wstatus;
        wait_wrapper(&wstatus);

        if (WIFEXITED(wstatus)) {

            if (WEXITSTATUS(wstatus) != 0) {
                status = WEXITSTATUS(wstatus); // update status if the current command failed
            }

        }
    }

    return status;
}


int launch_pipe(char* input) {

    struct history_entry* entry = malloc_wrapper(sizeof(struct history_entry));
    set_entry_command(input, entry);


    // split at "|" to get individual commands
    // split will automatically strip leading/trailing whitespace
    char* pipe_commands[MAXLEN];
    int num_pipe_commands = split(input, "|", pipe_commands); 
    int num_pipes = num_pipe_commands - 1;

    // creates pipes
    for(int i = 0; i < num_pipes; i++) {
        if (pipe(fd[i]) == -1) {
            perror("launch_pipe pipe error: ");
            exit(1);
        }
    }

    set_entry_start(entry);

    // pid of a piped command is pd of the first command
    int pid = launch_first_pipe(pipe_commands[0], num_pipes);
    entry->pid = pid;

    launch_intermediate_pipes(pipe_commands, num_pipe_commands, num_pipes);
    launch_last_pipe(pipe_commands[num_pipe_commands - 1], num_pipes);

    // after launching all children, close all pipes 
    for(int i = 0; i < num_pipes; i++) {
        close_wrapper(fd[i][0]);
        close_wrapper(fd[i][1]);
    }
    
    int status = wait_for_all(num_pipe_commands);
    
    set_entry_end(entry);
    add_history_entry(entry);

    return status;
}