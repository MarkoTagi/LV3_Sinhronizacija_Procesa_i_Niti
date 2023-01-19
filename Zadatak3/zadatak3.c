#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock1, lock2;

void printArray(int* inputArray, int length) {
    printf("The array: [ ");
    for (int i = 0; i < length - 1; ++i) printf("%d, ", inputArray[i]);
    printf("%d ]\n", inputArray[length - 1]);
}

void* insertRandom1(void* args) {
    while (1) {
        pthread_mutex_lock(&lock1);
        printf("Thread 1 is inserting a random number...");
        int randomNumber = rand() % 100; int randomPosition = rand() % 10;
        sleep(2);
        ((int*)args)[randomPosition] = randomNumber;
        printf(" (%d)\n", randomNumber);
        //printArray((int*)args, 20);
        pthread_mutex_unlock(&lock1);
    }
}

void* insertRandom2(void* args) {
    while (1) {
        pthread_mutex_lock(&lock2);
        printf("Thread 2 is inserting a random number...");
        int randomNumber = rand() % 100; int randomPosition = (rand() % 10) + 10;
        sleep(4);
        ((int*)args)[randomPosition] = randomNumber;
        printf(" (%d)\n", randomNumber);
        //printArray((int*)args, 20);
        pthread_mutex_unlock(&lock2);
    }
}

void* insertRandom3(void* args) {
    while (1) {
        pthread_mutex_lock(&lock1);
        pthread_mutex_lock(&lock2);
        printf("Thread 3 is inserting a random number...");
        int randomNumber = rand() % 100; int randomPosition = rand() % 20;
        sleep(4);
        ((int*)args)[randomPosition] = randomNumber;
        printf(" (%d)\n", randomNumber);
        //printArray((int*)args, 20);
        pthread_mutex_unlock(&lock2);
        pthread_mutex_unlock(&lock1);
    }
}

int main() {
    int anArray[20];
    srand(time(0));
    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);
    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, insertRandom1, &anArray);
    pthread_create(&thread2, NULL, insertRandom2, &anArray);
    pthread_create(&thread3, NULL, insertRandom3, &anArray);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return 0;
}
