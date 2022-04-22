#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

// Global Variables
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int found_files;
char **mp3_data;


// Thread function
void* thread_func(void* arg){

  unsigned char *dirnum = (unsigned char*)arg;

  char dirname[4] = "dir0";

  // Critical Section
  pthread_mutex_lock(&mutex);



    printf("%s\n", dirname);


  pthread_mutex_unlock(&mutex);


  return NULL;
}


int main(int argc, char **argv){

  // Command line error handling
  if(argc != 3) return -1;

  struct dirent *entry;

  // Set directory to user specification
  chdir(argv[1]);

  DIR *current = opendir(".");
  int root_dir_counter = -2;

  while((entry = readdir(current))){
    if(entry->d_type == DT_DIR){
      root_dir_counter++;
    }
  } 

  pthread_t tids[root_dir_counter];

  // Start appropriate amount of threads
  for(unsigned long i = 0; i < root_dir_counter; i++){


    int thread_create_errno = pthread_create(&tids[i], NULL, thread_func, (void*) i);
    if(thread_create_errno != 0){
      printf("Error creating thread\n Error Number: %d", thread_create_errno);
      return -2;
    }
  }


  // Wait for all threads to be done before we continue
  for(int i = 0; i < root_dir_counter; i++){
    pthread_join(tids[i], NULL);
  }

  return 0;
}
