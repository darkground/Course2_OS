#include <fcntl.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <locale.h>
#include <signal.h>
#include <string.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/param.h>

/*
* POSIX Memory Reader
* char* get_current_dir_name(void)
*/

int flag = 1;
int memfd;
sem_t* readsem;
sem_t* writesem;
void* laddr;

const char memname[] = "/project6";
const char readsemname[] = "/project6read";
const char writesemname[] = "/project6write";
const int size = 100;

void sig_handler(int signo) {
    printf("\nExiting: SIGINT %d\n", signo);
    sem_close(readsem);
    sem_unlink(readsemname);
    sem_close(writesem);
    sem_unlink(writesemname);
    munmap(laddr, size);
    close(memfd);
    shm_unlink(memname);
    exit(0);
}

void* thread1(void* args) {
    printf("поток 1 начал работу\n");
    char* shared = (char*)laddr;
    char buf[size];
    while (flag)
    {
        sem_wait(writesem);
        memcpy(buf, shared, size);
        printf("Read = %s\n", buf);
        sem_post(readsem);
    }
    printf("поток 1 закончил работу\n");
    pthread_exit(NULL);
}

int main() {
    setlocale(LC_ALL, "ru");
    signal(SIGINT, sig_handler);
    printf("программа начала работу\n");

    pthread_t id1 = 1;

    memfd = shm_open(memname, O_RDWR | O_CREAT, 0644);
    ftruncate(memfd, size);
    laddr = mmap(NULL, size, PROT_READ, MAP_SHARED, memfd, 0);
    readsem = sem_open(readsemname, O_CREAT, 0644, 0);
    writesem = sem_open(writesemname, O_CREAT, 0644, 0);

    pthread_create(&id1, NULL, thread1, NULL);
    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");
    flag = 0;

    pthread_join(id1, NULL);
    printf("поток 1 завершился\n");

    sem_close(readsem);
    sem_unlink(readsemname);
    sem_close(writesem);
    sem_unlink(writesemname);
    munmap(laddr, size);
    close(memfd);
    shm_unlink(memname);

    printf("программа завершила работу\n");
}