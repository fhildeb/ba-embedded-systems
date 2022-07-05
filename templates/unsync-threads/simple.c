/*  Beispiel aus Wagner / Towsley: "Getting Started With POSIX Threads"
     
      3 Threads laufen unsynchronisiert parallel ab. 
      Thread 0:  Haupt-Thread, erzeugt zwei Threads, beendet Prozess 
                 und damit die beiden anderen Threads  
      Thread 1:  gibt "Hello" aus  
      Thread 2:  gibt "World" aus
      Die Reihenfolge der Abarbeitung der Threads ist nicht vorhersagbar. 
      Moegliche Ausgaben:
    	Hello World
    	World Hello
    	Hello
    	World
    	keine Ausgabe	
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define pthread_attr_default NULL

void *print_message_function(void *ptr);

int main(void)
{
	pthread_t thread1, thread2;
	char *message1 = "Hello";
	char *message2 = "World\n";

	pthread_create( &thread1, pthread_attr_default, 
				print_message_function, (void *) message1);

	pthread_create( &thread2, pthread_attr_default, 
				print_message_function, (void *) message2);

	sleep(1);	/* give threads a little bit time to work */
   	return EXIT_SUCCESS;
}

void *print_message_function(void *ptr)
{
	char *message;
	message = (char *) ptr;
	printf(" %s ", message);
	return NULL;
}
