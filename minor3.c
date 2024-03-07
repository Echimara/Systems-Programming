// Due Date: 4/7/2023
// Program Description: "minor3.c" is a program that reads user input keys and echoes them back to the screen using the producer.
// The previous program has been refactored to reduce CPU utilization. Spurious wakeup and pthread condition variables are used to achieve this result.
// To see how this program's CPU% was made efficient, use the "top" command to compare the "%CPU" of the previous code (minor3_BEFORE.c) against that of this refactored code (minor3.c)
// This program should use fewer CPU resources.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NITEMS 10 // number of items in shared buffer

// shared variables
char shared_buffer[NITEMS]; // echo buffer
int shared_count; // item count
pthread_mutex_t mutex; // pthread mutex
pthread_cond_t shared_buffer_full; // handles the shared buffer when full
pthread_cond_t shared_buffer_empty; // handles the shared buffer when empty

unsigned int prod_index = 0; // producer index into shared buffer
unsigned int cons_index = 0; // consumer index into shard buffer

// function prototypes
void *producer(void *arg);
void *consumer(void *arg);

int main() {
    // declare three POSIX variables
    pthread_t prod_tid, cons_tid1, cons_tid2;

    // initialize pthread variables
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&shared_buffer_full, NULL);
    pthread_cond_init(&shared_buffer_empty, NULL);

    // start producer thread
    pthread_create(&prod_tid, NULL, producer, NULL);

    // start consumer threads
    pthread_create(&cons_tid1, NULL, consumer, NULL);
    pthread_create(&cons_tid2, NULL, consumer, NULL);

    // wait for threads to finish
    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid1, NULL);
    pthread_join(cons_tid2, NULL);

    // clean up
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&shared_buffer_full);
    pthread_cond_destroy(&shared_buffer_empty);

    return 0;
}

// producer thread executes this function
void *producer(void *arg) {
    // key - char variable used to log input characters
    char key;
    printf("Enter text for producer to read and consumer to print, use Ctrl-C to exit.\n\n");

    // this loop has the producer read data from stdin and place the read data on the shared buffer
    while (1) {
        // read input key
        scanf("%c", &key);

        // lock the buffer mutex
        pthread_mutex_lock(&mutex);

        // this loop is used to poll the shared buffer to see if it is full
        while (shared_count == NITEMS) {
            pthread_cond_wait(&shared_buffer_full, &mutex); // this cond_wait is called when
        }

        // Add the data to the buffer
        shared_buffer[prod_index] = key;

        // increment prod_index
        shared_count++;

        // update producer index
        if (prod_index == NITEMS - 1)
            prod_index = 0;
        else
            prod_index++;

        // Signal that the buffer is not empty
        pthread_cond_signal(&shared_buffer_empty); // this cond_signal is called when the

        // Release mutex lock
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

// consumer thread executes this function
void *consumer(void *arg) {
    char key;
    long unsigned int id = (long unsigned int)pthread_self();

    // this loop has the consumer get data from the shared buffer and print to stdout
    while (1) {
        // lock the buffer mutex
        pthread_mutex_lock(&mutex);

        // this loop is used to poll the shared buffer to see if it is empty
        while (shared_count == 0) {
            pthread_cond_wait(&shared_buffer_empty, &mutex); // this cond_wait is called
        }

        // read key from shared buffer
        key = shared_buffer[cons_index];

        // echo key
        printf("consumer %lu: %c\n", (long unsigned int)id, key);

        // update shared count variable
        shared_count--;

        // update consumer index
        if (cons_index == NITEMS - 1)
            cons_index = 0;
        else
            cons_index++;

        // Signal that the buffer is not empty
        pthread_cond_signal(&shared_buffer_full); // this cond_signal is called when the condition

        // release mutex lock
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}
