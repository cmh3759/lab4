#include <stdio.h>
#include <pthread.h>
void *producer(void *);
void *consumer(void *);

int buffer[100];
int count = 0;
int in = -1;
int out = -1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_has_space = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_has_data = PTHREAD_COND_INITIALIZER;

int main(){
	int i;
	pthread_t thread[2];
	pthread_create(&thread[0], NULL, producer, NULL);
	pthread_create(&thread[1], NULL, consumer, NULL);
	for(i=0;i<2;i++){
		pthread_join(thread[i],NULL);
	}
	return 0;
}

void *producer(void *arg){
	int i;
	for(i=0; i<100; i++){
		pthread_mutex_lock(&mutex);
		if(count == 10)
			pthread_cond_wait(&buffer_has_space, &mutex);
		
		in++;
		in %=100;
		buffer[in] = i;
		count++;
		pthread_cond_signal(&buffer_has_data);
		pthread_mutex_unlock(&mutex);
		printf("Producer");
	}
}

void *consumer(void *arg){
	int i, mutexData;
	for(i=0; i<100; i++){
		pthread_mutex_lock(&mutex);
		if(count == 0)
			pthread_cond_wait(&buffer_has_data, &mutex);
		
		out++;
		out %=10;
		mutexData = buffer[out];
		count--;
		pthread_cond_signal(&buffer_has_space);
		pthread_mutex_unlock(&mutex);
		printf("mutexData[2]= %d\n",mutexData); 
	}
}