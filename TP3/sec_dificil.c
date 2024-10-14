#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// Secuencia: 
//ACDE BCDE ABCDE
sem_t semA, semB, semC, semD;

void* tareaA(void* arg) {
    while(1) {
        sem_wait(&semA);
        
        // Código Arriba
        printf("A");
        fflush(stdout);
        usleep(10000);
        // Código Abajo
        sem_post(&semB);
 

    }
}

void* tareaB(void* arg) {
    while(1) {
        sem_wait(&semB);

        // Código Arriba
        printf("B");
        fflush(stdout);
        usleep(10000);
        // Código Abajo
       
        sem_post(&semC);

    }
}

void* tareaC(void* arg) {
    while(1) {
        sem_wait(&semC);
        sem_post(&semD);
        sem_wait(&semC);
        // Código Arriba
        printf("C");
        fflush(stdout);
        usleep(10000);
        // Código Abajo
        sem_post(&semD);

    }
}

void* tareaD(void* arg) {
    while(1) {

        sem_wait(&semD);
        sem_post(&semA);
        sem_wait(&semD);
        sem_post(&semA);
        sem_post(&semC);
        sem_wait(&semD);
        sem_wait(&semD);
        // Código Arriba
        printf("D");
        fflush(stdout);
        usleep(10000);
        // Código Abajo
        sem_post(&semA);
    }
}




int main() {
    pthread_t threadA, threadB, threadC, threadD, threadE;

    sem_init(&semA, 0, 1);
    sem_init(&semB, 0, 0);
    sem_init(&semC, 0, 0);
    sem_init(&semD, 0, 0);
    


    pthread_create(&threadA, NULL, tareaA, NULL);
    pthread_create(&threadB, NULL, tareaB, NULL);
    pthread_create(&threadC, NULL, tareaC, NULL);
    pthread_create(&threadD, NULL, tareaD, NULL);
    
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);
    pthread_join(threadD, NULL);
   
    sem_destroy(&semA);
    sem_destroy(&semB);
    sem_destroy(&semC);
    sem_destroy(&semD);
    
    return 0;
}