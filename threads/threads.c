#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int glob = 0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; // new data type and last t means time



static void* thread_function(void * args)//address of our loops variabel
{
    int loops = *((int *) args);//args is the argument which we happen to know is the address of loops. void pointer to integer pointer. double pointer is dereference so we get back to value
    int ix, ret;
    for(ix = 0; ix<loops;++ix)
    {
        ret = pthread_mutex_lock(&mtx);

        ++glob;

        ret = pthread_mutex_unlock(&mtx);

    }
    return NULL;
}


int main(int argc, char* argv[])
{
    pthread_t t1, t2;
    int loops, ret;
    loops = (argc>1) ? atoi(argv[1]) : 100000000; //at higher values it gets damaged depending on how powerful your computer is


    
    ret = pthread_create(&t1, NULL, thread_function, &loops);
    ret = pthread_create(&t2, NULL, thread_function, &loops);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("total = %d\n", glob);


    return 0;
}