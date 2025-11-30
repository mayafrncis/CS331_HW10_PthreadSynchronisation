#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semA, semB, semC;

pthread_mutex_t print_mutex;

const int n = 10;

void* a(void* args) {
	for (int i = 0; i < n; i++) {
		sem_wait(&semA);
		pthread_mutex_lock(&print_mutex);
		printf("Thread A: %d\n", i);
		pthread_mutex_unlock(&print_mutex);
		sem_post(&semB);
	}
	return NULL;
}

void* b(void* args) {
	for (int i = 0; i < n; i++) {
		sem_wait(&semB);
		pthread_mutex_lock(&print_mutex);
		printf("Thread B: %d\n", i);
		pthread_mutex_unlock(&print_mutex);
		sem_post(&semC);
	}
	return NULL;
}

void* c(void* args) {
	for (int i = 0; i < n; i++) {
		sem_wait(&semC);
		pthread_mutex_lock(&print_mutex);
		printf("Thread C: %d\n", i);
		pthread_mutex_unlock(&print_mutex);
		sem_post(&semA);
	}
	return NULL;
}

int main() {
	sem_init(&semA,0,1);
	sem_init(&semB,0,0);
	sem_init(&semC,0,0);

	pthread_t threadA, threadB, threadC;

	pthread_create(&threadA, NULL, a, NULL);
	pthread_create(&threadB, NULL, b, NULL);
	pthread_create(&threadC, NULL, c, NULL);

	pthread_join(threadA, NULL);
	pthread_join(threadB, NULL);
	pthread_join(threadC, NULL);

	sem_destroy(&semA);
	sem_destroy(&semB);
	sem_destroy(&semC);
	pthread_mutex_destroy(&print_mutex);
	return 0;
}
