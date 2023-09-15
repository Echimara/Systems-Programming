#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NITEMS 10

char shared_buffer[NITEMS];
int shared_count;
pthread_mutex_t mutex;
pthread_cond_t shared_buffer_full;
pthread_cond_t shared_buffer_empty;
unsigned int prod_index = 0;
unsigned int cons_index = 0;

void *producer(void *arg);
void *consumer(void *arg);

int main() {
    pthread_t prod_tid, cons_tid1, cons_tid2;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&shared_buffer_full, NULL);
    pthread_cond_init(&shared_buffer_empty, NULL);
    pthread_create(&prod_tid, NULL, producer, NULL);
    pthread_create(&cons_tid1, NULL, consumer, NULL);
    pthread_create(&cons_tid2, NULL, consumer, NULL);
    pthread_join(prod_tid, NULL);
    pthread_join(cons_tid1, NULL);
    pthread_join(cons_tid2, NULL);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&shared_buffer_full);
    pthread_cond_destroy(&shared_buffer_empty);
    return 0;
}

void *producer(void *arg) {
    char key;
    while (1) {
        // Read and discard any whitespace characters
        while (scanf(" %c", &key) != 1) {
            // Handle input errors if needed
        }

        pthread_mutex_lock(&mutex);
        while (shared_count == NITEMS) {
            pthread_cond_wait(&shared_buffer_full, &mutex);
        }
        shared_buffer[prod_index] = key;
        shared_count++;
        if (prod_index == NITEMS - 1)
            prod_index = 0;
        else
            prod_index++;
        pthread_cond_signal(&shared_buffer_empty);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *consumer(void *arg) {
    printf("Enter text for producer to read and consumer to print, use Ctrl-C to exit.\n\n");
    char key;
    long unsigned int id = (long unsigned int)pthread_self();
    while (1) {
        pthread_mutex_lock(&mutex);
        while (shared_count == 0) {
            pthread_cond_wait(&shared_buffer_empty, &mutex);
        }
        key = shared_buffer[cons_index];
        printf("consumer %lu: %c\n", (long unsigned int)id, key);
        shared_count--;
        if (cons_index == NITEMS - 1)
            cons_index = 0;
        else
            cons_index++;
        pthread_cond_signal(&shared_buffer_full);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
