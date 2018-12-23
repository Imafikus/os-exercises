#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define osAssert(cond, msg) osErrorFatal(cond, msg, __FILE__, __LINE__);

void osErrorFatal(bool cond, char* msg, char* file, int line)
{
    if(!cond)
    {
        perror(msg);
        fprintf(stderr, "%s %d\n", file, line);
        exit(EXIT_FAILURE);
    }
}

pid_t generateProcess(char** argv)
{
    char** realArgv = argv + 1;
    char** copyArgv = realArgv;
    while(*copyArgv != NULL)
    {
        printf("argv: %s\n", *copyArgv);
        ++copyArgv;
    }

    pid_t child = fork();
    osAssert(-1 != child, "fork failed");
    if(0 == child)
    {   
        // ovo ce sad da postane program sa komandom realArgv[0] i argumentima koji su u realArgv nizu
        osAssert(execvp(realArgv[0], realArgv), "execvp failed");
    }
    return child;
}

int main(int argc, char** argv)
{

    osAssert(argc >= 2, "wrong usage");

    pid_t processHandle = generateProcess(argv);

    int status;
    osAssert(-1 != waitpid(processHandle, &status, 0), "Waiting for child failed");

    if(WIFEXITED(status) )
        printf("Program %s was exited with code %d", argv[1], WEXITSTATUS(status));
    
    if(WIFSIGNALED(status) )
        printf("Program %s was killed with signal number %d (%s)", argv[1], WTERMSIG(status), strsignal(WTERMSIG(status)));
    
}