#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define THREADS 10

void *func(void *);

// shared data stuff
char **data;
int length;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv) {
	srand(time(NULL));
	pthread_t tids[THREADS];
	// initialize data to NULL
	data = NULL;

	// start all of the threads
	for(unsigned long i = 0; i < THREADS; ++i) {
		int err = pthread_create(&tids[i], NULL, func, (void*) i);
		if (err != 0) fprintf(stderr, "! Couldn't create thread");
	}

	// wait for threads and join them
	for(int i = 0; i < THREADS; ++i) {
		pthread_join(tids[i], NULL);
	}

	// print out data
	for(int i = 0; i < length; ++i) {
		// print out the data
		printf("%s", data[i]);
		free(data[i]);
	}
	free(data);

	return 0;
}

/**
 *
 * Thread Function
 *
 */
void *func(void *arg) {
	unsigned long num = (unsigned long) arg;
	int naptime = rand() % 10 + 1;
	printf("#%lu: Sleeping for %d seconds...\n", num, naptime);
	sleep(naptime);

	// critical section
	pthread_mutex_lock(&lock);
	// resize the array to be at least big enough for the current index
	if (length < (num + 1)) {
		length = num + 1;
	}
	data = realloc(data, sizeof(char*) * length);
	data[num] = strdup("Hello, World!\n");
	pthread_mutex_unlock(&lock);

	printf("#%lu: ...done!\n", num);

	return NULL;
}
