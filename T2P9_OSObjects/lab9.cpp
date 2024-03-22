#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <sys/wait.h>
#include <sys/utsname.h>

const int stack_size = 1024 * 1024;

static int childFunc(void *arg) {
    char newname[] = "Project9";
    sethostname(newname, sizeof(newname));

    utsname name;
    uname(&name);
    printf("Child: Hostname: \"%s\"\n", name.nodename);
    return 0;
}

int main() {
    char *stack;
    char *stackTop;
    stack = (char*)malloc(stack_size);
    stackTop = stack + stack_size;


    int child_pid = clone(childFunc, stackTop, CLONE_NEWUTS | SIGCHLD, NULL);
    if (child_pid == -1){
        perror("clone");
        exit(0);
    } else {
        utsname name;
        uname(&name);
        printf("Parent: Hostname: \"%s\"\n", name.nodename);
        
        waitpid(child_pid, NULL, 0);

        uname(&name);
        printf("Parent: Hostname: \"%s\"\n", name.nodename);
    }

    
}