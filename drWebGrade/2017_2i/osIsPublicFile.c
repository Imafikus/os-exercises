#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>           
#include <sys/stat.h>
#include <limits.h>

#define MAX_USR_LEN (64)
#define MAX_GRP_LEN (32)
#define MAX_OTH_LEN (32)


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

bool osIsPublicFile(const char *fpath);

int main(int argc, char const *argv[])
{
    osAssert(argc == 2, "Usage, ./program.out target_file");
    bool fileStat = osIsPublicFile(argv[1]);
    if(fileStat)
        printf("true\n");
    else
        printf("false\n");

}
bool osIsPublicFile(const char *fpath)
{
    //PATH_MAX je najduzi moguci path, i mi dodefinisemo ostale maxeve (na npr 64)
    char *finfoStr = malloc(PATH_MAX + MAX_USR_LEN + MAX_GRP_LEN + MAX_OTH_LEN);
    osAssert(NULL != finfoStr, "finfoStr malloc failed");
    
    //man 2 stat, struktura stat koja drzi podatke o fajlu
    struct stat finfo;
    osAssert(-1 != stat(fpath, &finfo), "Stat failed.");
    
    // man 7 inode drzi provere za fajlove (npr. S_ISREG za regularan fajl)
    osAssert(S_ISREG(finfo.st_mode), "File irregular");

    //S_IROTH i S_IWOTH su flegovi iz man 2 open, vracaju info o permisijama
    // ako bitovska konjukcija daje 0, znaci da nije postavljen odredjeni fleg
    if (!((S_IROTH & finfo.st_mode) && (S_IWOTH & finfo.st_mode)))
        return false;
    else
        return true;
    
}

