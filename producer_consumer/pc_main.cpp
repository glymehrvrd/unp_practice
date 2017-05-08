#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#include <semaphore.h>

static struct {
	int buff[10];
	int in;
	int out;
} store;

sem_t sput, sget, mutex;

void *producer(void *arg){
	printf("produce\n");
	int i=0;
	while(i<5){
		sem_wait(&sput);
		sem_wait(&mutex);
		store.buff[store.out]=i;
		printf("producing %d at pos %d\n", i, store.out);
		store.out++;
		i++;
		sem_post(&mutex);
		sem_post(&sget);
	}
}

void *consumer(void *arg){
	printf("consume\n");
	for(;;){
		sem_wait(&sget);
		sem_wait(&mutex);
		printf("consuming %d at pos %d\n", store.buff[store.in], store.in);
		store.in++;
		if(store.in==store.out){
			printf("now I restore it\n");
			for(int i=store.in;i>0;--i){
				store.in--;
				store.out--;
				printf("in %d out %d\n",store.in, store.out);
				sem_post(&sput);
			}
		}
		sem_post(&mutex);
	}
}

int main(int argc, char *argv[]){
	bzero(store.buff,10*sizeof(int));
	store.in=0;
	store.out=0;

	sem_init(&sput,0,10);
	sem_init(&sget,0,0);
	sem_init(&mutex,0,1);

	pthread_setconcurrency(10);
	pthread_t tid_producer[5], tid_consumer[5];
	pthread_create(&tid_producer[0], NULL, producer, NULL);
	pthread_create(&tid_producer[1], NULL, producer, NULL);
	pthread_create(&tid_producer[2], NULL, producer, NULL);
	pthread_create(&tid_producer[3], NULL, producer, NULL);
	pthread_create(&tid_producer[4], NULL, producer, NULL);

	pthread_create(&tid_consumer[0], NULL, consumer, NULL);
	pthread_create(&tid_consumer[1], NULL, consumer, NULL);
	pthread_create(&tid_consumer[2], NULL, consumer, NULL);
	pthread_create(&tid_consumer[3], NULL, consumer, NULL);
	pthread_create(&tid_consumer[4], NULL, consumer, NULL);
	
	for(int i=0;i<5;++i){
		pthread_join(tid_producer[i],NULL);
		pthread_join(tid_consumer[i],NULL);
	}
}
