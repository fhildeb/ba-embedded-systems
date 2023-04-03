#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t mutex_1=PTHREAD_MUTEX_INITIALIZER;

void *boris(void *param)
{
	printf("%s: Bin ich nun drin oder was...\n", (char *)param);
	pthread_mutex_lock(&mutex_1);
	printf("%s: Ich bin drin.... :-)\n", (char *)param);
	getchar();
	pthread_mutex_unlock(&mutex_1);
	printf("%s: Und wieder draussen.... :-)\n", (char *)param);
	return NULL;
}

int main(void)
{
	int retval;
	pthread_t thread1;
	pthread_t thread2;

	if((retval = pthread_create(&thread1, NULL, boris, "Bo1"))) {
		fprintf(stderr, "error creating thread1: %s\n", strerror(retval));
		return EXIT_FAILURE;
	}
	
	if((retval = pthread_create(&thread2, NULL, boris, "Bo2"))) {
		fprintf(stderr, "error creating thread2: %s\n", strerror(retval));
		return EXIT_FAILURE;
	}

	if ((retval = pthread_join(thread2, NULL))) {
		fprintf(stderr, "error joining thread2: %s\n", strerror(retval));
		return EXIT_FAILURE;
	}
	puts("Bo2 fertig!");
	
	if ((retval = pthread_join(thread1, NULL))) {
		fprintf(stderr, "error joining thread2: %s\n", strerror(retval));
		return EXIT_FAILURE;
	}
	puts("Bo1 fertig!");
}