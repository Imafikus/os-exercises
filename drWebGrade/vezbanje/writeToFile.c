#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <dirent.h>
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

void writeToFile(const char* fpath);

int main(int argc, char** argv)
{
    osAssert(2   == argc, "wrong num of params");
    
    writeToFile(argv[1]);
    return 0;
}

void writeToFile(const char* fpath)
{
    int fd = open(fpath, O_RDONLY);
    osAssert(-1 != fd, "open failed");
    osAssert(-1 != lseek(fd, 4, SEEK_SET), "lseek failed");

    int bytesRead = 0;
    int buffSize = 1U << 13;

    char* buff = malloc(buffSize);
    while((bytesRead = read(fd, buff, buffSize)) > 0)
    {
        osAssert(write(STDOUT_FILENO, buff, buffSize), "write failed");
    }
    osAssert(bytesRead == 0, "read failed");
    free(buff);
    close(fd);
}
