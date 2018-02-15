#include <pthread.h>
#include <stdio.h>

/* function prototypes for threads */
void* thread1();
void* thread2();

void
main()
{
	/* declare variables */
	pthread_t	i1;
	pthread_t	i2;
	
	/* initialize thread definitions */
	pthread_create(&i1, NULL, (void*)&thread1, NULL);
	pthread_create(&i2, NULL, (void*)&thread2, NULL);
	
	/* wait for threads to complete, then print */
	pthread_join(i1, NULL);
	pthread_join(i2, NULL);
	printf("Hello, I am the main process.\n");
}

/* function definitions for threads */
void*
thread1()
{
	printf("Hello, I am thread 1.\n");
	return NULL;
}

void*
thread2()
{
	printf("Hello, I am thread 2.\n");
	return NULL;
}

