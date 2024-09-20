#include "shell_utils.h"

static int history_idx = 0;

void add_history_entry(struct history_entry* entry, struct history_entry* entries[]) {
    entries[history_idx] = entry;
    history_idx = (history_idx + 1) % (MAXLEN); // If you exceed (MAXLEN), overwrite from the beginning
}

void print_history_entries(struct history_entry* entries[]) {
    for(int i = 0; i < history_idx; i++) {
        printf("%s\n", entries[i]->command);
    }
}