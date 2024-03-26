#include <ev.h>
#include <stdio.h>
#include <locale.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

/*
* Event Looped Parent program.
* execlp()
*/

ev_child child_watcher;
ev_io io_watcher;
pid_t pid;

void sig_handler(int signo) {
    printf("\nExiting: SIGINT %d\n", signo);
    exit(0);
}

static void stdin_cb(EV_P_  ev_io *w, int revents)
{
    ev_io_stop(EV_A_ w);
    ev_break(EV_A_ EVBREAK_ALL);
    kill(pid, SIGTERM);
    // Killing the process so it won't print after program exiting.
    printf("клавиша нажата\n");
}

static void child_cb(EV_P_ ev_child *w, int revents)
{
    ev_child_stop(EV_A_ w);
    ev_break(EV_A_ EVBREAK_ALL);
    printf("Parent: child exited with code: %d\n", WEXITSTATUS(w->rstatus));
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

    pid = fork();
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
            printf("Child: Provide 3-5 arguments\n");
            exit(1);
        }
    default:
        ev_child_init(&child_watcher, child_cb, pid, 0);
        ev_child_start(EV_DEFAULT_ &child_watcher);
        ev_io_init(&io_watcher, stdin_cb, STDIN_FILENO, EV_READ);
        ev_io_start(EV_DEFAULT_ &io_watcher);
        printf("Parent: parent's PID is %d\n", getppid());
        printf("Parent: my PID is %d\n", getpid());
        printf("Parent: child's PID is %d\n", pid);
        break;
    }

    printf("программа ждет нажатия клавиши\n");

    ev_run(EV_DEFAULT_ 0);

    printf("программа закончила работу\n");
    return 0;
}