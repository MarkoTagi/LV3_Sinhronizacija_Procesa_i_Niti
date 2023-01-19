#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct IntArray {
    int* theArray;
    int arrayLength;
} typedef IntArray;

void printArray(int inputArray[], int arrayLength) {
    printf("[ ");
    for (int i = 0; i < (arrayLength - 1); ++i) printf("%d, ", inputArray[i]);
    printf("%d ]\n", inputArray[arrayLength - 1]);
}

void* sortingThread1(void* arg) {
    int* inputArray = ((IntArray*)arg)->theArray;
    int arrayLength = ((IntArray*)arg)->arrayLength;
    //pthread_mutex_lock(&lock);
    //printf("Sorting_Thread1> Sorting (ascending)...\n");
    for (int i = 0; i < (arrayLength / 2) - 1; ++i) {
        int minIndex = i;
        for (int j = i + 1; j < (arrayLength / 2); ++j) minIndex = (inputArray[j] < inputArray[minIndex]) ? j : minIndex;
        if (minIndex != i) {
            int tmp_i = inputArray[i];
            inputArray[i] = inputArray[minIndex];
            inputArray[minIndex] = tmp_i;
        }
    }
    //pthread_mutex_unlock(&lock);
    //printf("Sorting_Thread1> Sorting (ascending) finished...\n");
}

void* sortingThread2(void* arg) {
    int* inputArray = ((IntArray*)arg)->theArray;
    int arrayLength = ((IntArray*)arg)->arrayLength;
    //pthread_mutex_lock(&lock);
    //printf("Sorting_Thread2> Sorting (descending)...\n");
    for (int i = arrayLength / 2; i < arrayLength - 1; ++i) {
        int maxIndex = i;
        for (int j = i + 1; j < arrayLength; ++j) maxIndex = (inputArray[j] > inputArray[maxIndex]) ? j : maxIndex;
        if (maxIndex != i) {
            int tmp_i = inputArray[i];
            inputArray[i] = inputArray[maxIndex];
            inputArray[maxIndex] = tmp_i;
        }
    }
    //printf("Sorting_Thread2> Sorting (descending) finished...\n");
    //pthread_mutex_unlock(&lock);
}

int main() {
    int arrayLength;
    do {
        printf("Main> Enter an integer: ");
        scanf("%d", &arrayLength);
        if (arrayLength <= 0) printf("You have to enter a positive integer!\n");
    } while (arrayLength <= 0);
    int* array = malloc(arrayLength * sizeof(int));
    srand(time(0));
    for (int i = 0; i < arrayLength; ++i) array[i] = rand() % 11;
    IntArray inputArray;
    inputArray.theArray = array;
    inputArray.arrayLength = arrayLength;
    printf("Main> Generated array: ");
    printArray(inputArray.theArray, inputArray.arrayLength);
    pthread_t sortingThread1_id, sortingThread2_id;
    pthread_create(&sortingThread1_id, NULL, sortingThread1, &inputArray);
    pthread_create(&sortingThread2_id, NULL, sortingThread2, &inputArray);
    pthread_join(sortingThread1_id, NULL);
    pthread_join(sortingThread2_id, NULL);
    printf("Main> Sorted array: ");
    printArray(inputArray.theArray, inputArray.arrayLength);
    //pthread_mutex_destroy(&lock);
    free(array);
    printf("Main> Exiting...\n");
    return 0;
}