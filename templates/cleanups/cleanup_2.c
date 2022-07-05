#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

pthread_mutex_t mutex_1=PTHREAD_MUTEX_INITIALIZER;

pthread_t thread1;
pthread_t thread2;

void unlock(void *v)
{
	pthread_mutex_unlock(&mutex_1);
}


void *thread1fkt(void *param)
{ 

	pthread_cleanup_push(unlock, NULL);
	printf("%s: Ich will in den krit. Bereich...\n", (char *)param);
	pthread_mutex_lock(&mutex_1);
	printf("%s: Ich bin drin.... :-)\n", (char *)param);
	getchar();
	pthread_mutex_unlock(&mutex_1);
	printf("%s: Wieder draußen\n", (char *)param);
	pthread_cleanup_pop(0);
	return "Ende1 !";
}

void *thread2fkt(void *param)
{
	int erg;
	pthread_cleanup_push(unlock, NULL);
	printf("%s: Ich will auch in den krit. Bereich...\n", (char *)param);
	erg=pthread_mutex_trylock(&mutex_1);

	if(erg == EBUSY) {
		printf("%s: Bin noch nicht drin, werde es aber gleich sein!!\n", (char *)param);
		sleep(2);
		pthread_cancel(thread1);
	}
	
	pthread_mutex_lock(&mutex_1);	
	printf("%s: Ich bin auch drin.... :-)\n", (char *)param);
	sleep(4);
	printf("%s: ausgeschlafen.... :-)\n", (char *)param);	
	pthread_cleanup_pop(1);
	return "Ende2 !";
}

int main(void)
{
	int retval;
	void *retptr;

	if((retval = pthread_create(&thread1, NULL, thread1fkt, "T1"))) {
		fprintf(stderr, "error creating thread1: %s\n", strerror(retval));
		return EXIT_FAILURE;
	}
	
	if((retval = pthread_create(&thread2, NULL, thread2fkt, "T2"))) {
		fprintf(stderr, "error creating thread2: %s\n", strerror(retval));
		return EXIT_FAILURE;
	}

	
	if ((retval = pthread_join(thread1, &retptr))) {
		fprintf(stderr, "error joining thread1: %s\n", strerror(retval));
		return EXIT_FAILURE;
	}

	if(retptr == PTHREAD_CANCELED) 
	    puts("T1 gecancelt");
	else
	    puts(retptr);
	
	
	if ((retval = pthread_join(thread2, &retptr))) {
		fprintf(stderr, "error joining thread2: %s\n", strerror(retval));
		return EXIT_FAILURE;
	}

	puts(retptr);
	return 0;
}
