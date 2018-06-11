#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MAX_TEXT 512

struct my_msg_st{
    long int my_msg_type;
    char some_text[BUFSIZ];
};

int main()
{
    int running = 1;
    int msgid, msgid2;
    struct my_msg_st some_data;
    long int msg_to_receive = 0;
    char buffer[BUFSIZ];

    //first set up message queue
    msgid = msgget((key_t)1234,  0666 | IPC_CREAT);
    msgid2 = msgget((key_t)1234, 0666 | IPC_CREAT);

    if (msgid == -1){
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    //then messages are retrieved from the queue, until an end message is encountered. 
    //lastly, the message queue is deleted.

    while (running){
        if (msgrcv(msgid, (void *)&some_data, BUFSIZ,
                   msg_to_receive, 0) == -1){
            fprintf(stderr, "msgrcv failed with error: %d\n", errno);
            exit(EXIT_FAILURE);
        }
        printf("You wrote: %s", some_data.some_text);
        if (strncmp(some_data.some_text, "end", 3) == 0){
            running = 0;
        }

        else{

            printf("Enter text: ");
            fgets(buffer, BUFSIZ, stdin);
            some_data.my_msg_type = 1;
            strcpy(some_data.some_text, buffer);
   if (msgsnd(msgid2, (void*)&some_data, MAX_TEXT, 0) == -1)
                exit(EXIT_FAILURE);
            if (strncmp(buffer, "end", 3) == 0)
                running = 0;
        }
    }

    if (msgctl(msgid, IPC_RMID, 0) == -1){
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);

}
