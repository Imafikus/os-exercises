#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>          
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)


bool osFileOpen(const char *filePath, const char *mode, int *fd);

void osErrorFatal(bool cond, const char *msg, const char *file, int line)
{
    if (!cond)
    {
        perror(msg);
        fprintf(stderr, "%s:%d\n", file, line);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char const **argv)
{
    osAssert(2 == argc, "Invalid Arguments. Usage ./mkfile.out fileName");

    // file descriptor, koristi se da znamo da li je sve proslo kako treba, radi kao FILE*
    int fd;

    // samo pravi fajl sa "w" privilegijom
    osAssert(osFileOpen(argv[1], "w", &fd), "File Create");

    // posebna komanda koja zatvara file descriptor (fd), obavezno zatvarati
    close(fd);
    return 0;
}
bool osFileOpen(const char *filePath, const char *mode, int *fd)
{
    static mode_t defaultMode = 0644; //mode_t je samo int, sluzi za postavljanje permisija

    int flags = 0;
    switch(mode[0])
    {
        //svi O_* parametri su binarni stepeni dvojke, pa konjukcijom dobijamo odgovarajuce sifre
        //ako njih ima vise
        case 'r':
            flags |= '+' == mode[1] ? O_RDWR : O_RDONLY;
            break;
        case 'w':
            flags |= '+' == mode[1] ? O_RDWR : O_RDONLY;
            flags |= O_TRUNC;
            flags |= O_CREAT;
            break;
        case 'a':
            flags |= '+' == mode[1] ? O_RDWR : O_RDONLY;
            flags |= O_APPEND;
            flags |= O_CREAT;
            break;
        default:
            return false;
    }
    *fd = open(filePath, flags, defaultMode);
    return *fd >= 0;
}