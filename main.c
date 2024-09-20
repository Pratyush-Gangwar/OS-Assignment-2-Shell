#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXLEN 1000

void print_prompt() {
    char cwd[MAXLEN];
    if (getcwd(cwd, MAXLEN) == NULL) {
        perror("print_promot getcwd error: ");
        exit(1);
    }

    char username[MAXLEN];
    if( getlogin_r(username, MAXLEN) != 0 ) {
        perror("print_promt getlogin_r error: ");
        exit(1);
    }

    char machinename[MAXLEN];
    if( gethostname(machinename, MAXLEN) == -1) {
        perror("print_prompt gethostname error: ");
        exit(1);
    }

    printf("%s@%s:%s$ ", username, machinename, cwd);
}

void read_user_input() {

}

int launch() {

}

void shell_loop() {
    int status;

    do {
        print_prompt();
        read_user_input();
        status = launch();
    } while (status != 0);

}

int main() {
    shell_loop();
}