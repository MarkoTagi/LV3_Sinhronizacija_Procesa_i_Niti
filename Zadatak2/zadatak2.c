#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

sem_t occupiedSlots, vacantSlots;

void* fillBuffer (void* args) {
    srand(time(0));
    while (true) {
        sem_wait(&vacantSlots);
        printf("Filling the buffer...\n");
        int randomNumber = (rand() % 10) + 1;
        int sleepTime = rand() % 6; sleep(sleepTime);
        ((int*)args)[0] = randomNumber;
        printf("After %d seconds thread wrote number %d into the buffer.\n", sleepTime, randomNumber);
        randomNumber = (rand() % 10) + 1;
        sleepTime = rand() % 6; sleep(sleepTime);
        ((int*)args)[1] = randomNumber;
        printf("After %d seconds thread wrote number %d into the buffer.\n", sleepTime, randomNumber);
        sem_post(&occupiedSlots);
    }
}

int main() {
    int buffer[2];
    sem_init(&occupiedSlots, 0, 0);
    sem_init(&vacantSlots, 0, 1);
    pthread_t fillThread;
    pthread_create(&fillThread, NULL, fillBuffer, &buffer);
    while (true) {
        sem_wait(&occupiedSlots);
        printf("The result: %d (number1 = %d, number2 = %d)\n", buffer[0] + buffer[1], buffer[0], buffer[1]);
        sem_post(&vacantSlots);
    }
    sem_destroy(&occupiedSlots);
    sem_destroy(&vacantSlots);
    return 0;
}

