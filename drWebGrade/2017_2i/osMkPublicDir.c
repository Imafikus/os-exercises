#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
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

void osMkPublicDir(const char *dname);

int main(int argc, const char **argv)
{
    osAssert(argc == 2, "Incorrect input");
    osMkPublicDir(argv[1]);
    return 0;
}

void osMkPublicDir(const char *dname)
{
    
    //umask se postavlja na negaciju ovoga u zagradi, a povratna vrednost je stari umask
    //povratna vrendost umask() je tipa mode_t
    mode_t oldUmask = umask(0);
    static mode_t perm = 0777;
    osAssert((mkdir(dname, perm) >= 0), "mkdir Failed");
    umask(oldUmask)

}
