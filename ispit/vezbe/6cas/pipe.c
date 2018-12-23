#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>


#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, char* msg, char* file, int line)
{
    if(!cond)
    {
        perror(msg);
        fprintf(stderr, "%s: %d\n", file, line);
        exit(EXIT_FAILURE);
    }
}

#define PIPE_RD_END (0)
#define PIPE_WR_END (1)
#define MAX_LINE_LEN (128)

int main(int argc, char** argv)
{
    int pipeFds[2];
    osAssert(-1 != pipe(pipeFds), "making pipe failed");

    pid_t child = fork();
    osAssert(-1 != child, "fork failed");

    if(child > 0)
    {
        close(pipeFds[PIPE_RD_END]);

        char* line = NULL;
        size_t len = 0;

        osAssert(-1 != getline(&line, &len, stdin), "get line failed");
        osAssert(len <= MAX_LINE_LEN, "line longer than max");
        
        osAssert(-1 != write(pipeFds[PIPE_WR_END], line, len), "len failed");
        
        osAssert(-1 != wait(NULL), "wait failed");

        close(pipeFds[PIPE_WR_END]);
        free(line);
    }
    else
    {
        close(pipeFds[PIPE_WR_END]);

        char buf[MAX_LINE_LEN];
        osAssert(-1 != read(pipeFds[PIPE_RD_END], buf, sizeof buf), "read failed");

        close(pipeFds[PIPE_RD_END]);

        printf("Text is: %s\n", buf);
        exit(0);
    }
    return 0;
}