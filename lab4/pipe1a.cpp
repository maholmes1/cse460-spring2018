//pipe1a.cpp
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  FILE *fpi;                            //for reading a pipe
  char command[15];
  strcat(command,argv[1]);
  strcat(command," ");
  strcat(command,argv[2]);
  cout << "Command:  " << endl;


  char buffer[BUFSIZ+1];                //BUFSIZ defined in <stdio.h>

  int chars_read;
  memset ( buffer, 0,sizeof(buffer));   //clear buffer
  fpi = popen ( "ps -auxw", "r" );      //pipe to command "ps -auxw"
  if ( fpi != NULL ) {
    //read data from pipe into buffer
    chars_read = fread(buffer, sizeof(char), BUFSIZ, fpi );
    if ( chars_read > 0 )
     cout << "Output from pipe: " << buffer << endl;
    pclose ( fpi );                     //close the pipe
    return 0;
  }

  return 1;
}
