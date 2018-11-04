#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, const char* msg, const char* file, int line)
{
    if (!cond)
    {
        perror(msg);
        fprintf(stderr, "%s:%d\n", file, line);
        exit(EXIT_FAILURE);
    }
}
bool readFile(const char* fpath);
bool writeFile(const char* fpath);
bool appendFile(const char* fpath);


int main(int argc, const char* argv[])
{
    osAssert(argc == 3, "wrong number of parameters provided");
    if(strcmp(argv[1], "-r") == 0)
    {
        osAssert(readFile(argv[2]), "read from file failed");
        return 0;
    }
    if(strcmp(argv[1], "-w") == 0)
    { 
        osAssert(writeFile(argv[2]), "write to file failed");
        return 0;
    }
    if(strcmp(argv[1], "-a") == 0)
    { 
        osAssert(appendFile(argv[2]), "append failed");
        return 0;
    }
    osAssert(1 == 0, "Wrong parameters given");
}
bool readFile(const char* fpath)
{
    int fd = open(fpath, O_RDONLY);
    osAssert(-1 != fd, "open failed");

    static const uint32_t memBufSize = 1U << 13;
    char* memBuf = malloc(memBufSize);

    if(NULL == memBuf)
        return false;
    
    int32_t readBytes;
    while((readBytes = read(fd, memBuf, memBufSize) > 0))
    {
        if(write(STDOUT_FILENO, memBuf, memBufSize) < 0)
        {
            free(memBuf);
            close(fd);
            return false;
        }
    }
    free(memBuf);
    close(fd);
    return 0 == readBytes;
}

bool writeFile(const char* fpath)
{
    int fd = open(fpath, O_WRONLY);
    osAssert(fd != -1, "open failed");

    static const uint32_t memBuffSize = 1U << 13;
    char* memBuff = malloc(memBuffSize);

    if(NULL == memBuff)
        return false;

    int32_t readBytes;
    while ((readBytes = read(STDIN_FILENO, memBuff, memBuffSize)) > 0)
    {
        if(write(fd, memBuff, memBuffSize) < 0)
        {
            free(memBuff);
            close(fd);
            return false;
        }
    }
    free(memBuff);
    close(fd);
    return 0 == readBytes;
}
bool appendFile(const char* fpath)
{
    int fd = open(fpath, O_APPEND | O_WRONLY);
    osAssert(-1 != fd, "open failed");

    static const uint32_t memBuffSize = 1U << 13;
    char* memBuff = malloc(memBuffSize);

    if(NULL == memBuff)
    {
        close(fd);
        free(memBuff);
        return false;
    }

    int32_t readBytes;
    while ((readBytes = read(STDIN_FILENO, memBuff, memBuffSize)) > 0)
    {
        if(write(fd, memBuff, memBuffSize) < 0)
        {
            close(fd);
            free(memBuff);
            return false;
        }
    } 
    free(memBuff);
    close(fd);
    return readBytes == 0;
}