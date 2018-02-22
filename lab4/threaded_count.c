#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t lock;

void *threadCounter(void *param){
    int *args = (int *)param;
    int i;
    for(i = 0; i < 1000000; i++){
    	pthread_mutex_lock(&lock);
        (*args)++;
        pthread_mutex_unlock(&lock);
    }
}

int main(int argc, char** argv){
    pthread_t t1;
    pthread_t t2;
    int shared = 0;
    int err;
    err = pthread_mutex_init(&lock, NULL);
    if(err != 0){
    	printf("Error. Could not init mutex.\n");
    	exit(err);
    }
    
    err = pthread_create(&t1, NULL, threadCounter, (void *)&shared);
    if(err != 0){
        errno = err;
        perror("pthread_create");
        exit(1);
    }
    
    err = pthread_create(&t2, NULL, threadCounter, (void *)&shared);
    if(err != 0){
        errno = err;
        perror("pthread_create");
        exit(1);
    }
    
    err = pthread_join(t1, NULL);
    if(err != 0){
        errno = err;
        perror("pthread_join");
        exit(1);
    }
    
    err = pthread_join(t2, NULL);
    if(err != 0){
        errno = err;
        perror("pthread_join");
        exit(1);
    }
    
    err = pthread_mutex_destroy(&lock);
    if(err != 0){
    	printf("Error. Could not destroy mutex.\n");
    	exit(err);
    }
    printf("After both threads are done executing, `shared` = %d\n", shared);
    return 0;
}
