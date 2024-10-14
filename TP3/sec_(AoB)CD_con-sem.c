#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


//(AoB)CD (AoB)CD ...


// Declaración de semáforos
sem_t semAoB, semC, semD;

// Función para imprimir 'A'
void *printA(void *arg) {
    while (1) {
        sem_wait(&semAoB); // Espera el semáforo A
        printf("A");
        fflush(stdout); // Asegúrate de que se imprima inmediatamente
        sleep(1);
        sem_post(&semC); // Libera el semáforo B
    }
    return NULL;
}

// Función para imprimir 'B'
void *printB(void *arg) {
    while (1) {
        sem_wait(&semAoB); // Espera el semáforo B
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
        printf("D\n");
        fflush(stdout);
        sleep(1);
        sem_post(&semAoB); // Libera el semáforo E
    }
    return NULL;
}


int main() {
    // Declaración de threads
    pthread_t threadA, threadB, threadC, threadD;

    // Inicialización de semáforos
    sem_init(&semAoB, 0, 1); // A comienza primero
    sem_init(&semC, 0, 0); // C comienza bloqueado
    sem_init(&semD, 0, 0); // D comienza bloqueado

    // Creación de hilos
    pthread_create(&threadA, NULL, printA, NULL);
    pthread_create(&threadB, NULL, printB, NULL);
    pthread_create(&threadC, NULL, printC, NULL);
    pthread_create(&threadD, NULL, printD, NULL);

    // Espera a que los hilos terminen (esto no ocurrirá en este caso)
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);
    pthread_join(threadD, NULL);

    // Destrucción de semáforos (esto no se ejecutará)
    sem_destroy(&semAoB);
    sem_destroy(&semC);
    sem_destroy(&semD);

    return 0;
}
