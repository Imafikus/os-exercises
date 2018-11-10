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
#include <errno.h>
#include <ftw.h>

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

void goThroughDir(const char* fpath);
int doShit (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf);

int main(int argc, char** argv)
{
    osAssert(2   == argc, "wrong num of params");
    
    struct stat finfo;
    osAssert(-1 != stat(argv[1], &finfo), "stat failed");
    osAssert(S_ISDIR(finfo.st_mode), "not a dir");

    //goThroughDir(argv[1]);
    osAssert(-1 != nftw(argv[1], doShit, 50, FTW_PHYS), "nftw failed");
    
    return 0;
}

void goThroughDir(const char* fpath)
{
    struct stat finfo;
    osAssert(-1 != lstat(fpath, &finfo), "stat failed");

    if(S_ISREG(finfo.st_mode))
    {
        printf("FILE IS REGULAR: %s\n", fpath);
        return;
    }
    if(!S_ISDIR(finfo.st_mode))
        return;

    DIR *dir = opendir(fpath);
    osAssert(NULL != dir, "opendir failed");

    struct dirent *entry;
    errno = 0;
    while(NULL != (entry = readdir(dir)))
    {
        char* newPath = malloc(strlen(fpath) + 2 + strlen(entry->d_name));
        osAssert(newPath != NULL, "malloc newPath failed");

        if(!strcmp("..", entry->d_name) || !strcmp(".", entry->d_name))
        {
            free(newPath);
            continue;
        }

        strcpy(newPath, fpath);
        strcat(newPath, "/");
        strcat(newPath, entry->d_name);

        goThroughDir(newPath);
    }

    osAssert(errno == 0, "readdir failed");
    osAssert(-1 != closedir(dir), "close dir failed");
}
int doShit (const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    (void)(sb); (void)(ftwbuf);
    if(typeflag == FTW_F)
        printf("Regular file: %s\n", fpath);

    return 0;
}