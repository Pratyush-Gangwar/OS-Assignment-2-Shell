#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include <ctype.h>

#define MAXLEN 1000

struct history_entry {
    char* command;
    pid_t pid;
    struct timeval start_time;
    struct timeval end_time;
};

int split(char* input, char* delim, char* output[]);
bool is_blank_line(char* input);

int launch_background(char* input);
int launch_pipe(char* input);
int launch_normal(char* input);

void exec_wrapper(char* args[]);

void add_history_entry(struct history_entry* entry);
void print_history_entry_names();
void print_history_entry_details();

void set_entry_command(char* input, struct history_entry* entry);
void set_entry_start(struct history_entry* entry);
void set_entry_end(struct history_entry* entry);
struct history_entry* get_entry_by_pid(int pid);

void setup_signal_handlers();