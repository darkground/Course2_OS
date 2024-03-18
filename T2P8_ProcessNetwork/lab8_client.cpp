#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <locale.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

/*
* UNIX TCP Socket Client
* char* get_current_dir_name(void)
*/

int number = 1;
int client_fd;
sockaddr_un server_addr, client_addr;

int conn_flag = 1;
int send_flag = 0;
int recv_flag = 0;
// 1 = thread is running
// 0 = thread didn't start
// -1 = thread needs joining

pthread_t conn_id = 1;
pthread_t send_id = 2;
pthread_t recv_id = 3;

const char servname[] = "p8server.soc";
const char hostname[] = "p8client.soc";
const int reuseaddr = 1;

void sig_handler(int signo) {
    printf("\nExiting: SIGINT %d\n", signo);
    shutdown(client_fd, SHUT_RDWR);
    close(client_fd);
    unlink(hostname);
    exit(0);
}

void* recv_thread(void* args) {
    printf("поток приема запросов начал работу\n");
    char* rcvbuf[128];
    while(recv_flag > 0) {
        int r = recv(client_fd, rcvbuf, sizeof(rcvbuf), 0);
        if (r == -1) {
            if (errno != EAGAIN) {
                perror("recv");
                sleep(1);
            }
        } else if (r == 0) {
            printf("recv: Connection closed\n");
            recv_flag = -1;
            send_flag = -1;
        } else {
            printf("recv: \"%s\"\n", rcvbuf);
        }
    }
    printf("поток приема запросов закончил работу\n");
    pthread_exit(NULL);
}

void* send_thread(void* args) {
    printf("поток обработки запросов начал работу\n");
    char sndbuf[128];
    while(send_flag > 0) {
        sprintf(sndbuf, "[%d]: ", number);
        int sr = send(client_fd, sndbuf, sizeof(sndbuf), 0);
        if (sr != -1) {
            printf("send: \"%s\"\n", sndbuf);
            number++;
        } else {
            perror("send");
        }
        sleep(1);
    }
    printf("поток обработки запросов закончил работу\n");
    pthread_exit(NULL);
}

void* conn_thread(void* args) {
    printf("поток установления соединения начал работу\n");
    socklen_t client_len = sizeof(server_addr);
    while(conn_flag > 0) {        
        if (connect(client_fd, (struct sockaddr *) &server_addr, client_len) == -1) {
            perror("connect");
            sleep(1);
        } else {
            printf("Connected to \"%s\"\n", server_addr.sun_path);
            getsockname(client_fd, (struct sockaddr *) &server_addr, &client_len);
            printf("Socket address: \"%s\"\n", server_addr.sun_path);
            conn_flag = -1;
            recv_flag = 1;
            send_flag = 1;
            pthread_create(&recv_id, NULL, recv_thread, NULL);
            pthread_create(&send_id, NULL, send_thread, NULL);
        }
    }
    printf("поток установления соединения закончил работу\n");
    pthread_exit(NULL);
}

int main() {
    setlocale(LC_ALL, "ru");
    signal(SIGINT, sig_handler);
    printf("программа начала работу\n");

    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    fcntl(client_fd, F_SETFL, O_NONBLOCK);
    server_addr.sun_family = AF_UNIX;
    client_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, servname, sizeof(server_addr.sun_path) - 1);
    strncpy(client_addr.sun_path, hostname, sizeof(client_addr.sun_path) - 1);
    bind(client_fd, (struct sockaddr *) &client_addr, sizeof(client_addr));

    pthread_create(&conn_id, NULL, conn_thread, NULL);

    printf("программа ждет нажатия клавиши\n");
    getchar();
    printf("клавиша нажата\n");

    // Check if threads do exist and need joining (avoiding segmentation fault)
    if (recv_flag != 0) {
        recv_flag = 0;
        pthread_join(recv_id, NULL);
        printf("поток приема запросов завершился\n");
    }
    if (send_flag != 0) {
        send_flag = 0;
        pthread_join(send_id, NULL);
        printf("поток обработки запросов завершился\n");
    }
    if (conn_flag != 0) {
        conn_flag = 0;
        pthread_join(conn_id, NULL);
        printf("поток ожидания соединений завершился\n");
    }

    shutdown(client_fd, SHUT_RDWR);
    close(client_fd);
    unlink(hostname);

    printf("программа завершила работу\n");
}