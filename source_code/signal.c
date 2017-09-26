#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

const int MAX_INTERRUPTS = 10;

int NumberOfInterrupts;
//-----------------------------------------------------------------------------
void InterruptCatcher(int TheSignal) {

//----For old UNIX signal(SIGINT,InterruptCatcher);
    NumberOfInterrupts++;
    printf("You just pressed ^C for the %dth time\n",NumberOfInterrupts);
}
//-----------------------------------------------------------------------------
int main(void) {

    void (*OldResponse)(int);

    if ((OldResponse = signal(SIGINT,InterruptCatcher)) == SIG_ERR) {
        perror("Could not set signal handler");
        return(EXIT_FAILURE);
    }

    NumberOfInterrupts = 0;
    while (NumberOfInterrupts < MAX_INTERRUPTS) {
        printf("Waiting for ^C\n");
        pause();
        printf("After the pause\n\n");
    }
    if (signal(SIGINT,OldResponse) == SIG_ERR) {
        perror("Could not reset signal handler");
    }

    while (1) {
        printf("Now things are back to normal\n");
        sleep(1);
    }

    return(EXIT_SUCCESS);
}
