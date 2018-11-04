#define _XOPEN_SOURCE 700

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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

void changePrivileges(const char* fpath);

int main (int argc, const char** argv)
{
    osAssert(2 == argc, "wrong input");
    changePrivileges(argv[1]);

}

void changePrivileges(const char* fpath)
{
    struct stat finfo;
    osAssert(stat(fpath, &finfo) >=0, "stat failed");

    int owner_perm = 00;
    int group_perm = 00;
    int other_perm = 00;
    
    //owner
    if (finfo.st_mode & S_IRUSR)
        owner_perm += 04;
    
    if (finfo.st_mode & S_IWUSR)
        owner_perm += 02;
    
    if (finfo.st_mode & S_IXUSR)
        owner_perm += 01;

    //group
    if (finfo.st_mode & S_IRGRP)
        group_perm += 04;
    
    if (finfo.st_mode & S_IWGRP)
        group_perm += 02;
    
    if (finfo.st_mode & S_IXGRP)
        group_perm += 01;
    
    //other
    if (finfo.st_mode & S_IROTH)
        other_perm += 04;
    
    if (finfo.st_mode & S_IWOTH)
        other_perm += 02;
    
    if (finfo.st_mode & S_IXOTH)
        other_perm += 01;

    mode_t new_mode = (owner_perm*0100 + other_perm*010 + group_perm);

    osAssert(chmod(fpath, new_mode) >= 0, "chmod failed");
}
