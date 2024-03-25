#include <stdio.h>
#include <locale.h>
#include <signal.h>
#include <stdlib.h>

/*
* Child program.
* execlp()
*/

void sig_handler(int signo) {
    printf("\nExiting: SIGINT %d\n", signo);
    exit(0);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");
    signal(SIGINT, sig_handler);

    printf("Child: parent's PID is %d\n", getppid());
    printf("Child: my PID is %d\n", getpid());

    for (int i = 0; i < argc; i++) {
        printf("Child: argument %d = %s\n", i + 1, argv[i]);
        sleep(1);
    }

    return 7;
}