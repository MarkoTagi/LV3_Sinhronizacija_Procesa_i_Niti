/*Korišćenjem programskog jezika C napisati Linux program koji simulira komunikaciju između niti
korišćenjem celobrojnih bafera. Nit N0 generiše celobrojne podatke i upisuje ih u bafere B1, B2 i B3.
Prilikom upisivanja podataka, nit N0, najpre proverava da li u baferu B3 ima slobodnih pozicija i u tom
slučaju novi podatak upisuje u bafer B3. Ako je bafer B3 pun, proverava da li postoje slobodne pozicije
u baferu B1. Ako postoje, proizvođač N0 generisani podatak upisuje u bafer B1. Ukoliko je i bafer B1
pun novi podatak se upisuje u bafer B2 ukoliko on poseduje slobodne pozicije. Ukoliko ni u jednom
baferu nema slobodnih pozicija, nit N0 čeka dok se u nekom od bafera ne oslobodi pozicija za upis
novog elementa. Niti N1, N2, N3 čitaju podatke iz bafera B1, B2, B3 i prikazuju ih na standardnom
izlazu. Za sinhronizaciju između niti iskoristiti POSIX semafore.*/

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
sem_t readSemaphore1, readSemaphore2, readSemaphore3, test;

struct Buffer {
    int* theBuffer;
    int size, itemCount;
};

void printBuffer(struct Buffer buffer, int id) {
    printf("Buffer %d...\n", id);
    if (buffer.itemCount == 0)
        printf("\tThe buffer: [ ].\n");
    else {
        printf("\tThe buffer: [ ");
        for (int i = 0; i < buffer.itemCount - 1; ++i) printf("%d, ", buffer.theBuffer[i]);
        printf("%d ],\n", buffer.theBuffer[buffer.itemCount - 1]);
    }
    printf("\tBuffer size: %d,\n\tItem count: %d.\n", buffer.size, buffer.itemCount);
}

void initializeBuffer(struct Buffer* aBuffer, int bufferSize) {
    aBuffer->theBuffer = malloc(bufferSize * sizeof(int));
    aBuffer->size = bufferSize;
    aBuffer->itemCount = 0;
}

void* generatingThread (void* arg) {
    struct Buffer* buffer1 = ((struct Buffer**)arg)[0];
    struct Buffer* buffer2 = ((struct Buffer**)arg)[1];
    struct Buffer* buffer3 = ((struct Buffer**)arg)[2];
    while (1) {
        pthread_mutex_lock(&lock);
        printf("IN GENERATING_THREAD...\n");
        int userInput;
        printf("Enter a number...");
        scanf("%d", &userInput);
        int itemCount;
        if (buffer3->itemCount != buffer3->size) {
            itemCount = buffer3->itemCount;
            (buffer3->theBuffer)[itemCount] = userInput;
            ++(buffer3->itemCount);
            sem_post(&readSemaphore3);
        } else if (buffer1->itemCount != buffer1->size) {
            itemCount = buffer1->itemCount;
            (buffer1->theBuffer)[itemCount] = userInput;
            ++(buffer1->itemCount);
            sem_post(&readSemaphore1);
        } else if (buffer2->itemCount != buffer2->size) {
            itemCount = buffer2->itemCount;
            (buffer2->theBuffer)[itemCount] = userInput;
            ++(buffer2->itemCount);
            sem_post(&readSemaphore2);
        } else {
            printf("Could not enter number (%d) - all buffers are currently full...\n", userInput);
            printBuffer(*buffer1, 1);
            printBuffer(*buffer2, 2);
            printBuffer(*buffer3, 3);
        }
        pthread_mutex_unlock(&lock);
    }
}

void* readingThread1 (void* arg) {
    struct Buffer* buffer1 = (struct Buffer*)arg;
    while (1) {
        sem_wait(&readSemaphore1);
        pthread_mutex_lock(&lock);
        printf("IN THREAD_1...\n");
        int itemCount = buffer1->itemCount;
        if (buffer1->itemCount > 0) {
            printBuffer(*buffer1, 1);
            printf("Reading number (%d) from buffer_1...\n", (buffer1->theBuffer)[itemCount - 1]);
            --(buffer1->itemCount);
            printBuffer(*buffer1, 1);
        } else {
            printf("Buffer1 item count: %d.\n", buffer1->itemCount);
        }
        pthread_mutex_unlock(&lock);
    }
}


void* readingThread2 (void* arg) {
    struct Buffer* buffer2 = (struct Buffer*)arg;
    while (1) {
        sem_wait(&readSemaphore2);
        pthread_mutex_lock(&lock);
        printf("IN THREAD_2...\n");
        int itemCount = buffer2->itemCount;
        if (buffer2->itemCount > 0) {
            printBuffer(*buffer2, 2);
            printf("Reading number (%d) from buffer_2...\n", (buffer2->theBuffer)[itemCount - 1]);
            --(buffer2->itemCount);
            printBuffer(*buffer2, 2);
        } else {
            printf("Buffer2 item count: %d.\n", buffer2->itemCount);
        }
        pthread_mutex_unlock(&lock);
    }
}


void* readingThread3 (void* arg) {
    struct Buffer* buffer3 = (struct Buffer*)arg;
    while (1) {
        sem_wait(&readSemaphore3);
        pthread_mutex_lock(&lock);
        printf("IN THREAD_3...\n");
        int itemCount = buffer3->itemCount;
        if (buffer3->itemCount > 0) {
            printBuffer(*buffer3, 3);
            printf("Reading number (%d) from buffer_3...\n", (buffer3->theBuffer)[itemCount - 1]);
            --(buffer3->itemCount);
            printBuffer(*buffer3, 3);
        } else {
            printf("Buffer3 item count: %d.\n", buffer3->itemCount);
        }
        pthread_mutex_unlock(&lock);
    }
}

int main() {
    //Deklaracija i inicijalizacija bafera
    int bufferSize;
    printf("Please enter the size of the 1st buffer: ");
    scanf("%d", &bufferSize);
    struct Buffer buffer1; initializeBuffer(&buffer1, bufferSize);
    printf("Please enter the size of the 2nd buffer: ");
    scanf("%d", &bufferSize);
    struct Buffer buffer2; initializeBuffer(&buffer2, bufferSize);
    printf("Please enter the size of the 3rd buffer: ");
    scanf("%d", &bufferSize);
    struct Buffer buffer3; initializeBuffer(&buffer3, bufferSize);
    //Kraj deklaracije i inicijalizacije bafera

    struct Buffer* theBuffers[3] = {&buffer1, &buffer2, &buffer3}; //Argumenti za thread funkciju generatingThread
    pthread_t thread0_id, thread1_id, thread2_id, thread3_id;
    sem_init(&readSemaphore1, 0, 0); sem_init(&readSemaphore2, 0, 0); sem_init(&readSemaphore3, 0, 0);
    pthread_create(&thread0_id, NULL, generatingThread, theBuffers);
    pthread_create(&thread1_id, NULL, readingThread1, &buffer1);
    pthread_create(&thread2_id, NULL, readingThread2, &buffer2);
    pthread_create(&thread3_id, NULL, readingThread3, &buffer3);
    pthread_join(thread0_id, NULL);
    pthread_join(thread1_id, NULL);
    pthread_join(thread2_id, NULL);
    pthread_join(thread3_id, NULL);

    pthread_mutex_destroy(&lock);
    sem_destroy(&readSemaphore1); sem_destroy(&readSemaphore2); sem_destroy(&readSemaphore3);
    free(buffer1.theBuffer);
    free(buffer2.theBuffer);
    free(buffer3.theBuffer);

    return 0;
}
