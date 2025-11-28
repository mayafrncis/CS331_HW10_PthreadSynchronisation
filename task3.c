#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

pthread_mutex_t buffer_mutex;

#define BUFFER_SIZE 8
int buffer[BUFFER_SIZE];
int in_pos = 0;
int out_pos = 0;

sem_t empty_slots;
sem_t full_slots;

const int n = 4;
int produced = 0;
int k = 10;
int consumed = 0;

void* producer(void* args) {
	while (1) {
		pthread_mutex_lock(&buffer_mutex);
		if (produced >= k) {
			pthread_mutex_unlock(&buffer_mutex);
			sem_post(&full_slots);
			break;
		}
		pthread_mutex_unlock(&buffer_mutex);
		sem_wait(&empty_slots);
		pthread_mutex_lock(&buffer_mutex);
		buffer[in_pos] = 1;
		printf("Produced an item.\n");
		in_pos = (++in_pos) % BUFFER_SIZE;
		produced++;
		pthread_mutex_unlock(&buffer_mutex);
		sem_post(&full_slots);
	}
	return NULL;
}

void* consumer(void* args) {
	while (1) {
		pthread_mutex_lock(&buffer_mutex);
		if (consumed >= k) {
			pthread_mutex_unlock(&buffer_mutex);
			sem_post(&empty_slots);
			break;
		}
		pthread_mutex_unlock(&buffer_mutex);
		sem_wait(&full_slots);
		pthread_mutex_lock(&buffer_mutex);
		printf("Consumed an item.\n");
		out_pos = (++out_pos) % BUFFER_SIZE;
		consumed++;
		pthread_mutex_unlock(&buffer_mutex);
		sem_post(&empty_slots);
	}
	return NULL;

}

int main() {
	pthread_t* p = (pthread_t*) malloc(sizeof(pthread_t) * n);
	pthread_t* c = (pthread_t*) malloc(sizeof(pthread_t) * n);

	sem_init(&empty_slots, 0, BUFFER_SIZE);
	sem_init(&full_slots, 0, 0);

	for (int i = 0; i < n; i++) {
		pthread_create(&p[i], NULL, producer, NULL);
                pthread_create(&c[i], NULL, consumer, NULL);
	}

	for (int i = 0; i < n; i++) {
		pthread_join(p[i], NULL);
		pthread_join(c[i], NULL);
	}

	sem_destroy(&empty_slots);
	sem_destroy(&full_slots);
	pthread_mutex_destroy(&buffer_mutex);

	printf("Program is done\n");

	return 0;
}
