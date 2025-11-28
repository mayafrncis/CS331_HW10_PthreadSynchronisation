#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> //https://www.geeksforgeeks.org/c/measure-execution-time-with-high-precision-in-c-c/?utm_source=chatgpt.com
#include <string.h>
#include <unistd.h>

long long balance = 0;
const int n = 4;
int is_mutex;
int is_long;

pthread_mutex_t mutex;
pthread_spinlock_t spinlock;

void lock() {
	if (is_mutex) pthread_mutex_lock(&mutex);
	else if (!is_mutex) pthread_spin_lock(&spinlock);
}

void unlock() {
	if (is_mutex) pthread_mutex_unlock(&mutex);
        else if (!is_mutex) pthread_spin_unlock(&spinlock);
}

void* deposit(void* args) {
	lock();
	balance++;
	if (is_long) usleep(100);
	unlock();
	return NULL;
}

void* withdraw(void* args) {
	lock();
	balance--;
	if (is_long) usleep(100);
	unlock();
	return NULL;
}

int main(int argc, char* argv[]) {  //https://www.geeksforgeeks.org/cpp/command-line-arguments-in-c-cpp/
	struct timespec start, end;

	clock_gettime(CLOCK_MONOTONIC, &start);

	if (argc != 3) {
		printf("Incorrect commandline arguments\n");
		exit(1);
	}

	is_mutex = strcmp(argv[1], "mutex") == 0;
	is_long = strcmp(argv[2],"long") == 0;

	if (!is_mutex) pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);


	pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * n);
	for (int i = 0; i < n; i+=2) {
		pthread_create(&threads[i], NULL, deposit, NULL);
		pthread_create(&threads[i+1], NULL, withdraw, NULL);
	}

	for (int i = 0; i < n; i++)
		pthread_join(threads[i], NULL);

	if (!is_mutex) pthread_spin_destroy(&spinlock);

	clock_gettime(CLOCK_MONOTONIC, &end);

	double time = ((end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)) * 1e-9;

	printf("Final balance: %lld\n", balance);
	printf("Time taken: %f\n", time);


	return 0;
}
