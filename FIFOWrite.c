#include <stdio.h>
#include <stdlib.h>

#define LOOPS 10
//-----------------------------------------------------------------------------
int main(int argc,char *argv[]) {

    FILE *FIFOWrite;
    int Index;

    if ((FIFOWrite = fopen(argv[1],"w")) == NULL) {
        perror("Opening FIFO to write");
        exit(EXIT_FAILURE);
    }

    for (Index = 0;Index < LOOPS;Index++) {
        fprintf(FIFOWrite,"Message %d into FIFO\n",Index);
        fflush(FIFOWrite);
        sleep(1);
    }

    fclose(FIFOWrite);
    return(EXIT_SUCCESS);
}
