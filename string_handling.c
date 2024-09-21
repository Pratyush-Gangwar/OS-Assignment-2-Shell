#include "shell_utils.h"

// return value is number of parts
int split(char* input, char* delim, char* output[]) {
    output[0] = strtok(input, delim);
    char* part = strtok(NULL, delim);
    
    int i = 1;
    while (part != NULL) {
        output[i] = part;
        part = strtok(NULL, delim); 
        i++;
    }

    output[i] = NULL; // ensure NULL termination
    return i;
}