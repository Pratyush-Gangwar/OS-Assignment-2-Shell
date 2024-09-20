#include "shell_utils.h"

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

void read_user_input(char* input) {
    fgets(input, MAXLEN, stdin); // gets is deprecated
    input[ strlen(input) - 1 ] = '\0'; // last character is newline
}

int launch(char* input) {
    int status;

    if ( strchr(input, '&') != NULL ) {
        status = launch_background(input);
    }

    else if ( strchr(input, '|') != NULL ) {
        status = launch_pipe(input);
    }

    else {
        status = launch_normal(input);
    }

    return status;
}

void shell_loop() {
    int status;
    char input[MAXLEN];

    do {
        print_prompt();
        read_user_input(input);
        status = launch(input);
    } while (status == 0);

}

int main() {
    shell_loop();
}