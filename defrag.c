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

  char* dirname = arg;

  //printf("%s\n", dirname);

  if(!(dir = opendir(dirname))) return NULL;

  while((entry = readdir(dir))){        

    if(entry->d_type == DT_DIR){

      char* path = strdup(dirname);
      
      if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

      
      strcat(path, "/");
      strcat(path, entry->d_name);

      //printf("%s\n", path);

      thread_func(path);

    }

    if(entry->d_type == DT_REG){
      printf("%s\n", entry->d_name);


      pthread_mutex_lock(&mutex);    


      found_files++;


      pthread_mutex_unlock(&mutex);
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
  char **root_dirs = malloc(sizeof(char*) * root_dir_counter);

  // Start appropriate amount of threads
  for(int i = 0; i < root_dir_counter; i++){
    char dir[4] = "dir0";
    dir[3] = i + '0';
    char *dir_literal = dir;

    root_dirs[i] = malloc(sizeof(char) * 4);
    strncpy(root_dirs[i], dir_literal, sizeof(char) * 4);

    //printf("%s\n", root_dirs[i]);
    int thread_create_errno = pthread_create(&tids[i], NULL, thread_func, root_dirs[i]);
    if(thread_create_errno != 0){
      printf("Error creating thread\n Error Number: %d", thread_create_errno);
      return -2;
    }
  }

  // Wait for all threads to be done before we continue
  for(int i = 0; i < root_dir_counter; i++){
    pthread_join(tids[i], NULL);
    //printf("Thread: %ld closed\n", tids[i]);
  }

  printf("%d\n", found_files);

  return 0;
}
