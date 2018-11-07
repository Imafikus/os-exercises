#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

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
void makeFD(const char* type, const char* fpath, const char* access);

int main(int argc, char** argv)
{
    osAssert(4 == argc, "wrong num of params");
    osAssert(!strcmp("-f", argv[1]) || !strcmp("-d", argv[1]), "usage -f or -d");
    makeFD(argv[1], argv[2], argv[3]);
    
    return 0;
}

void makeFD(const char* type, const char* fpath, const char* access)
{
    mode_t accessRights = strtol(access, NULL, 8);

    mode_t oldMask = umask(0);

    if(!strcmp("-d", type))
    {
        osAssert(mkdir(fpath, accessRights) != -1, "mkdir failed");
    }
    else
    { 
        osAssert(open(fpath,O_CREAT | O_EXCL, accessRights) != -1, "mk file failed");
    }
    umask(oldMask);

}