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
void countExtensions(const char* fpath, const char* ext, int* count);
int hasExtension(const char* file, const char* ext);

int main(int argc, const char** argv)
{
    osAssert(3 == argc, "wrong num of params");
    
    struct stat finfo;
    osAssert(stat(argv[1], &finfo) != -1, "stat failed");
    osAssert(S_ISDIR(finfo.st_mode), "not a dir");

    int count = 0;
    countExtensions(argv[1], argv[2], &count);
    printf("%d\n",count);
}

int hasExtension(const char* file, const char* ext)
{
    char* extPos = strrchr(file, '.');
    if(!strcmp(extPos, ext))
        return 1;
    else
        return 0;
}
void countExtensions(const char* fpath, const char* ext, int* count)
{
    struct stat finfo;
    osAssert(lstat(fpath, &finfo) != -1, "lstat failed");

    if(S_ISREG(finfo.st_mode))
        *count += hasExtension(fpath, ext);

    if(!S_ISDIR(finfo.st_mode))
        return;
    
    DIR* dir = opendir(fpath);
    osAssert(NULL != dir, "opendir failed");

    struct dirent *entry = NULL;
    while(NULL != (entry = readdir(dir)))
    {
        char* newPath = malloc(strlen(fpath) + strlen(entry->d_name) + 2);
        osAssert(NULL != newPath, "naking newPath failed");

        strcpy(newPath, fpath);
        strcat(newPath, "/");
        strcat(newPath, entry->d_name);

        if(!strcmp(".", entry->d_name) || !strcmp("..", entry->d_name))
        {
            free(newPath);
            continue;
        }

        countExtensions(newPath, ext, count);
        free(newPath);

    }

}