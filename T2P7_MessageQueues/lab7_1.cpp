#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <locale.h>
#include <signal.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/param.h>
#include <errno.h>

/*
* SVID Message Queue Writer
* char* get_current_dir_name(void)
*/

int flag = 1;
int mqid;

const char keyname[] = "lab1";

typedef struct {
    long mtype = 1;
    char buff[256];
} TMessage;

void sig_handler(int signo) {
    printf("\nExiting: SIGINT %d\n", signo);
    msgctl(mqid, IPC_RMID, NULL);
    exit(0);
}

void* thread1(void* args) {
    printf("поток 1 начал работу\n");
    while (flag)
    {
        char* buf = get_current_dir_name();
        TMessage message;
        int len = MIN(strlen(buf), sizeof(message.buff));
        memcpy(message.buff, buf, len);
        int r = msgsnd(mqid, &message, len, IPC_NOWAIT);
        if (r < 0) {
            perror("msgsnd");
        } else {
            printf("write: %s\n", message.buff);
        }
        free(buf);
        sleep(1);
    }
    printf("поток 1 закончил работу\n");
    pthread_exit(NULL);
}

int main() {
    setlocale(LC_ALL, "ru");
    signal(SIGINT, sig_handler);
    printf("программа начала работу\n");

    pthread_t id1 = 1;
    key_t key = ftok(keyname, 'X');
    mqid = msgget(key, IPC_CREAT | 0644);

    pthread_create(&id1, NULL, thread1, NULL);
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    flag = 0;

    pthread_join(id1, NULL);
    printf("поток 1 завершился\n");

    msgctl(mqid, IPC_RMID, NULL);

    printf("программа завершила работу\n");
}