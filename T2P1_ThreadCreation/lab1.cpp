#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <locale.h>
#include <string.h>

struct t_arg {
    int flag = 1;
    char chr = '0';
};

void* thread1(void* args) {
    t_arg* arg = (t_arg*) args;
    printf("поток 1 начал работу\n");
    while (arg->flag) {
        putchar(arg->chr);
        fflush(stdout);
        sleep(1);
    }
    printf("поток 1 закончил работу\n");
    pthread_exit((void*) 1);
}

void* thread2(void* args) {
    t_arg* arg = (t_arg*) args;
    printf("поток 2 начал работу\n");
    while (arg->flag) {
        putchar(arg->chr);
        fflush(stdout);
        sleep(1);
    }
    printf("поток 2 закончил работу\n");
    pthread_exit((void*) 2);
}

int main() {
    setlocale(LC_ALL, "ru");
    printf("программа начала работу\n");

    pthread_t id1 = 1;
    pthread_t id2 = 2;
    t_arg arg1;
    t_arg arg2;
    arg1.chr = '1';
    arg2.chr = '2';
    
    // Установим атрибут второго потока DetachState = Detached,
    // что должно привести к ошибке EINVAL (22) при использовании
    // pthread_join на id этого потока.
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_create(&id1, NULL, thread1, &arg1);
    pthread_create(&id2, &attr, thread2, &arg2);
    
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    
    arg1.flag = 0;
    arg2.flag = 0;
    void* exit1;
    void* exit2;
    int exit_result1 = pthread_join(id1, &exit1);
    int exit_result2 = pthread_join(id2, &exit2);

    if(exit_result1 != 0) {
        printf("pthread1_join fail: %s (%d)\n", strerror(exit_result1), exit_result1);
    } else {
        printf("поток 1 завершился с кодом %d\n", (int*) exit1);
    }
    if(exit_result2 != 0) {
        printf("pthread2_join fail: %s (%d)\n", strerror(exit_result2), exit_result2);
    } else {
        printf("поток 2 завершился с кодом %d\n", (int*) exit2);
    }
    pthread_attr_destroy(&attr);
    printf("программа завершила работу\n");
}