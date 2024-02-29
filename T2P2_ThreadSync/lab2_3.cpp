#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <locale.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

/*
* pthread_mutex_trylock()
*/

int flag1 = 1;
int flag2 = 1;
pthread_mutex_t mutex;

void sig_handler(int signo) {
    printf("\nExiting: SIGINT %d\n", signo);
    pthread_mutex_destroy(&mutex);
    exit(0);
}

void* thread1(void* args) {
    printf("поток 1 начал работу\n");
    while (flag1)
    {
        while(true) {
            if (!flag1) pthread_exit(NULL);
            int busy = pthread_mutex_trylock(&mutex);
            if (busy) {
                sleep(1);
            } else {
                break;
            }
        }
        // Критический участок
        for(int i = 0; i < 10; i++) {
            putchar('1');
            fflush(stdout);
            sleep(1);
        }
        // Конец критического участка
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    printf("поток 1 закончил работу\n");
    pthread_exit(NULL);
}

void* thread2(void* args) {
    printf("поток 2 начал работу\n");
    while (flag2)
    {
        while(true) {
            if (!flag2) pthread_exit(NULL);
            int busy = pthread_mutex_trylock(&mutex);
            if (busy) {
                sleep(1);
            } else {
                break;
            }
        }
        // Критический участок
        for(int i = 0; i < 10; i++) {
            putchar('2');
            fflush(stdout);
            sleep(1);
        }
        // Конец критического участка
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    printf("поток 2 закончил работу\n");
    pthread_exit(NULL);
}

int main() {
    setlocale(LC_ALL, "ru");
    signal(SIGINT, sig_handler);
    printf("программа начала работу\n");

    pthread_t id1 = 1;
    pthread_t id2 = 2;

    pthread_mutex_init(&mutex, NULL);
    
    //pthread_mutex_lock(&mutex);

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

    pthread_mutex_destroy(&mutex);
    printf("программа завершила работу\n");
}