#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <assert.h>

#define TOTAL_TIME 20
#define MAX_THREADS 50
#define BONUS_FACTOR 1.2
//-----------------------------------------------------------------------------
void *IndependentLoop(void *Length) {

    int Delay,TotalDelay,TargetIndex,Bullet,Bullets;
    extern pthread_t NewThreads[];
    extern int Delays[];

    if ((Delay = atoi((char *)Length)) == 0) {
        fprintf(stderr,"ERROR: %s is not an integer\n",(char *)Length);
        return(NULL);
    }
    printf("Thread %3d starts\n",Delay);
    srand(Delay);

    TotalDelay = 0;
    while (TotalDelay < TOTAL_TIME) {
        sleep(Delay);
        TotalDelay += Delay;
        Bullets = ceil(pow(Delay,BONUS_FACTOR));
        printf("Thread %3d is up to %3d, gets %3d bullets\n",Delay,TotalDelay,
Bullets);
//----Fire at other threads
        for (Bullet = 0; Bullet < Bullets; Bullet++) {
            TargetIndex = rand() % MAX_THREADS;
            printf("Thread %3d chooses %3d ... ",Delay,Delays[TargetIndex]);
            if (Delays[TargetIndex] > 0) {
                if (pthread_equal(NewThreads[TargetIndex],pthread_self())) {
                    printf("and shoots into sky to avoid suicide\n");
//----Shoot at chosen thread
                } else if (pthread_cancel(NewThreads[TargetIndex]) == 0) {
                    printf("and kills %3d\n",Delays[TargetIndex]);
//----Negate delay for that thread, so we know it was shot
                    Delays[TargetIndex] = -Delays[TargetIndex];
                }
            } else if (Delays[TargetIndex] == 0) {
                printf("and wastes bullet shooting into thin air\n");
            } else if (Delays[TargetIndex] < 0) {
                printf("and wastes a bullet on a dead thread\n");
//----Don't shoot at self
            }
        }
    }

    printf("LIFE! Thread %3d is completed\n",Delay);
    return(NULL);
}
//-----------------------------------------------------------------------------
int main(int argc,char *argv[]) {

    int Index;
    extern pthread_t NewThreads[];
    extern int Delays[];
    int Position;

    for (Index = 0;Index < MAX_THREADS;Index++) {
        Delays[Index] = 0;
    }
    srand(time(NULL));

    for (Index = 1; Index < argc; Index++) {
//----Choose random position to store thread information
        do {
            Position = rand() % MAX_THREADS;
        } while (Delays[Position] != 0);

//----Store thread information
        Delays[Position] = atoi(argv[Index]);
        if (pthread_create(&NewThreads[Position],NULL,IndependentLoop,
argv[Index]) != 0) {
           perror("Creating thread");
           exit(EXIT_FAILURE);
        }
        printf("Started thread %3s at index %3d\n",argv[Index],Position);

//-----Detatch thread
        if (pthread_detach(NewThreads[Position]) != 0) {
           perror("Detaching thread");
           exit(EXIT_FAILURE);
        }
    }

    printf("Exiting the main program, leaving the threads running\n");
    pthread_exit(NULL);

}
