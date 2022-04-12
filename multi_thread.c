/*
    Threads 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

//global variable
int a = 0;


// static function 
void *func(void *var)
{
    int *id = (int *) var;
    static int b = 0;

    b = 7;
    a = 7;

    printf("The id of the thread is %d\n", *id);
    printf("The value of the static variable is %d\n", b);
    printf("The value of the global variable is %d \n", a);

}

int main()
{
    pthread_t th_id;
    int i;
    for(i = 0;i < 2; i++)
    {
        pthread_create(&th_id, NULL, func, (void *) &th_id); // type cast th_id
    }   

    pthread_exit(NULL);

    return 0;
}
