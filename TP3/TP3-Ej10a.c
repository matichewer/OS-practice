#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Declaración de semáforos
sem_t semA, semB, semC, semD, semE;

// Función para imprimir 'A'
void *printA(void *arg) {
    while (1) {
        sem_wait(&semA); // Espera el semáforo A
        printf("A");
        fflush(stdout); // Asegúrate de que se imprima inmediatamente
        sleep(1);
        sem_post(&semB); // Libera el semáforo B
    }
    return NULL;
}

// Función para imprimir 'B'
void *printB(void *arg) {
    while (1) {
        sem_wait(&semB); // Espera el semáforo B
        printf("B");
        fflush(stdout);
        sleep(1);
        sem_post(&semC); // Libera el semáforo C
    }
    return NULL;
}

// Función para imprimir 'C'
void *printC(void *arg) {
    while (1) {
        sem_wait(&semC); // Espera el semáforo C
        printf("C");
        fflush(stdout);
        sleep(1);
        sem_post(&semD); // Libera el semáforo D
    }
    return NULL;
}

// Función para imprimir 'D'
void *printD(void *arg) {
    while (1) {
        sem_wait(&semD); // Espera el semáforo D
        printf("D");
        fflush(stdout);
        sleep(1);
        sem_post(&semE); // Libera el semáforo E
    }
    return NULL;
}

// Función para imprimir 'E'
void *printE(void *arg) {
    while (1) {
        sem_wait(&semE); // Espera el semáforo E
        printf("E\n");
        fflush(stdout);
        sleep(1);
        sem_post(&semA); // Libera el semáforo A para comenzar de nuevo
    }
    return NULL;
}

int main() {
    // Declaración de threads
    pthread_t threadA, threadB, threadC, threadD, threadE;

    // Inicialización de semáforos
    sem_init(&semA, 0, 1); // A comienza primero
    sem_init(&semB, 0, 0); // B comienza bloqueado
    sem_init(&semC, 0, 0); // C comienza bloqueado
    sem_init(&semD, 0, 0); // D comienza bloqueado
    sem_init(&semE, 0, 0); // E comienza bloqueado

    // Creación de hilos
    pthread_create(&threadA, NULL, printA, NULL);
    pthread_create(&threadB, NULL, printB, NULL);
    pthread_create(&threadC, NULL, printC, NULL);
    pthread_create(&threadD, NULL, printD, NULL);
    pthread_create(&threadE, NULL, printE, NULL);

    // Espera a que los hilos terminen (esto no ocurrirá en este caso)
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);
    pthread_join(threadD, NULL);
    pthread_join(threadE, NULL);

    // Destrucción de semáforos (esto no se ejecutará)
    sem_destroy(&semA);
    sem_destroy(&semB);
    sem_destroy(&semC);
    sem_destroy(&semD);
    sem_destroy(&semE);

    return 0;
}
