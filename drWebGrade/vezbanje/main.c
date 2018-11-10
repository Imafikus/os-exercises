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

#define SEC_IN_DAY (60*60*24)
#define MAX_DAY_NUM 30

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

bool osIsPublicFile(const char* fpath);
void osMkPublicDir(const char* dname);
unsigned osNumDaysFileModified(const char* fpath);
void osMoveFile(const char* srcPath, const char* destPath);
void osMakeBackup(const char* fpath, const char* dirPath);

int main(int argc, char** argv)
{
    osAssert(3   == argc, "wrong num of params");
    
    struct stat finfo;
    osAssert(-1 != (stat(argv[1], &finfo)), "stat failed");
    osAssert(S_ISDIR(finfo.st_mode), "not a dir");
    
    osMkPublicDir(argv[2]);

    char* absDirPath = realpath(argv[2], NULL);
    fprintf(stderr, "absDirPath is: %s\n", absDirPath);

    osMakeBackup(argv[1], absDirPath);
    return 0;
}

bool sIsPublicFile(const char* fpath)
{
    struct stat finfo;
    osAssert(-1 != stat(fpath, &finfo), "stat failed");
    if(S_ISREG(finfo.st_mode))
    {
        if((finfo.st_mode & S_IROTH) && (finfo.st_mode & S_IWOTH))
            return true;
    }
    else
        return false;
}
void osMkPublicDir(const char* dname)
{
    umask(0);
    mode_t accessRights = 0777;

    osAssert(-1 != mkdir(dname, accessRights), "mkdir failed");
}
unsigned osNumDaysFileModified(const char* fpath)
{
    struct stat finfo;
    osAssert(-1 != stat(fpath, &finfo), "stat failed");

    time_t curTime = time(NULL);
    osAssert(curTime != -1, "time failed");

    double daysSinceMod = ((double)curTime - (double)finfo.st_mtime) / SEC_IN_DAY;
    daysSinceMod = (unsigned)ceil(daysSinceMod);
    return daysSinceMod;

}

void osMoveFile(const char* srcPath, const char* destPath)
{
    struct stat finfoSrc;
    osAssert(-1 != stat(srcPath, &finfoSrc), "stat Src failed");

    struct stat finfoDest;
    int passedStat = stat(destPath, &finfoDest);

    char* srcAbsPath = realpath(srcPath, NULL);
    osAssert(NULL != srcAbsPath, "getting srcAbsPath failed");

    if(-1 != passedStat)
    {
        char* destAbsPath = realpath(destPath, NULL);
        osAssert(NULL != destAbsPath, "getting destAbsPath failed");

        fprintf(stderr, "Src abs path: %s\nDest abs path: %s\n", srcAbsPath, destAbsPath);
        osAssert(strcmp(destAbsPath, srcAbsPath), "same file");

    }

    osAssert(-1 != rename(srcPath, destPath), "rename failed");
}

void osMakeBackup(const char* fpath, const char* dirPath)
{
    if(osIsPublicFile(fpath))
    {
        if(osNumDaysFileModified(fpath) <= MAX_DAY_NUM)
        {
            char* absFilePath = realpath(fpath, NULL);
            osAssert(NULL != absFilePath, "getting abs path failed");
            osMoveFile(absFilePath, dirPath);
        }
    }

    struct stat finfo;
    osAssert(-1 != (stat(fpath, &finfo)), "stat failed");    
    if(!S_ISDIR(finfo.st_mode))
        return;
    
    DIR* dir = opendir(fpath);
    osAssert(NULL != dir, "opendir failed");

    struct dirent* entry = NULL;

    while(NULL != (entry = readdir(dir)))
    {
        char* newPath = malloc(strlen(fpath) + 2 + strlen(entry->d_name));
        osAssert(NULL != newPath, "making new path failed");

        if(!strcmp("..", fpath) || !strcmp(".", fpath))
        {
            free(newPath);
            return;
        }

        strcpy(newPath, fpath);
        strcat(newPath, "/");
        strcat(newPath, entry->d_name);

        osMoveFile(newPath, dirPath);
        free(newPath);
    }
    osAssert(-1 != closedir(dir), "closedir failed");

}
