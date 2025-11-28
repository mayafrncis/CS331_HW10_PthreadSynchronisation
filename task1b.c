#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>

long long counter = 0;
const int n = 4;
const int m = 10000;
pthread_mutex_t mutex;

void* increment(void* args) {
	for (int i = 0; i < m; i++) {
		pthread_mutex_lock(&mutex);
		counter++;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
 
 
int main() {
	pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * n);
 
	for (int i = 0; i < n; i++) {
		pthread_create(&threads[i], NULL, increment, NULL); 
	}
	for (int i = 0; i < n; i++)
		pthread_join(threads[i], NULL);
 
	printf("Expected value of counter: %d\n", m * n);
	printf("Actual value of counter: %lld\n", counter);
 
	return 0;
}

