#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <fcntl.h> 

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__)

void osErrorFatal (bool cond, const char *msg, const char *file, int line)
{
    /*
    Proverava da li je tacan cond, ako jeste, baca gresku u stderr
    Greska sadrzi msg na koji perror doda string koda greske koji se desio
    file i line su konstante i one sluze da se da ime fajla i red na kome se desila greska
    */
    if(!cond)
    {
        perror(msg);
        fprintf(stderr, "%s%d\n", file, line);
        exit(EXIT_FAILURE);
    }
}
int main(int argc, char const **argv)
{
    // ! Note: '' i "" nije isto, drugo se ponasa kao string(ima \0) a prvo kao char
    osAssert(3 == argc, "Argument missing. Usage ./rmfile -[fd] path/to/file/or/dir");

    osAssert('-' == argv[1][0], "Not a command line option.");
    if('f' == argv[1][1])
        osAssert(unlink(argv[2]) >= 0, "Removing file failed");
    
    if('d' == argv[1][1])
        osAssert(rmdir(argv[2]) >=0, "Removing folder failed");
    
    else
        osAssert(false, "Wrong option, valid options are 'f' and 'd'");

    return 0;
}
