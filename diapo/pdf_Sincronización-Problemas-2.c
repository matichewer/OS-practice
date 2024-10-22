#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Declaración de semáforos
sem_t semA, semB, semC;

// Función para imprimir 'A'
void *printA(void *arg) {
    while (1) {
        sem_wait(&semA); 

        printf("A");
        fflush(stdout); 
     //   sleep(1);

        sem_post(&semB); 
    }
    return NULL;
}

// Función para imprimir 'B'
void *printB(void *arg) {
    while (1) {
        sem_wait(&semB);
        sem_wait(&semB);

        printf("B");
        fflush(stdout);
    //    sleep(1);

        sem_post(&semC); 
    }
    return NULL;
}

// Función para imprimir 'C'
void *printC(void *arg) {
    while (1) {
        sem_wait(&semC); 

        printf("C\n");
        fflush(stdout);
        sleep(1);

        sem_post(&semA); 
        sem_post(&semA); 
    }
    return NULL;
}

int main() {
    // Declaración de threads
    pthread_t threadA, threadB, threadC, threadD, threadE;

    // Inicialización de semáforos
    sem_init(&semA, 0, 2); // A comienza primero
    sem_init(&semB, 0, 0); // B comienza bloqueado
    sem_init(&semC, 0, 0); // C comienza bloqueado

    // Creación de hilos
    pthread_create(&threadA, NULL, printA, NULL);
    pthread_create(&threadB, NULL, printB, NULL);
    pthread_create(&threadC, NULL, printC, NULL);

    // Espera a que los hilos terminen (esto no ocurrirá en este caso)
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);

    // Destrucción de semáforos (esto no se ejecutará)
    sem_destroy(&semA);
    sem_destroy(&semB);
    sem_destroy(&semC);

    return 0;
}
