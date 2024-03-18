#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <locale.h>
#include <string.h>
#include <malloc.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>

/*
* char* get_current_dir_name(void)
*/

int flag1 = 1;
int flag2 = 1;
int filedes[2];

void sig_handler(int signo) {
    printf("\nExiting: SIGINT %d\n", signo);
    close(filedes[0]);
    close(filedes[1]);
    exit(0);
}

void* thread1(void* args) {
    printf("поток 1 начал работу\n");
    char* buf;
    while (flag1)
    {
        buf = get_current_dir_name();
        int res = write(filedes[1], buf, strlen(buf));
        if (res == -1) { // Error
            perror("write_fail");
        }
        free(buf);
        sleep(2);
    }
    printf("поток 1 закончил работу\n");
    pthread_exit(NULL);
}

void* thread2(void* args) {
    printf("поток 2 начал работу\n");
    char buf[64];
    while (flag2)
    {
        for (int i = 0; i < sizeof(buf); i++) { 
            buf[i] = '\0';
        }
        int res = read(filedes[0], buf, 64);
        if (res > 0) { // Success
            printf("%s (%d symbols)\n", buf, res);
            fflush(stdout);
        }
        if (res == -1) { // Error
            perror("read_fail");
        }
        if (res == 0) { // EOF
            printf("end of file\n");
        }
        sleep(1);
    }
    printf("поток 2 закончил работу\n");
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");
    signal(SIGINT, sig_handler);
    printf("программа начала работу\n");

    pthread_t id1 = 1;
    pthread_t id2 = 2;
    int pipe_type = 0;

    if (argc == 2) {
        if (strcmp(argv[1], "pipe") == 0) {
            pipe_type = 0;
        } 
        if (strcmp(argv[1], "pipe2") == 0) {
            pipe_type = 1;
        }
        if (strcmp(argv[1], "fcntl") == 0) {
            pipe_type = 2;
        }
    }

    switch (pipe_type) {
    case 0:
        printf("using pipe\n");
        pipe(filedes);
        break;
    case 1:
        printf("using pipe2\n");
        pipe2(filedes, O_NONBLOCK);
        break;
    case 2:
        printf("using pipe + fcntl\n");
        pipe(filedes);
        fcntl(filedes[0], F_SETFL, O_NONBLOCK);
        fcntl(filedes[1], F_SETFL, O_NONBLOCK);
        break;
    }

    pthread_create(&id1, NULL, thread1, NULL);
    pthread_create(&id2, NULL, thread2, NULL);
    
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    
    flag1 = 0;
    flag2 = 0;

    pthread_join(id1, NULL);
    printf("поток 1 завершился\n");
    pthread_join(id2, NULL);
    printf("поток 2 завершился\n");

    close(filedes[0]);
    close(filedes[1]);

    printf("программа завершила работу\n");
}