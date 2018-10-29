#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <sys/stat.h>
#include <limits.h>

#define DAY_IN_SECS (60*60*24)

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, const char *msg, const char *file, int line)
{
    if(!cond)
    {
        perror(msg);
        fprintf(stderr, "%s:%d\n", file, line);
        exit(EXIT_FAILURE);
    }
}

unsigned osNumDaysFileModified(const char *fpath);

int main(int argc, const char **argv)
{
    osAssert(2 == argc, "Incorrect parameters given.");
    printf("%d\n", osNumDaysFileModified(argv[1]));
}

unsigned osNumDaysFileModified(const char *fpath)
{

    struct stat fileStat;
    osAssert(-1 != stat(fpath, &fileStat), "stat failed");

    time_t curTimeInSec = time(NULL);
    
    //st_mtime cuva koliko je vremena proteklo od modifikacije fajla
    time_t diffInSec = curTimeInSec - fileStat.st_mtime;

    unsigned lastModified = diffInSec / DAY_IN_SECS;
    return(lastModified);

}