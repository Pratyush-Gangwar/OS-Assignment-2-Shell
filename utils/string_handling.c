#include "shell_utils.h"

// no errors given by strtok or isspace

// return value is number of parts
int split(char* input, char* delim, char* output[]) {
    output[0] = strtok(input, delim);
    char* part = strtok(NULL, delim); // strtok has a static variable pointng to the string we passed previously
    
    int i = 1; // 0-th index was already populated
    while (part != NULL) {
        output[i] = part;
        part = strtok(NULL, delim); 
        i++;
    }

    output[i] = NULL; // ensure NULL termination
    return i;
}

bool is_blank_line(char* input) {

    while(*input != '\0') {
        if (!isspace(*input)) { // checks for space, tab, new line and more
            return false;
        }

        input++;
    }

    return true;
}