#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#define MAXLEN 1000

int split(char* input, char* delim, char* output[]);

int launch_background(char* input);
int launch_pipe(char* input);
int launch_normal(char* input);