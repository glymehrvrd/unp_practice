#include "rwlock.h"
#include "stdio.h"
#include <unistd.h>
#include <pthread.h>

struct rw_lock_t lck;
pthread_t tids[10];

void *func1(void *arg){
	printf("1: getting read lock.\n");
	rw_lock_readlock(&lck);
	printf("1: read lock.\n");
	sleep(2);
	
	printf("1: getting read unlock.\n");
	rw_unlock_readlock(&lck);
	printf("1: read unlock.\n");
	sleep(2);

	printf("1: getting write lock.\n");
	rw_lock_writelock(&lck);
	printf("1: write lock.\n");
	sleep(4);

	printf("1: getting write unlock.\n");
	rw_unlock_writelock(&lck);
	printf("1: write unlock.\n");
	sleep(1);

	printf("1: getting read lock.\n");
	rw_lock_readlock(&lck);
	printf("1: read lock.\n");
}

void *func2(void *arg){
	sleep(1);

	printf("2: getting read lock.\n");
	rw_lock_readlock(&lck);
	printf("2: read lock.\n");
	sleep(4);
	
	printf("2: getting read unlock.\n");
	rw_unlock_readlock(&lck);
	printf("2: read unlock.\n");
	sleep(1);

	printf("2: getting write lock.\n");
	rw_lock_writelock(&lck);
	printf("2: write lock.\n");
	sleep(4);

	printf("2: getting write unlock.\n");
	rw_unlock_writelock(&lck);
	printf("2: write unlock.\n");
}

int main(){
	rw_lock_init(&lck);

	pthread_setconcurrency(5);
	pthread_create(&tids[0],NULL,func1,NULL);
	pthread_create(&tids[1],NULL,func2,NULL);

	pthread_join(tids[0],NULL);
	pthread_join(tids[1],NULL);

	rw_lock_destroy(&lck);
	return 0;
}
