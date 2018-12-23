#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

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
int main(int argc, char** argv)
{
    osAssert(-1 != execlp("ls", "ls", NULL), "execl failed");


}