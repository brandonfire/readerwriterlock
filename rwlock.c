#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
typedef struct rwlock_t {
	pthread_cond_t writejoin; //use condition variable instead of semaphore
	pthread_mutex_t lock; // mutex lock
	sem_t writelock; // used to allow ONE writer or MANY readers
    sem_t writeonly;//use semaphore to proctect write++and write--
	int readers; // count of readers reading in critical section
	int writer;//signal writer join in
	} rwlock_t;
void rwlock_init(rwlock_t *rw) {
	rw->readers = 0;
	rw->writer = 0; //add a writer value
	pthread_mutex_init(&rw->lock, NULL);//sem_init(&rw->lock, 0, 1);
	sem_init(&rw->writelock, 0, 1);
	sem_init(&rw->writeonly, 0, 1);
	rw->writejoin = (pthread_cond_t) PTHREAD_COND_INITIALIZER;
}
void acquire_readlock(rwlock_t *rw) {
	pthread_mutex_lock(&rw->lock);//sem_wait(&rw->lock);
	while(rw->writer >0)
	pthread_cond_wait(&rw->writejoin, &rw->lock);// check whether there is one or more writers, if exist writer, release lock and wait for signal
	rw->readers++;
	if (rw->readers == 1){
		sem_wait(&rw->writelock);} // first reader acquires write lock
    	pthread_mutex_unlock(&rw->lock);
}
void release_readlock(rwlock_t * rw) {
	pthread_mutex_lock(&rw->lock);
	rw->readers--;
	if (rw->readers == 0){
		printf("no readers now, writer lock is free! \n");
		sem_post(&rw->writelock);}// last reader releases write lock
	pthread_mutex_unlock(&rw->lock);
}
void acquire_writelock(rwlock_t *rw) {
    sem_wait(&rw->writeonly);
    rw->writer++;//critical region between writers
	printf("writer join in. readers please wait. \n");
    sem_post(&rw->writeonly);
    sem_wait(&rw->writelock);
    
}
void release_writelock(rwlock_t *rw) {
    sem_wait(&rw->writeonly);//proctect writer-- and if writer==0
	rw->writer--;
	if(rw->writer ==0)
	{	
		pthread_cond_broadcast(&rw->writejoin);//if no writers then broadcast readers. good news!!! I think finally the lock is okay-- chengbin hu.
		printf("no writer now, readers can join in.\n");
	}
	sem_post(&rw->writeonly);
	sem_post(&rw->writelock);
    
}

void *reader(void *rw){
	rwlock_t *m = (rwlock_t *) rw;
	acquire_readlock(m);
	printf("reader lock acquired\n");
	sleep(0.5);
	release_readlock(m);
	
}
void *writer(void *rw){
    rwlock_t *m = (rwlock_t *) rw;
	acquire_writelock(m);
	printf("writer lock acquired. Wait for 1 second write action\n");
	sleep(1);
	printf("writer job done\n");
	release_writelock(m);
}

int main(int argc, char *argv[]){
	if(argc != 4) {
        fprintf(stderr, "ERROR please input three arguments: number of first group of readers, number of writers, number of sencond group of reader. \n");
        exit(1);
    	}
	int b,d,a,c,e,f;
	a = atoi(argv[1]);
	b = atoi(argv[2]);
	e = atoi(argv[3]);
	printf("We have %d reader come first, and then %d writers come, and finally %d readers come. \n",a,b,e);
	sleep(1);
	rwlock_t rw;
	rwlock_init(&rw);
	pthread_t thread_id[a];
	pthread_t thwrite_id[b];
	pthread_t threader_id[e];
	for(c=0;c<a;c++){
    	pthread_create(&thread_id[c],NULL,reader,&rw); 
    	printf("readers %d created.\n",c);
	}
	for(d=0;d<b;d++){
 		pthread_create(&thwrite_id[d],NULL,writer,&rw);
 		printf("writers %d created.\n",d);
	}
	for(f=0;f<e;f++){
		pthread_create(&threader_id[f],NULL,reader,&rw);
		printf("second group readers %d created.\n",f);
	}
	for(c=0;c<a;c++){
		pthread_join(thread_id[c],NULL);}
	for(d=0;d<b;d++){
		pthread_join(thwrite_id[d],NULL);}
	for(f=0;f<e;f++){
		pthread_join(threader_id[f],NULL);}
	
	sleep(1);
	return 0;
}
