#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, const char* msg, const char* file, int line)
{
    if(!cond)
    {
        perror(msg);
        fprintf(stderr, "%s:%d\n", file, line);
        exit(EXIT_FAILURE);
    }
}

void moveFile(const char* rPath1, const char* rPath2);

int main(int argc, const char** argv[])
{
    osAssert(3 == argc, "wrong params");
    moveFile(argv[1], argv[2]);
}
void moveFile(const char* rPath1, const char* rPath2)
{
    char* src = realpath(rPath1, NULL);
    osAssert(NULL != src, "src path failed");
    
    char* dest = realpath(rPath2, NULL);
    //osAssert(NULL != dest, "dest path failed");

    if (NULL != dest)
    {
        osAssert(strcmp(src, dest) != 0, "same file");
    }
    osAssert(rename(rPath1, rPath2) != -1, "rename failed");
    
    free(src);
    free(dest);
    return 0;
}