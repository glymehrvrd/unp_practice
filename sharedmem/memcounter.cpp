#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include "stdio.h"
#include "stdlib.h"

#include <semaphore.h>

struct sem_counter{
	sem_t mutex;
	int counter;
};

int main(){
	shm_unlink("/memcounter");
	int fd = shm_open("/memcounter",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	if(fd<0){
		perror("shmopen");
		exit(-1);
	}
	if(ftruncate(fd, 1024)<0){
		perror("ftruncate");
		exit(-1);
	};

	void *addr = mmap(NULL,1024,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	sem_counter *sc = (sem_counter *)addr;

	sem_init(&sc->mutex,1,1);

	if(fork()==0){
		// child
		for(int i=0;i<10;i++){
			sem_wait(&sc->mutex);
			printf("child:\t%d\n",sc->counter++);
			sem_post(&sc->mutex);
			usleep(100000);
		}
		exit(0);
	}

	for(int i=0;i<10;i++){
		sem_wait(&sc->mutex);
		printf("parent:\t%d\n",sc->counter++);
		sem_post(&sc->mutex);
		usleep(100000);
	}
	return 0;
}
