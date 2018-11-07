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
#include <limits.h>
#include <ftw.h>

#define SECONDS_IN_DAY (60*60*24)

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
void osMoveFile(const char* srcPath, const char* destPath);
unsigned osNumDaysFileModified(const char* fpath);
void osMkPublicDir(const char* fpath);
bool isPublicFile(const char* fpath);
int fn(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf);

char* dirPath;

int processFile(const char* fpath, const struct stat *sb, int typeflag, struct FTW* ftwbuf) {
  
  
  if (typeflag == FTW_F) {

    if (isPublicFile(fpath)) {

      unsigned days = osNumDaysFileModified(fpath);
      
      if (days < 30) {
	
	char* newPath = malloc(strlen(dirPath) + 2 + strlen(fpath + ftwbuf->base));
	strcpy(newPath, dirPath);
	strcat(newPath,"/");
	strcat(newPath, fpath + ftwbuf->base);
	
	osMoveFile(fpath, newPath);
	
	free(newPath);
      }
      else {
	
	osAssert(unlink(fpath) != -1, "unlink failed");
      }
    }
  }
  
  return 0;
}


int main(int argc, const char** argv)
{
    osAssert(3 == argc, "wrong num of params");
    dirPath = argv[2];
    osMkPublicDir(dirPath);
    osAssert(nftw(argv[1], processFile, 50, 0) != -1, "nftw failed");

}
int fn(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if (FTW_F == typeflag)
    {
        if(isPublicFile (fpath))
        {
            if (osNumDaysFileModified(fpath) <= 30)
            {
                char* newPath = malloc(strlen(dirPath) + 2 + strlen(fpath + ftwbuf->base));
                strcat(newPath, "/");
                strcat(newPath, fpath + ftwbuf->base);
                osMoveFile(fpath, newPath);
                free(newPath);
            }
            else
                osAssert(unlink(fpath), "delete failed");
        }
    }
    return 0;
}

unsigned osNumDaysFileModified(const char* fpath)
{
    struct stat finfo;
    osAssert(stat(fpath, &finfo) != -1, "stat failed");

    time_t cur_time = time(NULL);
    osAssert(-1 != cur_time, "ctime failed");

    return ((cur_time - finfo.st_mtime) / SECONDS_IN_DAY);

}
bool isPublicFile(const char* fpath)
{
    struct stat finfo;
    osAssert(stat(fpath, &finfo) >= 0, "stat failed");

    osAssert(S_ISREG(finfo.st_mode), "File not regular");
    
    if((finfo.st_mode & S_IROTH) && (finfo.st_mode & S_IWOTH))
        return true;
    else
        return false;
}
void osMkPublicDir(const char* fpath)
{
    mode_t oldMask = umask(0);
    osAssert(mkdir(fpath, S_IRWXU | S_IRWXG | S_IRWXO) != -1, "mkdir failed");
    umask(oldMask);
    return;
}
void osMoveFile(const char* srcPath, const char* destPath)
{
    char* absSrc = realpath(srcPath, NULL);
    osAssert(absSrc != NULL, "file doesnt exist");

    char* absDest = realpath(destPath, NULL);
    if (absDest != NULL)
        osAssert(strcmp(absSrc, absDest) != 0, "same file");
    
    free(absDest);
    free(absSrc);
    osAssert(rename(srcPath, destPath) != -1, "rename failed");
}