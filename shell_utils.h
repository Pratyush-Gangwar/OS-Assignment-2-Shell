#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>

#define MAXLEN 1000

struct history_entry {
    char* command;
    pid_t pid;
    struct timeval start_time;
    struct timeval end_time;
};

int split(char* input, char* delim, char* output[]);

int launch_background(char* input);
int launch_pipe(char* input);
int launch_normal(char* input);

void add_history_entry(struct history_entry* entry);
void print_history_entry_names();
void print_history_entry_details();

void setup_signal_handlers();