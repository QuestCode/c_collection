#include <stdio.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>

#define SEMAPHORE_KEY 1000
#define NUMBER_OF_SEMAPHORES 1
#define NUMBER_OF_PROCESSES 2

#define STRING_LENGTH 80
typedef char string[STRING_LENGTH];
/*---------------------------------------------------------------------------*/
void ChangeSemaphore(int SemaphoreSetID,int Change) {

    struct sembuf SemaphoreOperations[1];

    SemaphoreOperations[0].sem_num = 0;
    SemaphoreOperations[0].sem_op = Change;
    SemaphoreOperations[0].sem_flg = SEM_UNDO;
    if (semop(SemaphoreSetID,SemaphoreOperations,1) == -1) {
        perror("Semaphore opertaion");
        exit(-1);
    }
}
/*---------------------------------------------------------------------------*/
int GetString(int SemaphoreSetID,string String) {

/*----Claim right to talk to user */
    ChangeSemaphore(SemaphoreSetID,-1);
/*----Get a string */
    printf("Please enter string for process %d:",getpid());
    if (fgets(String,STRING_LENGTH-1,stdin) != NULL) {
/*----release right */
        ChangeSemaphore(SemaphoreSetID,1);
        return(1);
    } else {
        ChangeSemaphore(SemaphoreSetID,1);
        return(0);
    }
}
/*---------------------------------------------------------------------------*/
void PutString(int SemaphoreSetID,string String) {

/*----Claim right to output */
    ChangeSemaphore(SemaphoreSetID,-1);
/*----Do output */
    printf("The string entered for process %d was %s\n",getpid(),String);
/*----release right */
    ChangeSemaphore(SemaphoreSetID,1);
}
/*---------------------------------------------------------------------------*/
int main(void) {

    int SemaphoreSetID,ProcessIndex;
    string String;

/*----Create or attach to a semaphore */
    if ((SemaphoreSetID = semget((key_t) SEMAPHORE_KEY, NUMBER_OF_SEMAPHORES,
IPC_CREAT|S_IRUSR|S_IWUSR)) == -1) {
        perror("SEMAPHORE: Error getting semaphore set");
        exit(-1);
    } else {
        if (semctl(SemaphoreSetID,0,SETVAL,1) == -1)
             perror("SEMAPHORE: Could not set semaphore value");

        for (ProcessIndex=0;ProcessIndex<NUMBER_OF_PROCESSES;ProcessIndex++)
            if (fork() == 0)
                break;

        while (GetString(SemaphoreSetID,String)) {
            PutString(SemaphoreSetID,String);
            sleep(strlen(String) % 3);
        }

/*----Delete the semaphore ignoring errors when everyone tries to delete it */
        semctl(SemaphoreSetID,NUMBER_OF_SEMAPHORES,IPC_RMID,0);
    }

    return(0);
}
