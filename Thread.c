/*
    Threads 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// static function 
void *func(void *var)
{
    sleep(2);
    printf("We are using threads \n");
    return NULL; // NULL is 0 for pointers so instead of 0 use a NULL.
}

int main()
{
    pthread_t t_id; // thread id is being stored

    printf("Before using threading \n");
    pthread_create(&t_id, NULL, func, NULL); 
    pthread_join(t_id, NULL); // wait function 

    printf("After thread \n");

    exit (0);

}
