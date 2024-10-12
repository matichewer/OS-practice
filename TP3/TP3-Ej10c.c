#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semAoB, semC, semD, semE;

void* procesoA(void* arg) {
    while(1) {
        sem_wait(&semAoB); // Espera a que le toque a A
        printf("A");
        fflush(stdout);
       // sleep(1);
        sem_post(&semC); // Permite que C se ejecute después de A
    }
}

void* procesoB(void* arg) {
    while(1) {
        sem_wait(&semAoB); // Espera a que le toque a B
        printf("B");
        fflush(stdout);
      //  sleep(1);
        sem_post(&semC); // Permite que C se ejecute después de B
    }
}

void* procesoC(void* arg) {
    while(1) {
        sem_wait(&semC); // Espera a que A o B hayan liberado C
        printf("C");
        fflush(stdout);
      //  sleep(1);
        sem_post(&semD); // Permite que D se ejecute
    }
}

void* procesoD(void* arg) {
    while(1) {
        sem_wait(&semD); // Espera a C para ejecutar D
        printf("D");
        fflush(stdout);
        //sleep(1);
        sem_post(&semE); // Permite que E se ejecute
    }
}

void* procesoE(void* arg) {
    while(1) {
        sem_wait(&semE); // Espera a D para ejecutar E
        printf("E\n");
        fflush(stdout);
        //sleep(1);
        sem_post(&semAoB); 
    }
}

int main() {
    pthread_t tA, tB, tC, tD, tE;

    // Inicializar semaforos
    sem_init(&semAoB, 0, 1); 
    sem_init(&semC, 0, 0);
    sem_init(&semD, 0, 0); 
    sem_init(&semE, 0, 0); 

    // Crear hilos para cada proceso
    pthread_create(&tA, NULL, procesoA, NULL);
    pthread_create(&tB, NULL, procesoB, NULL);
    pthread_create(&tC, NULL, procesoC, NULL);
    pthread_create(&tD, NULL, procesoD, NULL);
    pthread_create(&tE, NULL, procesoE, NULL);

    // Esperar a que los hilos terminen (en este caso nunca lo harán)
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    pthread_join(tC, NULL);
    pthread_join(tD, NULL);
    pthread_join(tE, NULL);

    // Destruir semáforos
    sem_destroy(&semAoB);
    sem_destroy(&semC);
    sem_destroy(&semD);
    sem_destroy(&semE);

    return 0;
}