#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

int test1;
int test2;
pthread_mutex_t lock;
pthread_cond_t can_cond;
pthread_cond_t I_cond;


void *printHello(void *args){
	pthread_mutex_lock(&lock);
	flockfile(stdout);
	fprintf(stdout, "Never going to give you up\n");
	funlockfile(stdout);
	pthread_cond_signal(&can_cond);
	test1 = 1;
	pthread_mutex_unlock(&lock);
}

void *printCanYouHearMe(void *args){
	pthread_mutex_lock(&lock);
	while(test1 == 0){
		pthread_cond_wait(&can_cond, &lock);
	}
	flockfile(stdout);
	fprintf(stdout, "Never going to let you down\n");
	funlockfile(stdout);
	pthread_cond_broadcast(&I_cond);
	test2 = 1;
	pthread_mutex_unlock(&lock);
}

void *printIWasWondering(void *args){
	pthread_mutex_lock(&lock);
	while(test2 == 0){
		pthread_cond_wait(&I_cond, &lock);
	}
	flockfile(stdout);
	fprintf(stdout, "Never going to run around");
	funlockfile(stdout);
	pthread_mutex_unlock(&lock);
}

int main(int argc, char **argv){
	pthread_t t[3];
	int i;
	int err;
	test1 = 0;
	test2 = 0;

	err = pthread_mutex_init(&lock, NULL);
	if(err){
		errno = err;
		perror("mutex init");
		return -1;
	}

	err = pthread_cond_init(&can_cond, NULL);
	if(err){
		errno = err;
		perror("cond init");
		pthread_mutex_destroy(&lock);
		return -1;
	}

	err = pthread_cond_init(&I_cond, NULL);
	if(err){
		errno = err;
		perror("cond init");
		pthread_mutex_destroy(&lock);
		pthread_cond_destroy(&can_cond);
		return -1;
	}

	for(i = 0; i < 3; i++){
		if(i == 0){
			err = pthread_create(&t[i], NULL, printHello, NULL);
		}else if(i == 1){
			err = pthread_create(&t[i], NULL, printCanYouHearMe, NULL);
		}else{
			err = pthread_create(&t[i], NULL, printIWasWondering, NULL);
		}
		if(err){
			errno = err;
			perror("pthread create");
			pthread_mutex_destroy(&lock);
			pthread_cond_destroy(&can_cond);
			pthread_cond_destroy(&I_cond);
			return -1;
		}
	}

	for(i = 0; i < 3; i++){
		err = pthread_join(t[i], NULL);
		if(err){
			errno = err;
			perror("pthread join");
			pthread_mutex_destroy(&lock);
			pthread_cond_destroy(&can_cond);
			pthread_cond_destroy(&I_cond);
			return -1;
		}
	}

	printf(" and desert you\n");

	// clean up
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&can_cond);
	pthread_cond_destroy(&I_cond);

	return 0;
}
