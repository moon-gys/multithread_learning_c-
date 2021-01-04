/*!
Threads operation include thread creation, termination,synchronization(joins, blocking),
scheduling, data management and process interaction
*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define NUM_THREADS 10
#define COUNT_DONE 10
#define COUNT_HALT1 3
#define COUNT_HALT2 6
/* example 1: threads create and terminate*/
void *print_message_function(void *ptr);

/* Note scope of variable and mutex are the same*/
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t condition_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_cond = PTHREAD_COND_INITIALIZER;
int counter = 0;

/*example 2: synchronization -- Mutexes*/
void *print_function_mutex(void *);

/*example 2 -- Joins*/
void *print_function_join(void *);

/*example 2 -- condition variables*/
void *function_count1(void*);
void *function_count2(void *);


int main()
{
    pthread_t thread1, thread2;
    int iret1, iret2;

    pthread_t threads[NUM_THREADS] = {0};
    int i, j;

    /*  
    // example 1: threads create and terminate
    char* message1 = "Thread 1";
    char* message2 = "Thread 2";
    
    //Create independent thread each of which will execute print_message_function
    iret1 = pthread_create(&thread1, NULL, print_message_function, (void*) message1);
    iret2 = pthread_create(&thread2, NULL, print_message_function, (void*) message2);
    */
    

     /*! The thread library provides three synchronization mechanisms:
    -- mutexes: 
        --Mutual exclusion lock: Block access to variables by other threads. This enforces exclusive access by a thread to a variable or set of variales.

    -- joins
        -- Make a thread wait till others are complete(terminated)

    --condition variables
        -- data tyr pthread_cond_t
    */

    /*
    //example 2 -- Mutexes: Create independent threads each of will execute print_function_synchronization
    if( (iret1 = pthread_create(&thread1, NULL, print_function_mutex, (void *)NULL)) )
        printf("Thread creation failed: %d\n", iret1);

    if( (iret2 = pthread_create(&thread2, NULL, print_function_mutex, (void *)NULL)) )
        printf("Thread creation failed: %d\n", iret2);
    */

    /*
    //example 2--Joins: A join is performed when one wants to wait for a thread to finish.
    for(i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, print_function_join, (void*)NULL);
    }

    for(j = 0; j < NUM_THREADS; j++)
    {
        pthread_join(threads[j], NULL);
    }
    */

    //example 2--Condition variables
    pthread_create(&thread1, NULL, function_count1, (void*)NULL);
    pthread_create(&thread2, NULL, function_count2, (void*)NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    

    // printf("Thread1 returns: %d\n", iret1);
    // printf("Thread2 returns: %d\n", iret2);
    //printf("Final counter value: %d\n", counter);
   
    exit(0);

    return 0;
}

// example 1: threads create and terminate
void *print_message_function(void* ptr)
{
    const char *message;
    message = (char*)ptr;

    printf("%s\n", message);
};

//example 2: mutex
void *print_function_mutex( void *)
{
    pthread_mutex_lock(&mutex1);
    counter++;
    printf("Counter value : %d\n", counter);
    pthread_mutex_unlock(&mutex1);
};

//example 2: join
void *print_function_join(void *)
{
    printf("Thread number %ld\n", pthread_self());
    pthread_mutex_lock(&mutex1);
    counter++;
    pthread_mutex_unlock(&mutex1);
};

//example 2: condition variable
void *function_count1(void *)
{
    for(;;)
    {
        pthread_mutex_lock(&condition_mutex);
        while(counter >= COUNT_HALT1 && counter <= COUNT_HALT2)
        {
            pthread_cond_wait(&condition_cond, &condition_mutex);
        }
        pthread_mutex_unlock(&condition_mutex);

        pthread_mutex_lock(&count_mutex);
        counter++;
        printf("Counter value functionCount1: %d\n", counter);
        pthread_mutex_unlock(&count_mutex);

        if(counter >= COUNT_DONE)
            return NULL;
    }
};

void *function_count2(void *)
{
    for(;;)
    {
        pthread_mutex_lock(&condition_mutex);
        while(counter >= COUNT_HALT1 || counter <= COUNT_HALT2)
        {
            pthread_cond_signal(&condition_cond);
        }
        pthread_mutex_unlock(&condition_mutex);

        pthread_mutex_lock(&count_mutex);
        counter++;
        printf("Counter value functionCount1: %d\n", counter);
        pthread_mutex_unlock(&count_mutex);

        if(counter >= COUNT_DONE)
            return NULL;
    }
};

