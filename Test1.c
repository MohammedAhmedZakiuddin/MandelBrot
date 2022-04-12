#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * entry_thread(void * value)
{
    printf("Hello from the second thread \n");

    int * num = (int * )value;
    printf("The value is %d\n", *num);

    return NULL;

}

int main(int argc, char ** argv)
{
    pthread_t thread_id;
    
    printf("Hello from the first thread \n");

    int num = 123;  

    pthread_create(&thread_id, NULL, entry_thread, &num);

    pthread_join(thread_id, NULL);

    return EXIT_SUCCESS;

}
