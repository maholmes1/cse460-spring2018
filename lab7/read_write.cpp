// read_write.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

SDL_mutex *mutex;
SDL_cond *reader_queue;
SDL_cond *writer_queue;
int readers = 0;
int writers = 0;
int reader(void* data) {
 while(true) { // Read indefinitely
 // Random delay until read
 SDL_Delay(rand() % 3000);

 SDL_LockMutex(mutex);

 // Wait until there is a writer
 while(writers != 0)
 SDL_CondWait(reader_queue, mutex);

 // Include this thread as current reader
 ++readers;

 SDL_UnlockMutex ( mutex );

 // Open file
 ifstream file("counter.txt");

 if (file.good()) {
 // Get the current counter value
 int counter;
 file >> counter;

 // Print the counter value
 cout << *((string*)data) << " with counter value " << counter << endl;

 // Close file
 file.close();
 } else // Print error
 cout << *((string*)data) << " unable to read counter" << endl;

 SDL_LockMutex(mutex);

 // Exclude this thread from current readers and signal waiting writers if appropriate
 if (--readers == 0 )
 SDL_CondSignal (writer_queue);
 SDL_UnlockMutex(mutex);
 }
}
int writer (void* data) {
 while(true) { // write indefinitely
 // Random delay until write
 SDL_Delay(rand() % 3000);

 SDL_LockMutex(mutex);

 // Wait for readers and writers
 while(readers != 0 && writers != 0)
 SDL_CondWait(writer_queue, mutex);

 // Set writer as active
 ++writers;

 SDL_UnlockMutex(mutex);

 // Write
 int counter = -1;

 // Get the current counter value
 ifstream file_read("counter.txt");
 if (file_read.good()) {
 file_read >> counter;
 file_read.close();
 }

 // Increase counter and write it
 ++counter;
 ofstream file_write("counter.txt", std::ofstream::trunc);
 file_write << counter;
 file_write.close();

 // Print counter value
 cout << *((string*)data) << " with counter value " << counter << endl;

 SDL_LockMutex (mutex);

 // Deactivate writer
 --writers;

 // Wake up next writers
 SDL_CondSignal(writer_queue);
 // Wake up all readers
 SDL_CondSignal(reader_queue);

 SDL_UnlockMutex (mutex);
 }
}
int main () {
 SDL_Thread* reader_thread[20];
 SDL_Thread* writer_thread[3];
 mutex = SDL_CreateMutex();
 reader_queue = SDL_CreateCond();
 writer_queue = SDL_CreateCond();
 // Create writers
 for(int i = 0; i < 3; ++i) {
 stringstream ss;
 ss << "writer " << i;
 string* name = new string(ss.str());
 writer_thread[i] = SDL_CreateThread(writer, (void*)name);
 }

 // Create readers
 for(int i = 0; i < 20; ++i) {
 stringstream ss;
 ss << "reader " << i;
 string* name = new string(ss.str());
 reader_thread[i] = SDL_CreateThread (reader, (void*)name);
 }

 // Wait indefinitely for a thread
 SDL_WaitThread(writer_thread[0], NULL);
 SDL_DestroyCond(reader_queue);
 SDL_DestroyCond(writer_queue);
 SDL_DestroyMutex(mutex);

 return 0;
}
                                                                                                                                                                                                 135,1         Bot

 
