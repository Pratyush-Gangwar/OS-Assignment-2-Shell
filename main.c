#include "shell_utils.h"

#define RED "\033[31m"
#define RESET "\033[0m"

struct history_entry* entries[MAXLEN];

void print_prompt() {
    char cwd[MAXLEN];
    if (getcwd(cwd, MAXLEN) == NULL) {
        perror("print_prompt getcwd error: ");
        exit(1);
    }

    char username[MAXLEN];
    if( getlogin_r(username, MAXLEN) != 0 ) {
        perror("print_prompt getlogin_r error: ");
        exit(1);
    }

    char machinename[MAXLEN];
    if( gethostname(machinename, MAXLEN) == -1) {
        perror("print_prompt gethostname error: ");
        exit(1);
    }

    printf(RED "%s@%s:%s$ " RESET, username, machinename, cwd);
}

void read_user_input(char* input) {
    fgets(input, MAXLEN, stdin); // gets is deprecated
    input[ strlen(input) - 1 ] = '\0'; // last character is newline
}

int launch(char* input) {
    int status;

    if ( strchr(input, '&') != NULL ) {

        char* ampersand_ptr = strchr(input, '&');
        *ampersand_ptr = '\0';

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
    char input[MAXLEN];

    do {
        print_prompt();
        read_user_input(input);
        launch(input);
    } while (1);

}

int main() {
    setup_signal_handlers();
    shell_loop();
}