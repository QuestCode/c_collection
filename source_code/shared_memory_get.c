#include <stdio.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define SHARED_MEMORY_KEY 1000
#define LAST_NUMBER 16
/*---------------------------------------------------------------------------*/
int main(void) {

    int SegmentID,*SharedIntPtr,PreviousSharedInt;
    struct shmid_ds StatusBuffer;

/*----Find a shared segment with ID */
    if ((SegmentID = shmget(SHARED_MEMORY_KEY,sizeof(int),0)) == -1 ||
/*----Attach to the segment */
(SharedIntPtr = (int *)shmat(SegmentID,NULL,0)) == NULL) {
        perror("SHAREGET: Error getting shared segment");
        exit(-1);
    }

    PreviousSharedInt = -1;

/*----Read the integers until it becomes the last one */
    while (*SharedIntPtr != LAST_NUMBER) {
        if (*SharedIntPtr != PreviousSharedInt) {
            PreviousSharedInt = *SharedIntPtr;
            printf("Shared integer is now %d\n",PreviousSharedInt);
        }
    }

/*----Detach from the shared segment */
    if (shmdt(SharedIntPtr) == -1) {
        perror("SHAREGET: Error detaching segment");
        exit(-1);
    }

/*----Remove the shared segement */
    if (shmctl(SegmentID,IPC_RMID,&StatusBuffer) == -1) {
        perror("SHAREGET: Error removing segment");
        exit(-1);
    }

    return(0);
}
