#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long long counter = 0;
const int n = 4;
const int m = 10000;
pthread_spinlock_t spinlock;

void* increment(void* args) {
        for (int i = 0; i < m; i++) {
                pthread_spin_lock(&spinlock);
                counter++;
                pthread_spin_unlock(&spinlock);
        }
        return NULL;
}
 
 
int main() {
	pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);

        pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * n);
	 
        for (int i = 0; i < n; i++) {
                pthread_create(&threads[i], NULL, increment, NULL); 
        }
	for (int i = 0; i < n; i++)
		pthread_join(threads[i], NULL);
 
        printf("Expected value of counter: %d\n", m * n);
        printf("Actual value of counter: %lld\n", counter);

	pthread_spin_destroy(&spinlock); 
	free(threads);
	threads = NULL;

        return 0;
}
