// Name: Chimara Okeke
// Course Description: 
// This C program showcases mutex lock behavior by converting characters in a user-input sentence to uppercase using multi-threading.
// It initializes a mutex, creates threads for each character, and ensures thread-safe access to the shared 'sentence' array. 
// Upon completion, it destroys the mutex.

// HOW TO RUN:
// -Open up a terminal
// -Ensure that this file is accessible on that terminal
// -Assume filename to be "toUpper.c", compile with "gcc toUpper.c -o toUpper -lpthread"
// -execute file with "./toUpper"

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#define SIZE 50
char sentence[2000];
int ind = 0;

pthread_mutex_t mutex; // Declare mutex lock

char convertUppercase(char lower)
{
    // Converts lowercase to uppercase
    if ((lower > 96) && (lower < 123))
    {
        return (lower - 32);
    }
    else
    {
        return lower;
    }
}

void printChar()
{
    // Prints the converted sentence
    printf("The new sentence is [%d]: \t%c\n", ind, sentence[ind]);
    ind++;
}

void *convertMessage(void *ptr)
{
    // Function that each thread initiates its execution
    pthread_mutex_lock(&mutex); // Lock mutex 
    if (ind % 2)
    {
        sentence[ind] = convertUppercase(sentence[ind]);
    }
    pthread_mutex_unlock(&mutex); // Unlock mutex 
    printChar();
    return 0;
}

int main()
{
    pthread_mutex_init(&mutex, NULL); // Initialize mutex lock
    int i;
    char buffer[SIZE];
    char *p;
    pthread_t ts[SIZE]; // Define up to 50 threads

    printf("Please enter a phrase (less than 50 characters): ");
    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        if ((p = strchr(buffer, '\n')) != NULL)
        {
            *p = '\0';
        }
    }

    strcpy(sentence, buffer); // Copy string to char array
    printf("The original sentence is: \t %s\n", sentence);

    // Create one thread for each character in the input word
    for (i = 0; i < strlen(buffer) + 1; ++i)
    {
        pthread_create(&ts[i], NULL, convertMessage, NULL);
    }

    // Wait until all threads finish execution
    for (i = 0; i < strlen(buffer); i++)
    {
        pthread_join(ts[i], NULL);
    }
    printf("\n");

    pthread_mutex_destroy(&mutex); // Destroy mutex lock
    return 0;
}
