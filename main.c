#include <stdio.h>

#define MAXLEN 1000

void print_prompt() {
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