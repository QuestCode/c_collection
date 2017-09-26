#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/tty.h>
#include <sys/signal.h>
#include <unistd.h>

#define STDIN 0
#define STDOUT 1

/*----External variable have to be used as the signal */
/*----handlers cannot be passed parameters */
int FileDescriptors[2];
int SlaveCount;
/*---------------------------------------------------------------------------*/
/*----Service a SIGTERM by reading the pipe */
void Service(int TheSignal) {

int NumberRead;
char Buffer[TTYHOG];

/*----Reset the signal */
signal(SIGTERM,Service);
/*----read from the pipe into the Buffer and echo */
NumberRead = read(FileDescriptors[0],Buffer,TTYHOG);
write(STDOUT,Buffer,NumberRead);
}
/*---------------------------------------------------------------------------*/
/*----Service SIGCHLD, meaning a child has died */
void SlaveDrop(int TheSignal) {

/*----Reset signal */
signal(SIGCHLD,SlaveDrop);
--SlaveCount;
}
/*---------------------------------------------------------------------------*/
/*----Starts a slave process to transfer tty input to the pipe */
void StartSlave(char *TTY,int Pipe,char *PIDString) {

/*----Fork to produce a new slave terminal reader */
if (fork() == 0) {
/*----If the child then set up as a reader */
/*----Set STDIN to the tty */
    close(STDIN);
    if (STDIN != open(TTY,O_RDONLY)) {
        fprintf(stderr,"Bad tty %s\n",TTY);
        exit(-1);
        }
/*----Set STDOUT to pipe */
    close(STDOUT);
    if (dup(Pipe) != STDOUT) {
        fprintf(stderr,"Error on stdout\n");
        exit(-2);
        }
    close(FileDescriptors[0]);
    close(FileDescriptors[1]);
/*----exec to the slave */
    execl("./PipeSlave","slave",PIDString,NULL);
    fprintf(stderr,"Can't exec slave for %s\n",TTY);
    exit(-3);
    }
else printf("Forked slave for %s\n",TTY);
}
/*---------------------------------------------------------------------------*/
int main(int argc,char *argv[])
/*----Usage master <device> <device> ... */
{
int i1;
char PIDString[10];

/*----Set interrupt handlers */
signal(SIGTERM,Service);
signal(SIGCHLD,SlaveDrop);

/*----Set string for this process's ID to be used by */
/*----slave to signal the main */
sprintf(PIDString,"%d",getpid());

/*----Set up pipe to recieve data through */
pipe(FileDescriptors);

/*----Number of slaves is given by number of argv */
SlaveCount = argc-1;
/*----Start each slave */
for (i1=1;i1<=SlaveCount;i1++)
    StartSlave(argv[i1],FileDescriptors[1],PIDString);

/*----Close the output channel -it's used by the slaves */
close(FileDescriptors[1]);

/*----Do nothing, the handlers will do all the work */
while (SlaveCount > 0)
    pause();

return(0);
}
