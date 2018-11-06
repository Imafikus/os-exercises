#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <ftw.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

#define UNUSED_VAR(x) ((void)x)

void osErrorFatal(bool cond, const char* msg, const char* file, int line)
{
    if(!cond)
    {
        perror(msg);
        fprintf(stderr, "%s:%d\n", file, line);
        exit(EXIT_FAILURE);
    }
}
int countExtensions(const char *fpath, const struct stat *sb,
                          int typeflag, struct FTW *ftwbuf);
int hasExtension(const char* file, const char* ext);

char* extension = NULL;
unsigned count = 0;

int main(int argc,  char** argv)
{
    osAssert(3 == argc, "wrong num of params");
    
    struct stat finfo;
    osAssert(stat(argv[1], &finfo) != -1, "stat failed");
    osAssert(S_ISDIR(finfo.st_mode), "not a dir");

    extension = argv[2];
    osAssert(nftw(argv[1], countExtensions, 50, FTW_PHYS) != -1, "nftw failed");
    printf("%d\n", count);
}

int hasExtension(const char* file, const char* ext)
{
    char* extPos = strrchr(file, '.');
    if(!strcmp(extPos, ext))
        return 1;
    else
        return 0;
}
int countExtensions(const char *fpath, const struct stat *sb,
                          int typeflag, struct FTW *ftwbuf)
{

    UNUSED_VAR(sb);
    UNUSED_VAR(ftwbuf);

    if(FTW_F == typeflag)
        count += hasExtension(fpath, extension);

    return 0;
}