#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, char* msg, char* file, int line)
{
    if(!cond)
    {
        perror(msg);
        fprintf(stderr, "%s: %d", file, line);
        exit(EXIT_FAILURE);
    }
}
int main(int argc, char** argv)
{
    pid_t childPid = fork();
    osAssert(-1 != childPid, "fork failed");

    if (childPid > 0)
        printf("This is parent process\n");
    else
        printf("This is child process\n");

    printf("Both processes to this!\n");
}