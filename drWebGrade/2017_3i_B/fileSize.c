#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>   

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BYTES_TO_KB (1024)
#define BYTES_TO_MB (1024 * 1024)
#define BYTES_TO_GB (1024 * 1024 * 1024)

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

void fileSize(char* fpath, const char* fsize);

int main(int argc, char** argv)
{
    osAssert(3 == argc, "wrong num of params");
    fileSize(argv[1], argv[2]);
}    

void fileSize(char* fpath, const char* fsize)
{
    struct stat finfo;
    osAssert(-1 != stat(fpath, &finfo), "stat failed");
    double size = 0;

    char* fileName = strrchr(fpath, '/');
    if(NULL == fileName)
        fileName = fpath;
    else
        fileName++;

    if(!strcmp("KB", fsize))
    {
        fprintf(stderr, "KB choosen\n");
        size = (double)(finfo.st_size *1.0 / BYTES_TO_KB);
    }
    else if(!strcmp("MB", fsize))
    {
        fprintf(stderr, "MB choosen\n");
        size = (double)(finfo.st_size *1.0 / BYTES_TO_MB);
    }
    else if(!strcmp("GB", fsize))
    {
        fprintf(stderr, "GB choosen\n");
        size = (double)(finfo.st_size *1.0 / BYTES_TO_GB);
    }
    else
    {
        osAssert(false, "incorrect extension given");        
    }

    size = ceil(size);
    printf("%s %d%s", fileName, (int)size, fsize);
    
}