#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#define SECS_PER_DAY (60*60*24)

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

int timeDiff(const char* fpath);

int main(int argc, char** argv)
{
    osAssert(2 == argc, "wrong num of params");
    int days = timeDiff(argv[1]);
    printf("%d\n", abs(days));
}

int timeDiff(const char* fpath)
{
    struct stat finfo;
    osAssert(stat(fpath, &finfo) != 1, "stat failed");
    osAssert(S_ISREG(finfo.st_mode), "file not regular");

    int diff = (int)((finfo.st_mtime - finfo.st_atime)/SECS_PER_DAY); 
    
    return diff;

}