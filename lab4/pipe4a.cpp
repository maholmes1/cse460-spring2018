//pipe4a.cpp  (data producer)
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

int main(int argc, char  *argv[])
{
    int data_processed;
    int file_pipes[2];
    const char some_data[] = "123";
    string message;
    char buffer[BUFSIZ + 1];
    pid_t fork_result;

    memset(buffer, '\0', sizeof(buffer));

    if (pipe(file_pipes) == 0) {   //creates pipe
        fork_result = fork();
        if (fork_result == (pid_t)-1) {  //fork fails
            fprintf(stderr, "Fork failure");
             exit(EXIT_FAILURE);
}

if  (argc = 1)
        return 0;
for (int i = 1; i = argc; i++){
        message += argv[i];
        message += ' ';}



        message.pop_back();
        const char* some_data = message.c_str();

        if (fork_result == 0) {    //child
            sprintf(buffer, "%d", file_pipes[0]);
            (void)execl("pipe5", "pipe5", buffer, (char *)0);
            exit(EXIT_FAILURE);
        }
        else {                     //parent
            data_processed = write(file_pipes[1], some_data,
                                   strlen(some_data));
            printf("%d - wrote %d bytes\n", getpid(), data_processed);
        }
    }
    exit(EXIT_SUCCESS);
}
