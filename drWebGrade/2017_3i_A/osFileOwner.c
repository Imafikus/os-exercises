#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h> 
#include <grp.h> 
#include <pwd.h>

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

void osFileOwner(const char *fpath);

int main(int argc, const char **argv)
{
    osAssert(2 == argc, "Incorrect parameters provided.");
    osFileOwner(argv[1]);
}

void osFileOwner(const char *fpath)
{
    struct stat finfo;
    osAssert(stat(fpath, &finfo) >= 0, "stat failed ");

    gid_t gid = finfo.st_gid;
    uid_t uid = finfo.st_uid;

    // man 3 getgrid
    struct group *user_gid = getgrgid(gid);
    osAssert(NULL != user_gid, "getgrid failed");
    
    //man 3 getpwuid
    struct passwd *user_name = getpwuid(uid);
    osAssert(NULL != user_name, "getpwuid failed");

    printf("%s %s",user_name->pw_name ,user_gid->gr_name);
}