#include <pthread.h>

struct rw_lock_t{
        pthread_mutex_t mutex;
        pthread_cond_t rcond;
        pthread_cond_t wcond;
        int rcnt;
        int wcnt;
};

int rw_lock_init(rw_lock_t *lock);
int rw_lock_readlock(rw_lock_t *lock);
int rw_lock_writelock(rw_lock_t *lock);
int rw_unlock_readlock(rw_lock_t *lock);
int rw_unlock_writelock(rw_lock_t *lock);
int rw_lock_destroy(rw_lock_t *lock);
