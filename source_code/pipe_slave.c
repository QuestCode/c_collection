#include <stdio.h>
#include <signal.h>
#include <sys/types.h> /*----For types in tty.h */
#include <sys/tty.h> /*----For TTYHOG - max line from tty */
#include <sys/signal.h>
#include <unistd.h>
#include <stdlib.h>

#define STDIN 0
#define STDOUT 1
/*---------------------------------------------------------------------------*/
int main(int argc,char *argv[])
/*----Slave reads from a terminal and sends to the master program. The */
/*----terminal is set as STDIN and the pipe to the main program as STDOUT */
{
int NumberRead,MasterPID;
char Buffer[TTYHOG];

/*----Get the process id of the master */
MasterPID = atoi(argv[1]);
fprintf(stderr,"Ready for input\n");
while (1) {
    NumberRead = read(STDIN,Buffer,TTYHOG);
    if (NumberRead == 0) {
/*----Terminate cleanly on EOF */
        close(STDOUT);
        exit(0);
        }
    kill(MasterPID,SIGTERM);
    write(STDOUT,Buffer,NumberRead);
    }
}
