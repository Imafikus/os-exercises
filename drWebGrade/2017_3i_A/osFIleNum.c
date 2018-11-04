#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ftw.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

unsigned f_reg = 0;
unsigned f_dir = 0;
unsigned f_char = 0;
unsigned f_blk = 0;
unsigned f_link = 0;
unsigned f_fifo = 0;
unsigned f_sock = 0;

unsigned f_all = 0;

void osErrorFatal(bool cond, const char* msg, const char* file, int line)
{
    if(!cond)
    {
        perror(msg);
        fprintf(stderr, "%s:%d\n", file, line);
        exit(EXIT_FAILURE);
    }
}

int walkFn(const char* fpath, const struct stat *sb, int typeflag, struct FTW* ftwbuf)
{
    
    if(S_ISREG(sb->st_mode))
        f_reg++;
    if(S_ISDIR(sb->st_mode))
        f_dir++;
    if(S_ISCHR(sb->st_mode))
        f_char++;
    if(S_ISBLK(sb->st_mode))
        f_blk++;
    if(S_ISFIFO(sb->st_mode))
        f_fifo++;
    if(S_ISLNK(sb->st_mode))
        f_link++;
    if(S_ISSOCK(sb->st_mode))
        f_sock++;

    return 0;
}

int main(int argc, const char* argv[])
{
    osAssert(2 == argc, "wrong num of parameters");
    osAssert(-1 != nftw(argv[1], walkFn, 50, FTW_PHYS), "nftw failed");

    f_all = f_blk + f_char + f_dir + f_fifo + f_link + f_reg + f_sock;
    printf("%d %d %d %d %d %d %d %d\n", f_reg, f_dir, f_char, f_blk, f_link, f_fifo, f_sock, f_all);

    return 0;
}