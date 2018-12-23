#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, char* msg, char* file, int line)
{
    if(!cond)
    {
        perror(msg);
        fprintf(stderr, "%s: %d\n", file, line);
    }
}

int main(int argc, char** argv)
{
    pid_t childPid = fork();

    osAssert(-1 != childPid, "fork failed");

    if(childPid > 0)
    {
        printf("Hello from parent, child pid is %ld, and my pid is %ld\n", (intmax_t)(childPid), (intmax_t)(getpid()) );
        osAssert(-1 != wait(NULL), "wait failed");
    }
    else
    {
        printf("Hello from child, my pid is %ld, and my pid is %ld\n", (intmax_t)(getpid()), (intmax_t)(getppid()) );
        exit(0);
    }

    printf("Parent executes this\n");
}