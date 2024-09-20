#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

#define MAXLEN 1000

struct history_entry {
    char* command;
    pid_t pid;
    time_t start_time;
    time_t end_time;
};

int split(char* input, char* delim, char* output[]);

int launch_background(char* input);
int launch_pipe(char* input);
int launch_normal(char* input, struct history_entry entries[]);

void add_history_entry(struct history_entry entry, struct history_entry entries[]);
void print_history_entries(struct history_entry entries[]);