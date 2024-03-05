#include <stdio.h>
#include <locale.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

/*
* Parent program.
* execlp()
*/

void sig_handler(int signo) {
    printf("\nExiting: SIGINT %d\n", signo);
    exit(0);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");
    signal(SIGINT, sig_handler);
    printf("программа начала работу\n");
    
    char pth[] = "PATH=";
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));

    char modpath[sizeof(pth)+sizeof(cwd)];
    strcpy(modpath, pth);
    strcat(modpath, cwd);

    printf("Path to append: %s\n", modpath);
    putenv(modpath);

    pid_t pid = fork();
    int wstatus = -1;

    switch (pid)
    {
    case -1:
        perror("fork");
        break;
    case 0: 
        switch (argc)
        {
        case 4: // 3 arguments
            execlp("lab_child", argv[1], argv[2], argv[3], NULL);
            break;
        case 5: // 4 arguments
            execlp("lab_child", argv[1], argv[2], argv[3], argv[4], NULL);
            break;
        case 6: // 5 arguments
            execlp("lab_child", argv[1], argv[2], argv[3], argv[4], argv[5], NULL);
            break;
        default: 
            break;
        }
        return 0;
    default:
        printf("Parent: parent's PID is %d\n", getppid());
        printf("Parent: my PID is %d\n", getpid());
        printf("Parent: child's PID is %d\n", pid);
        while(waitpid(pid, &wstatus, WNOHANG) == 0) {
            printf("Parent: still waiting...\n");
            usleep(0.5 * 1000000);
        }
        printf("Parent: child exited with code %d\n", WEXITSTATUS(wstatus));
        break;
    }

    printf("программа закончила работу\n");
    return 0;
}