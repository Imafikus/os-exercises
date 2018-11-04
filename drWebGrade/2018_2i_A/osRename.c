#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
int main (int argc, const char **argv)
{
    osAssert(3 == argc, "Wrong parameters");

    struct stat finfo;
    osAssert(stat(argv[1], &finfo) >=0, "stat failed");

    if(S_ISDIR(finfo.st_mode))
    {
        char *realPath = realpath(argv[1], NULL);
        osAssert(NULL != realPath, "realpath failed");
        
        char* beginPtr = realPath;
        char* ptrToLast = strrchr(realPath, '/');

        char* copiedString = malloc(PATH_MAX);
        strncpy(copiedString, realPath, ptrToLast - beginPtr+1);

        printf("whole string: %s\n", realPath);
        printf("copied string: %s\n", copiedString);

        char* newPath = copiedString;
        strcat(newPath, argv[2]);
        printf("new path: %s\n", copiedString);
        

        osAssert(rename(realPath, newPath) >=0, "rename failed");
        return 0;
    }
    else
        osAssert(1 == 0, "file is not a directory");
}