#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct IntArray {
    int size, itemCount;
    int* array;
} IntArray;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty3 = PTHREAD_COND_INITIALIZER;

void* thread_producer(void*);
void* thread_consumer1(void*);
void* thread_consumer2(void*);
void* thread_consumer3(void*);

int main() {
    int size1, size2, size3;
    printf("Enter the size of the 1st buffer: ");
    scanf("%d", &size1);
    IntArray buffer1; buffer1.size = size1; buffer1.itemCount = 0; buffer1.array = malloc(size1 * sizeof(int));
    printf("Enter the size of the 2nd buffer: ");
    scanf("%d", &size2);
    IntArray buffer2; buffer2.size = size2; buffer2.itemCount = 0; buffer2.array = malloc(size2 * sizeof(int));
    printf("Enter the size of the 3rd buffer: ");
    scanf("%d", &size3);
    IntArray buffer3; buffer3.size = size3; buffer3.itemCount = 0; buffer3.array = malloc(size3 * sizeof(int));
    IntArray* buffers[3] = {&buffer1, &buffer2, &buffer3};
    pthread_t id_producer, id_consumer1, id_consumer2, id_consumer3;
    pthread_create(&id_producer, NULL, thread_producer, buffers);
    pthread_create(&id_consumer1, NULL, thread_consumer1, &buffer1);
    pthread_create(&id_consumer2, NULL, thread_consumer2, &buffer2);
    pthread_create(&id_consumer3, NULL, thread_consumer3, &buffer3);
    pthread_join(id_producer, NULL);
    free(buffer1.array);
    free(buffer2.array);
    free(buffer3.array);
    return 0;
}

void* thread_producer(void* arg) {
    IntArray** buffers = (IntArray**)arg;
    IntArray* buffer1 = buffers[0];
    IntArray* buffer2 = buffers[1];
    IntArray* buffer3 = buffers[2];
    srand(time(0));
    while (1) {
        pthread_mutex_lock(&lock);
        while (buffer1->itemCount == buffer1->size && buffer2->itemCount == buffer2->size && buffer3->itemCount == buffer3->size) pthread_cond_wait(&full, &lock);
        int randomNumber = rand() % 25 + 1;
        if (buffer3->itemCount != buffer3->size) {
            printf("3> Generating [%d]...\n", randomNumber);
            buffer3->array[buffer3->itemCount] = randomNumber;
            ++(buffer3->itemCount);
            pthread_cond_signal(&empty3);
        }
        else if (buffer1->itemCount != buffer1->size) {
            printf("1> Generating [%d]...\n", randomNumber);
            buffer1->array[buffer1->itemCount] = randomNumber;
            ++(buffer1->itemCount);
            pthread_cond_signal(&empty1);
        }
        else {
            printf("2> Generating [%d]...\n", randomNumber);
            buffer2->array[buffer2->itemCount] = randomNumber;
            ++(buffer2->itemCount);
            pthread_cond_signal(&empty2);
        }
        sleep(rand() % 5 + 1);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* thread_consumer1(void* arg) {
    IntArray* buffer1 = (IntArray*)arg;
    while(1) {
        pthread_mutex_lock(&lock);
        if (buffer1->itemCount == 0) printf("1> EMPTY!\n");
        while (buffer1->itemCount == 0) pthread_cond_wait(&empty1, &lock);
        printf("1> Reading [%d].\n", buffer1->array[--(buffer1->itemCount)]);
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&lock);
    }
    sleep(rand() % 5 + 1);
    return NULL;
}

void* thread_consumer2(void* arg) {
    IntArray* buffer2 = (IntArray*)arg;
    while(1) {
        pthread_mutex_lock(&lock);
        if (buffer2->itemCount == 0) printf("2> EMPTY!\n");
        while (buffer2->itemCount == 0) pthread_cond_wait(&empty2, &lock);
        printf("2> Reading [%d].\n", buffer2->array[--(buffer2->itemCount)]);
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&lock);
    }
    sleep(rand() % 5 + 1);
    return NULL;
}

void* thread_consumer3(void* arg) {
    IntArray* buffer3 = (IntArray*)arg;
    while (1){
        pthread_mutex_lock(&lock);
        if (buffer3->itemCount == 0) printf("3> EMPTY!\n");
        while (buffer3->itemCount == 0) pthread_cond_wait(&empty3, &lock);
        printf("3> Reading [%d].\n", buffer3->array[--(buffer3->itemCount)]);
        pthread_cond_signal(&full);
        pthread_mutex_unlock(&lock);
    }
    sleep(rand() % 5 + 1);
    return NULL;
}