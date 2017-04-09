#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include<stdlib.h>
#include<semaphore.h>

#define W_SIZE 1024
char w_area[W_SIZE];
sem_t bin_sem;

void *thread_func(void *arg) {
	sem_wait(&bin_sem);
	while(strncmp("end",w_area,3) !=0) {
		printf("you input %d characters.\n",strlen(w_area)-1);
		sem_wait(&bin_sem);
	}
	pthread_exit(NULL);
}

int main() {
	int r;
	pthread_t th;
	void *t_result;
	
	r = sem_init(&bin_sem,0,0);
	if (r != 0) {
		perror("Semaphore Initialization failed\n");
		exit(EXIT_FAILURE);
	}
	r = pthread_create(&th,NULL, thread_func, NULL);
	if (r != 0) {
		perror("thread creation failed\n");
		exit(EXIT_FAILURE);
	}
	printf("Input some text. Enter 'end' to finish\n");
	while(strncmp("end",w_area,3) != 0) {
		fgets(w_area, W_SIZE, stdin);
		sem_post(&bin_sem);
	}
	printf("\n Waiting for thread to finish.....\n");
	r = pthread_join(th, &t_result);
	if (r != 0) {
		perror("thread join failed\n");
		exit(EXIT_FAILURE);
	}
	printf("thread joined\n");
	sem_destroy(&bin_sem);
	exit(EXIT_SUCCESS);
	return 0;
}
