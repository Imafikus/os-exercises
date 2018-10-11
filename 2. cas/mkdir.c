#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

bool osCreateDirectory(const char *dirPath);

osErrorFatal(bool cond, const char *msg, const char *file, int line)
{
    if(!cond)
    {
        perror(msg);
        fprintf(stderr, "%s:%d\n", file, line);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char const **argv)
{
    osAssert(2 <= argc, "Argument missing. Usage: ./mkdir.out directoryName");
    osAssert(osCreateDirectory(argv[1]), "Create Directory Failed");
    return 0;
}

bool osCreateDirectory(const char *dirPath)
{
    static mode_t mode = 0755;// _t tipovi su intovi, definise prava pristupa datoteci
    return mkdir(dirPath, mode) >= 0; //mkdir vraca -1 ako je fejlovao
}
