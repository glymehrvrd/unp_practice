#include <pthread.h>
#include "rwlock.h"

int rw_lock_init(rw_lock_t *lock){
	int result;

	if((result=pthread_mutex_init(&lock->mutex,NULL))!=0){
		goto err1;
	}

	if((result=pthread_cond_init(&lock->rcond,NULL))!=0){
		goto err2;
	}

	if((result=pthread_cond_init(&lock->wcond,NULL))!=0){
		goto err3;
	}

	lock->rcnt=0;
	lock->wcnt=0;
	return 0;

err3:
	pthread_cond_destroy(&lock->rcond);
err2:
	pthread_mutex_destroy(&lock->mutex);
err1:
	return result;
}

int rw_lock_readlock(rw_lock_t *lock){
	pthread_mutex_lock(&lock->mutex);

	while(lock->wcnt!=0)
		pthread_cond_wait(&lock->rcond,&lock->mutex);
	lock->rcnt++;
	pthread_mutex_unlock(&lock->mutex);
}

int rw_lock_writelock(rw_lock_t *lock){
	pthread_mutex_lock(&lock->mutex);

	while(lock->wcnt!=0 || lock->rcnt!=0)
		pthread_cond_wait(&lock->wcond,&lock->mutex);
	lock->wcnt++;
	pthread_mutex_unlock(&lock->mutex);
}

int rw_unlock_readlock(rw_lock_t *lock){
	pthread_mutex_lock(&lock->mutex);

	if(lock->rcnt!=0){
		lock->rcnt--;
		pthread_cond_signal(&lock->wcond);
	}
	pthread_mutex_unlock(&lock->mutex);
}

int rw_unlock_writelock(rw_lock_t *lock){
	pthread_mutex_lock(&lock->mutex);

	if(lock->wcnt!=0){
		lock->wcnt--;
		pthread_cond_signal(&lock->rcond);
		pthread_cond_signal(&lock->wcond);
	}
	pthread_mutex_unlock(&lock->mutex);
}

int rw_lock_destroy(rw_lock_t *lock){
	pthread_cond_destroy(&lock->rcond);
	pthread_cond_destroy(&lock->wcond);
	pthread_mutex_destroy(&lock->mutex);
}
