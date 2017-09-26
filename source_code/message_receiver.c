/*---------------------------------------------------------------------------*/
/*----Message receiver */
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "message.h"
/*---------------------------------------------------------------------------*/
/*----Multiple copies of this are run */
int main(void) {

    int MessageNumber,QueueID;
    REQUEST_BUFFER Request;
    SUPPLY_BUFFER Supply;
    long SupplyKey,ProcessMessageType;
    int PID;

    do {
        PID = getpid();
/*----Make a unique message type */
        ProcessMessageType = (long) PID;
        printf("RECEIVER %d: Receiving messages of type %ld\n",PID,
ProcessMessageType);
/*----Make the message queue */
        if ((QueueID = msgget(QUEUE_KEY,0)) == -1) {
            perror("RECEIVER : Error attaching to message queue");
            exit(-1);
        }
        printf("RECEIVER %d: Receiving on queue %d\n",PID,QueueID);

/*----Receive messages */
        for (MessageNumber=0;MessageNumber < RECEIVE_MESSAGES;MessageNumber++) {
            Request.MessageType = REQUEST_TYPE;
            Request.SendType = ProcessMessageType;
/*----Send a request for a message */
            printf("RECEIVER %d: Request %d on queue %d for type %ld going out\n",
PID,MessageNumber,QueueID,ProcessMessageType);
/*----Break out if the producer has removed the Q */
            if (msgsnd(QueueID,&Request,sizeof(Request) -
sizeof(Request.MessageType),0) == -1) {
                printf("RECEIVER %d: Message queue is closed\n");
                exit(0);
            }
            printf("RECEIVER %d: Request %d on queue %d for type %ld gone\n",
PID,MessageNumber,QueueID,ProcessMessageType);
/*----Receive a reply message */
            if (msgrcv(QueueID,&Supply,sizeof(Supply) -
sizeof(Supply.MessageType),ProcessMessageType,0) == -1) {
                printf("RECEIVER %d: Message queue is closed\n");
                exit(0);
            }
            printf("RECEIVER %d: Received message %d of type %ld with data %d\n",
PID,MessageNumber,ProcessMessageType,Supply.Data);
        }
/*----Make another receiver */
        PID = fork();
/*----New process collects more messages */
    } while (PID == 0);

    return(0);
}
