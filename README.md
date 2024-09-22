# Group 121
Pratyush Gangwar (Solo)

# Github
https://github.com/Pratyush-Gangwar/OS-Assignment-2-Shell

# Unsupported commands
fg, bg, jobs, exit, cd (and all other shell built-ins)

These are not commands provided by UNIX. That is, these are not located in /usr/bin/ or /bin/
Instead, these are commands implemented by the shell (similar to history command in this assignment)

# Design

1. main.c: 
main()
- Set up the signal handlers and run shell_loop()

shell_loop()
- It's an infinite loop which performs three steps - print the prompt, read user input and launch the command. If the user input is a blank line, then the command is not launched and not added to history.

print_prompt()
- The prompt displays the username, the machine name and the current working directory, just like the actual shell prompt.
- ANSI coloring is used to make the prompt text red.

read_user_input(char* input)
- Uses fgets to read user input into the specified character array.
- gets is not used since it is deprecated
- The last character read by fgets is a newline so it is removed 

launch(char* input)
- If the input command contains '&', execute launch_background() after removing the '&'
- If it contains '|', execute launch_pipe() 
- Else, execute launch_normal()

- strchr is used to see if the string contains a specific character


2. launch.c

exec_wrapper(char* args[], struct history_entry* entry)
- Check if the command to be run is the history command. If so, execute print_history_entry_names()
- Otherwise, launch execvp() with its arguments being the same as the arguments passed to exec_wrapper()
    - This automatically locates UNIX commands in /usr/bin/ or /bin/
    - It also handles custom ELF files (./hello.out, etc.)

split_and_run(char* input)
- Given an input string, split it at space characters to get the command name and the command arguments
- Set the start_time attribute of the history entry 
- Call fork_wrapper() and execute exec_wrapper() in the child
- In the parent, set pid attribute of history entry and return to the method that called split_and_run()

launch_normal(char* input)
- Allocate space for the history entry on the heap. We want it to persist outside the function
- Set the command attribute of the history entry
- Call split_and_run()

- Wait for the child to exit. Once it exits, set the end time attribute of the history entry and then add the entry to the global history entry array
- If the child was terminated abnormally or exited with a non-zero exit code, then return 1 to the caller of launch() (i.e, shell_loop())

launch_background(char* input)
- Allocate space for the history entry on the heap. We want it to persist outside the function
- Set the command attribute of the history entry
- Add the entry to the global history entry array 
- Call split_and_run()

- Don't wait for the child to exit

3. launch_pipe.c 
- It has a global MAXLEN/2 x 2 array of file descriptors. Each row in the array represents a pipe. 
- It is static to prevent it from being accessed from outside the file. 

launch_first_pipe(char* first_command)
- It forks
- It reads from STDIN and redirects its output to the write end of the pipe with index 0 in the fd array (fd[0])
- The read end of fd[0] is unused so it closes that
- It closes the both ends of the remaining pipes in the array
- Then, it splits the first_command at spaces and passes the resultant array to the exec_wrapper so that the command can be executed. strok() ensures that there are no leading or trailing whitespaces

launch_last_pipe(char* last_command)
- It forks
- It writes to STDOUT and reads its input from the read end of the pipe with index num_pipes - 1 in the fd array (fd[num_pipes - 1])
- The write end of fd[num_pipes - 1] is unused so it closes that
- It closes the both ends of the remaining pipes in the array
- Then, it splits the last_command at spaces and passes the resultant array to the exec_wrapper so that the command can be executed. strok() ensures that there are no leading or trailing whitespaces


launch_intermediate_pipes(char* pipe_commands[])
- It launches all commands except the first and the last command. The for-loop indexing reflects this
- In each iteration of the for-loop, it forks.
- Command with index i will read from read end of fd[i - 1] and write to write end of fd[i]
- fd[i - 1][1] and fd[i][0] are unused so it closes those
- It also closes the remaining pipes in the array 
- Then, it splits the pipe_commands[i] at spaces and passes the resultant array to the exec_wrapper so that the command can be executed. strok() ensures that there are no leading or trailing whitespaces


wait_for_all(int num_pipe_commands)
- Uses a for-loop to wait for all commands
- If all commands succeed, it returns 0
- If any of them fails, then it returns the exit code of the last command that failed or 1 if the last command was abnormally terminated (i.e, without calling exit)

launch_pipe(char* input)
- Allocate space for the history entry on the heap. We want it to persist outside the function
- Set the command attribute of the history entry
- Split the input at '|' to the get the individual pipe commands
- Use a for-loop to create num_pipes number of pipes
- Set start time attribute of history entry
- Launch the first pipe and set pid attribute of history entry as the pid of the first command
- Launch the intermediate pipes 
- Launch the last pipe 
- Then, close all the pipes in the parent
- Call wait_for_all
- Set the end time attribute of the history entry
- Add entry to the global history entries array

4. history.c 
- Has a global array of history entries and a global counter to keep track of the number of history entries
- Both variables are static to prevent them from being accessed from outside history.c

add_history_entry(struct history_entry* entry) 
- adds the given entry to the global entry
- if there are more than MAXLEN entries, then it wraps around and overwrites the first history entry

print_history_entry_names()
- prints the command attribute of history entries

print_history_entry_details
- prints the pid, launch time, duration and command for each entry
- ctime() is used to convert seconds since epoch (start_time is timeval and timeval.tv_sec is time since epoch in seconds) to a date-time string
- start_time and end_time are timeval structs. timeval.tv_sec is seconds since epoch and timeval.tv_usec is microseconds 
- convert both start_time and end_time to microseconds and then calculate duration

set_entry_command(char* input, struct history_entry* entry) 
- sets the command attribute of the entry
- since char* input is being reused in main.c, future commands will be reflected in past entries!
- we must create a copy

set_entry_start(struct history_entry* entry) 
- sets the start_time attribute of entry

set_entry_end(struct history_entry* entry) 
- sets the end_time attribute of entry

get_entry_by_pid(int pid)
- searches the history array for an entry which has the given pid
- if not found, returns NULL

5. signal_handler.c 
history_handler(int signal)
- merely calls print_history_details()

background_handler(int signal, siginfo_t* info, void* ucontext)
- finds the history entry for the child process via its pid
- then, sets the end time for the process and reaps it

- add_history_entry() is called after the wait
- but SIGCHLD is handled before wait is called for foreground processes
- so, get_entry_by_pid returns NULL for foreground processes
- but background processes are already added to the history list before they terminate so they aren't NULL

setup_signal_handlers
- since we want to access the child process PID, we want use the siginfo_t struct
- to use this struct, we must use SA_SIGINFO flag and associate our handler with sa_sigaction instead of sa_handler
- SA_RESTART restarts interrupted syscalls

6. string_handling.c
split(char* input, char* delim, char* output[])
- First call to strtok takes the source string and delimiter character
- Subsequent calls to strok take NULL as the source string since strtrok maintains a static variable to the string
- strtok is repeatedly called in a while-loop 
- The output array is NULL terminated
- The number of tokens split is returned

is_blank_line(char* input)
- Checks if the input consists of all spaces, tabs and newlines

7. error_handling.c 
- Just contains wrappers around syscalls 
- Collate in one place to reduce code repetition