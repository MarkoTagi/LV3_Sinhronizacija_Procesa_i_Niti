#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ready = PTHREAD_COND_INITIALIZER;
int count = 0;

void* generatingThread(void*);

int main() {
    int buffer[2];
    pthread_t generatingThread_id;
    pthread_create(&generatingThread_id, NULL, generatingThread, buffer);
    while (1) {
        pthread_mutex_lock(&lock);
        // printf("Waiting...\n");
        while (count == 0) pthread_cond_wait(&ready, &lock);
        int sum = buffer[0] + buffer[1];
        printf("Sum of [ %d, %d ]: %d.\n", buffer[0], buffer[1], sum);
        fflush(stdout);
        count = 0;
        pthread_cond_signal(&ready);
        pthread_mutex_unlock(&lock);
    }
    return 0;
}

void* generatingThread(void* arg) {
    int* buffer = (int*)arg;
    while (1) {
        pthread_mutex_lock(&lock);
        while (count == 1) pthread_cond_wait(&ready, &lock);
        srand(time(0));
        // printf("Generating...\n");
        buffer[0] = (rand() % 10) + 1;
        sleep(rand() % 6);
        buffer[1] = (rand() % 10) + 1;
        sleep(rand() % 6);
        printf("Generated: [ %d, %d ]\n", buffer[0], buffer[1]);
        count = 1;
        pthread_cond_signal(&ready);
        pthread_mutex_unlock(&lock);
    }
}