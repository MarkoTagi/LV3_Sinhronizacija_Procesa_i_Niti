/*Pristup bazi podataka obavlja se radi upisa i čitanja od strane više procesa. U jednom trenutku može
postojati više procesa koji čitaju sadržaj iz baze podataka procedurom read_database(), ali ako jedan
proces upisuje sadržaj u bazu podataka procedurom write_database(), nijednom drugom procesu nije
dozvoljen pristup bazi podataka radi upisa i čitanja. Prednost imaju procesi koji čitaju sadržaj, tako da
dok god ima procesa koji čitaju iz baze podataka, proces koji treba da upisuje podatke mora da čeka.
Korišćenjem programskog jezika C napisati Linux program koji korišćenjem procesa i poznatih IPC
mehanizama simulira prethodno opisani algoritam. (Sinhronizacion problem Čitaoci – pisci).*/

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define DATABASE_SIZE 1000

void* readDatabase (void* arg) {
    char* database = (char*)arg;
    int inputLength = 0;
    while (database[i] != '\0') +inputLength;
}

void* writeDatabase (void* arg) {
    char* database = (char*)arg;
    printf("[Writing Thread]\n\tEnter something:");
    scanf("%s", &database);
}

int main() {
    char database[DATABASE_SIZE];
    pthread_t writingThread_ID, readingThread_ID;
    pthread_create(&writingThread_ID, NULL, writeDatabase, &database);
    pthread_create(&readingThread_ID, NULL, readDatabase, &database);
    return 0;
}
