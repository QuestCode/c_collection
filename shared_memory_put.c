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

    int SegmentID,*SharedIntPtr;

/*----Create a shared segment */
    if ((SegmentID = shmget(SHARED_MEMORY_KEY,sizeof(int),
IPC_CREAT|S_IRUSR|S_IWUSR)) == -1 ||
/*----Attach to the shared segment */
(SharedIntPtr = (int *)shmat(SegmentID,NULL,0)) == NULL) {
        perror("SHAREPUT: Error getting shared segment");
        exit(-1);
    }

/*----Change the value in the shared segement every second */
    for (*SharedIntPtr=0;*SharedIntPtr<=LAST_NUMBER;(*SharedIntPtr)++) {
        sleep(1);
    }
/*----Detach from the shared segment */
    if (shmdt(SharedIntPtr) == -1) {
        perror("SHAREPUT: Error detaching shared segment");
    }

    return(0);
}
