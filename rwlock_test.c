#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{	
	printf("Greedtings, In this test, we have 3 groups of readers and writers:\n");
	printf("first there comes a group of readers: each reader need  0.5 second to do their job\n");
	printf("second comes a group of writers: they each need one second to do their job\n");
	printf("third comes a group of readers: they each need 0.5 second to do their job\n");
	printf("we could observe that when writers come, no more readers can added and soon \"no readers writer lock is free\" message will appear. \n");
	sleep(1);
	char command[20];
	strcpy(command,"./rwlock 5 1 5");
    	system(command);
    	strcpy(command,"./rwlock 5 2 5");
   	system(command);
  	strcpy(command,"./rwlock 10 1 5");
  	system(command);
  	strcpy(command,"./rwlock 10 2 5");
  	system(command);
  	strcpy(command,"./rwlock 10 5 10");
   	system(command);
	strcpy(command,"./rwlock 10 10 20");
   	system(command);

}

