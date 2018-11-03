#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utime.h>

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

void changeTime(const char* fpath, const char* secs);


int main(int argc, const char** argv)
{
    osAssert(3 == argc, "Wrong usage.");
    printf("Prosao\n");
    changeTime(argv[1], argv[2]);

    return 0;
}
void changeTime(const char* fpath, const char* secs)
{
    struct stat finfo;
    osAssert(stat(fpath, &finfo) >= 0, "stat failed");

    time_t ep = atoi(secs);
    struct utimbuf new_times;
    
    new_times.actime = ep;
    new_times.modtime = ep;

    osAssert(utime(fpath, &new_times) >= 0, "utime failed");
    
    

}