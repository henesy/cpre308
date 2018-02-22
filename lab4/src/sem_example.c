#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>

#define MAX_PRODUCE 40

sem_t buffer_items;
sem_t buffer_slots;

// Producer thread
void *producer(void *args){
	// Produces ints and puts them into the buffer
	// until the value of `buffer_items` gets
	// to 20, then wait for the consumer to consume
	// the ints
	int i = 0;
	int sem_val;
	int err;
	int tail = 0;
	int *buffer = (int *)args;
	while(i < MAX_PRODUCE){
		sem_wait(&buffer_slots);
		buffer[tail % 20] = rand() % MAX_PRODUCE;
		flockfile(stdout);
		fprintf(stdout, "Producer: element %d: %d; placed in the tail of the buffer\n", i, buffer[tail & 20]);
		funlockfile(stdout);
		i++;
		tail++;
		sem_post(&buffer_items);
	}
}

// Consumer thread
void *consumer(void *args){
	// Consumes the ints that are put into the buffer
	// by the producer until either all the ints produced
	// by the Producer is consumed, or the buffer is empty
	int i = 0;
	int sem_val;
	int err;
	int head = 0;
	int *buffer = (int *)args;
	while(i < MAX_PRODUCE){
		sem_wait(&buffer_items);
		flockfile(stdout);
		fprintf(stdout, "Consumer: element %d: %d; read from the head of the buffer\n", i, buffer[head % 20]);
		funlockfile(stdout);
		buffer[head % 20] = -1;
		i++;
		head++;
		sem_post(&buffer_slots);
	}
}

int main(int argc, char **argv){
	int err;
	int *buffer;
	pthread_t t1;
	pthread_t t2;

	buffer = calloc(20, sizeof(int));
	if(NULL == buffer){
		perror("calloc");
		return -1;
	}

	err = sem_init(&buffer_items, 0, 0);
	if(err){
		perror("sem_init");
		free(buffer);
		return -1;
	}
	
	err = sem_init(&buffer_slots, 0, 20);
	if(err){
		perror("sem_init");
		free(buffer);
		sem_destroy(&buffer_items);
		return -1;
	}

	err = pthread_create(&t1, NULL, producer, buffer);
	if(err){
		errno = err;
		perror("pthread_create");
		sem_destroy(&buffer_items);
		sem_destroy(&buffer_slots);
		free(buffer);
		return -1;
	}

	err = pthread_create(&t2, NULL, consumer, buffer);
	if(err){
		errno = err;
		perror("pthread_create");
		sem_destroy(&buffer_items);
		sem_destroy(&buffer_slots);
		free(buffer);
		return -1;
	}

	err = pthread_join(t1, NULL);
	if(err){
		errno = err;
		perror("pthread_join");
		sem_destroy(&buffer_items);
		sem_destroy(&buffer_slots);
		free(buffer);
		return -1;
	}

	err = pthread_join(t2, NULL);
	if(err){
		errno = err;
		perror("pthread_join");
		sem_destroy(&buffer_items);
		sem_destroy(&buffer_slots);
		free(buffer);
		return -1;
	}

	sem_destroy(&buffer_items);
	sem_destroy(&buffer_slots);
	free(buffer);

	return 0;
}
