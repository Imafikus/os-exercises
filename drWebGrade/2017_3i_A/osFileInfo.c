#define _XOPEN_SOURCE_ 700

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <fcntl.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal(bool cond, const char *msg, const char *file, int line)
{
    if(!cond)
    {
        perror(msg);
        fprintf(stderr, "%s:%d\n", file, line);
        exit(EXIT_FAILURE);
    }
}

void writeInfo(const char *fpath);

int main(int argc, const char **argv)
{
    osAssert(2 == argc, "Incorrect parameters");
    writeInfo(argv[1]);
    return 0;
}

void writeInfo(const char *fpath)
{
    struct stat finfo;
    osAssert(stat(fpath, &finfo) >= 0, "stat failed");

    char a[] = "----------";
    if (!(S_ISREG(finfo.st_mode)))
        a[0] = 'd';

    if (finfo.st_mode & S_IRUSR)
        a[1] = 'r';
    if (finfo.st_mode & S_IWUSR)
        a[2] = 'w';
    if (finfo.st_mode & S_IXUSR)
        a[3] = 'x';
    
    if (finfo.st_mode & S_IRGRP)
        a[4] = 'r';
    if (finfo.st_mode & S_IWGRP)
        a[5] = 'w';
    if (finfo.st_mode & S_IXGRP)
        a[6] = 'x';
    
    if (finfo.st_mode & S_IROTH)
        a[7] = 'r';
    if (finfo.st_mode & S_IWOTH)
        a[8] = 'w';
    if (finfo.st_mode & S_IXOTH)
        a[9] = 'x';

    printf("%s\n", a);

}


