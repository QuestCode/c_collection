/*---------------------------------------------------------------------------*/
/*----Message sender */
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "message.h"
/*---------------------------------------------------------------------------*/
int main() {

    int Message = 0;
    int MessageNumber,QueueID;
    REQUEST_BUFFER Request;
    SUPPLY_BUFFER Supply;

/*----Assume that no Q with this key exists */
    if ((QueueID = msgget(QUEUE_KEY,0666 | IPC_CREAT | IPC_EXCL) ) == -1) {
        perror("SENDER: Error creating message queue\n");
        exit(-1);
    }
    printf("SENDER: New queue with ID %d opened\n",QueueID);

/*----Receive message requests and reply with time data */
    for (MessageNumber=0; MessageNumber < SEND_MESSAGES; MessageNumber++) {
        printf("SENDER: Waiting for request %d on queue %d\n",MessageNumber,
QueueID);
        if (msgrcv(QueueID,&Request,sizeof(Request)-sizeof(Request.MessageType),
REQUEST_TYPE,0) == -1) {
            perror("SENDER: Error receiving request\n");
            exit(-1);
        }
        printf("SENDER: Request %d for type %ld received on queue %d\n",
MessageNumber,Request.SendType,QueueID);

/*----Generate data to send back */
        Supply.Data = ++Message;
        Supply.MessageType = Request.SendType;
        if (msgsnd(QueueID,&Supply,sizeof(Supply) - sizeof(Supply.MessageType),
0)) {
            perror("SENDER: Error sending message\n");
            exit(-1);
        }
        printf("SENDER: Message %d type %ld with data %d sent on queue %d\n",
MessageNumber,Request.SendType,Supply.Data,QueueID);
    }

/*----Delete the queue */
    msgctl(QueueID,IPC_RMID,NULL);
    printf("SENDER: Queue %d closed\n",QueueID);

    return(0);
}
