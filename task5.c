#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

const int k = 2, n = 15;
sem_t printers;

void* print(void* args) {
	int i = (int)(long)args;
	sem_wait(&printers);
	printf("Thread %d is printing...\n", i);
	usleep(50);
	printf("Thread %d is done printing.\n", i);
	sem_post(&printers);

	return NULL;
}


int main() {
	sem_init(&printers, 0, k);

	pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * n);
	for (int i = 0; i < n; i++) {
		pthread_create(&threads[i], NULL, print, (void*)(long)(i+1));
	}

	for (int i = 0; i < n; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);
	threads = NULL;
	sem_destroy(&printers);
	return 0;
}

