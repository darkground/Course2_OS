#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <locale.h>
#include <signal.h>
#include <semaphore.h>

/*
* select()
*/

sem_t* sem;
int filedes;
const char out[] = "2";
const char sem_name[] = "/project5";

void sig_handler(int signo) {
    printf("\nExiting: SIGINT %d\n", signo);
    close(filedes);
    sem_post(sem);
    sem_unlink(sem_name);
    exit(0);
}

int isEnterPressed() {
    fd_set readfds;
    FD_SET(0, &readfds);

    timeval t;
    t.tv_sec = 1;
    t.tv_usec = 0;
    return select(1, &readfds, NULL, NULL, &t);
}

int main() {
    setlocale(LC_ALL, "ru");
    signal(SIGINT, sig_handler);
    printf("программа начала работу\n");

    sem = sem_open(sem_name, O_CREAT, 0644, 1);
    filedes = open("output", O_CREAT | O_APPEND | O_WRONLY);

    while (true) {
        sem_wait(sem);

        for (int i = 0; i < 10; i++) {
            printf("%s", out);
            fflush(stdout);
            write(filedes, out, sizeof(out));
            sleep(1);
        }

        sem_post(sem);

        if (isEnterPressed()) {
            printf("клавиша нажата\n");
            break;
        }
    }

    close(filedes);
    sem_close(sem);
    sem_unlink(sem_name);

    printf("программа завершила работу\n");
}