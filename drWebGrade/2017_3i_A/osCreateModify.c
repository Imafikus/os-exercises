#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <errno.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, const char *msg, const char *file, int line)
{
    if(!cond)
    {
        perror(msg);
        fprintf(stderr, "%s:%d", file, line);
        exit(EXIT_FAILURE);
    }
}

void createModifyFile(const char *fpath, const char *accessMode);
void createModifyDir(const char *fpath, const char *accessMode);

int main(int argc, const char **argv)
{
    osAssert(argc == 4, "Usage ./file -[f/d] file_name access privileges");
    osAssert(argv[1][0] == '-', "Assert - fali");
    osAssert(argv[1][1] == 'f' || argv[1][1] == 'd', "Fali f ili d");
    
    //if (argv[1][1] == 'f')
    //    createModifyFile(argv[1], argv[2]);
    if (argv[1][1] == 'd')
        createModifyDir(argv[2], argv[3]);    
    
    if (argv[1][1] == 'f')
        createModifyFile(argv[2], argv[3]);    
}



void createModifyDir(const char *fpath, const char *accessMode)
{
    long mode = strtol(accessMode, NULL, 8);
    
    mode_t oldMask = umask(0);
    int checkMkdir = mkdir(fpath, mode);
    if (errno == EEXIST)
    {
        int checkChmod = chmod(fpath, mode);
        osAssert(checkChmod >= 0 , "chmod failed");
        
        //Zasto ovo ispod ne radi?
        //errno = 0;
        //osAssert(chmod(fpath, mode), "chmod failed");
    }
    else
        osAssert(checkMkdir >= 0 , "mkdir failed");
    umask(oldMask);
}
void createModifyFile(const char *fpath, const char *accessMode)
{
    long mode = strtol(accessMode, NULL, 8);

    mode_t oldMask = umask(0);
    
    int fd = open(fpath, O_CREAT | O_EXCL, mode);
    if (errno == EEXIST)
    {
        int checkChmod = chmod(fpath, mode);
        osAssert(checkChmod >= 0 , "chmod failed");
        
    }
    else
        osAssert(fd >= 0 , "open failed");

    umask(oldMask);
}




