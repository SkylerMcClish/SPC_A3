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

  DIR *dir;
  struct dirent *entry;

  unsigned long num = (unsigned long) arg;
  
  char dirname[4] = "dir0";
  dirname[3] = num + '0';

  if(!(dir = opendir(dirname))) return NULL;

  unsigned long dir_counter = 0;

  while((entry = readdir(dir)) != NULL){

    if(entry->d_type == DT_DIR){
      if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
      
      printf("dir%ld\n", dir_counter);

      thread_func((void*) dir_counter);

      dir_counter++;
    }

    if(entry->d_type == DT_REG){
      printf("%s\n", entry->d_name);
    }

    if(entry->d_type == DT_LNK){
      printf("Its a trap!\n");
    }


  }




  // Critical Section
  pthread_mutex_lock(&mutex);    


  pthread_mutex_unlock(&mutex);

  closedir(dir);

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
