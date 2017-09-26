#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>

#define TOTAL_IO_TIME 10
#define STRING_LENGTH 80
typedef char cstring[STRING_LENGTH];
//-----------------------------------------------------------------------------
void CreateChild(int Pipe[2],int Delay) {

    cstring Message;
    int IOTime;

    if (pipe(Pipe) == -1) {
        perror("Creating pipe");
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) {
        sprintf(Message,"Message from %d to %d, delay %d",Pipe[1],Pipe[0],
Delay);
        close(Pipe[0]);
        IOTime = 0;
//----Child send messages for a while, at delayed intervals
        while (IOTime < TOTAL_IO_TIME) {
            write(Pipe[1],Message,strlen(Message)+1);
            sleep(Delay);
            IOTime += Delay;
        }
        close(Pipe[1]);
        exit(EXIT_SUCCESS);
    } else {
        close(Pipe[1]);
    }
}
