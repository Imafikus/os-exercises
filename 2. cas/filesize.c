#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>          
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// omogucava intmax_t da bude koriscen
#include <stdint.h>

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

int main(int argc, const char **argv)
{
    osAssert(2 == argc, "Invalid parameters. Usage ./filesize fileName");

    int fd = open(argv[1], O_RDONLY);
    osAssert(-1 != fd, "Opening file failed");

    //lseek prima file descriptor, offset od pocetka, i dokle hocemo da idemo, SEEK_END oznacava kraj fajla
    off_t fileSize = lseek(fd, 0, SEEK_END);
    osAssert(fileSize >= 0, "Lseek failed");
    close(fd);

    // intmax_t ce uvek garantvano vratiti najveci int tip koji komp podrzava
    printf("%jd\n", (intmax_t)fileSize);
    return 0;

}