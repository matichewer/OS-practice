#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Declaración de semáforos
sem_t semI, semP1, semP2, semP3, semP4, semP5, semP6;

void *printI(void *arg) {
    while (1) {
        sem_wait(&semI);

        printf("I");
        fflush(stdout); 
     //   sleep(1);

        sem_post(&semP1); 
    }
    return NULL;
}

void *printP1(void *arg) {
    while (1) {
        sem_wait(&semP1);

        printf("1");
        fflush(stdout);
     //   sleep(1);

        sem_post(&semP2);
        sem_post(&semP3);
        sem_post(&semP4);
    }
    return NULL;
}

void *printP2(void *arg) {
    while (1) {
        sem_wait(&semP2);

        printf("2");
        fflush(stdout);
    //    sleep(1);

        sem_post(&semP6);
    }
    return NULL;
}

void *printP3(void *arg) {
    while (1) {
        sem_wait(&semP3);
        sem_wait(&semP3);
        sem_wait(&semP3);

        printf("3");
        fflush(stdout);
      //  sleep(1);

        sem_post(&semP6);
    }
    return NULL;
}

void *printP4(void *arg) {
    while (1) {
        sem_wait(&semP4);

        printf("4");
        fflush(stdout);
   //     sleep(1);

        sem_post(&semP3);
        sem_post(&semP5);
    }
    return NULL;
}

void *printP5(void *arg) {
    while (1) {
        sem_wait(&semP5);

        printf("5");
        fflush(stdout);
        //sleep(1);

        sem_post(&semP3);
        sem_post(&semP6);
    }
    return NULL;
}

void *printP6(void *arg) {
    while (1) {
        sem_wait(&semP6);
        sem_wait(&semP6);
        sem_wait(&semP6);

        printf("6\n");
        fflush(stdout);
      //  sleep(1);

        sem_post(&semI);
    }
    return NULL;
}

int main() {
    // Declaración de threads
    pthread_t thread_I, thread_P1, thread_P2, thread_P3, thread_P4, thread_P5, thread_P6;

    // Inicialización de semáforos
    sem_init(&semI, 0, 1); 
    sem_init(&semP1, 0, 0); 
    sem_init(&semP2, 0, 0);
    sem_init(&semP3, 0, 0); 
    sem_init(&semP4, 0, 0); 
    sem_init(&semP5, 0, 0); 
    sem_init(&semP6, 0, 0); 

    // Creación de hilos
    pthread_create(&thread_I, NULL, printI, NULL);
    pthread_create(&thread_P1, NULL, printP1, NULL);
    pthread_create(&thread_P2, NULL, printP2, NULL);
    pthread_create(&thread_P3, NULL, printP3, NULL);
    pthread_create(&thread_P4, NULL, printP4, NULL);
    pthread_create(&thread_P5, NULL, printP5, NULL);
    pthread_create(&thread_P6, NULL, printP6, NULL);

    // Espera a que los hilos terminen (esto no ocurrirá en este caso)
    pthread_join(thread_I, NULL);
    pthread_join(thread_P1, NULL);
    pthread_join(thread_P2, NULL);
    pthread_join(thread_P3, NULL);
    pthread_join(thread_P4, NULL);
    pthread_join(thread_P5, NULL);
    pthread_join(thread_P6, NULL);

    // Destrucción de semáforos (esto no se ejecutará)
    sem_destroy(&semI);
    sem_destroy(&semP1);
    sem_destroy(&semP2);
    sem_destroy(&semP3);
    sem_destroy(&semP4);
    sem_destroy(&semP5);
    sem_destroy(&semP6);

    return 0;
}
