#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <locale.h>
#include <string.h>

/*
* Без средств синхронизации
*/

int flag1 = 1;
int flag2 = 1;

void* thread1(void* args) {
    printf("поток 1 начал работу\n");
    while (flag1)
    {
        // Критический участок
        for(int i = 0; i < 10; i++) {
            putchar('1');
            fflush(stdout);
            sleep(1);
        }
        // Конец критического участка
        sleep(1);
    }
    printf("поток 1 закончил работу\n");
    pthread_exit(NULL);
}

void* thread2(void* args) {
    printf("поток 2 начал работу\n");
    while (flag2)
    {
        // Критический участок
        for(int i = 0; i < 10; i++) {
            putchar('2');
            fflush(stdout);
            sleep(1);
        }
        // Конец критического участка
        sleep(1);
    }
    printf("поток 2 закончил работу\n");
    pthread_exit(NULL);
}

int main() {
    setlocale(LC_ALL, "ru");
    printf("программа начала работу\n");

    pthread_t id1 = 1;
    pthread_t id2 = 2;
    
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

    printf("программа завершила работу\n");
}