#include "shell_utils.h"

extern struct history_entry* entries[];
static int history_idx = 0;

void add_history_entry(struct history_entry* entry) {
    entries[history_idx] = entry;
    history_idx = (history_idx + 1) % (MAXLEN); // If you exceed (MAXLEN), overwrite from the beginning
}

void print_history_entry_names() {
    for(int i = 0; i < history_idx; i++) {
        printf("%s\n", entries[i]->command);
    }
}

void print_history_entry_details() {
    printf("\n"); // So that it doesn't print on the same line as the prompt
    printf("PID \t Launch Time \t\t\t Duration (us) \t Command\n");

    for(int i = 0; i < history_idx; i++) {
        struct timeval start_time = entries[i]->start_time;
        struct timeval end_time = entries[i]->end_time;

        char* launch_time = ctime(&start_time.tv_sec);
        launch_time[ strlen(launch_time) - 1 ] = '\0'; // remove \n at the end

        int duration_micro = ( end_time.tv_sec  * 1000000 + end_time.tv_usec ) - ( start_time.tv_sec * 1000000 + start_time.tv_usec );
        printf("%d \t %s \t %d \t\t %s \n", 
                entries[i]->pid, launch_time, duration_micro, entries[i]->command );
    }
}