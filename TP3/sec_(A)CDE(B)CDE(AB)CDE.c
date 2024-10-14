#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

//(A) CDE (B)CDE (AB)CDE..
sem_t sem_A, sem_B, sem_C, sem_D, sem_E;

void *letraA(void *args){
    while(1){
        sem_wait(&sem_A);
        sem_wait(&sem_A);
        sleep(1);
        printf("A");
        fflush(stdout);
        sem_post(&sem_B);
        sem_post(&sem_C);
      //  sem_wait(&sem_A);
       // sem_wait(&sem_A);
    }
}
void *letraB(void *args){
    while(1){
        sem_wait(&sem_B);
        sem_wait(&sem_B);
        //sem_post(&sem_A);
        sleep(1);
        printf("B");
        fflush(stdout);
        sem_post(&sem_C);
        sem_post(&sem_C);
        sem_post(&sem_A);
    }
}
void *letraC(void *args){
    while(1){
        sem_wait(&sem_C);
        sleep(1);
        printf("C");
        fflush(stdout);
        sem_post(&sem_D);
        //sem_wait(&sem_C);
    }
}
void *letraD(void *args){
    while(1){
        sem_wait(&sem_D);
        sleep(1);
        printf("D");
        fflush(stdout);
        sem_post(&sem_E);
    }
}

void *letraE(void *args){
    while(1){
        sem_wait(&sem_E);
        sleep(1);
        printf("E");
        fflush(stdout);
        sem_post(&sem_A);
        sem_post(&sem_B);
    }
}

int main()
{
    pthread_t thread_A, thread_B, thread_C, thread_D, thread_E;

    sem_init(&sem_A, 0, 2);
    sem_init(&sem_B, 0, 0);
    sem_init(&sem_C, 0, 0);
    sem_init(&sem_D, 0, 0);
    sem_init(&sem_E, 0, 0);

    pthread_create(&thread_A, NULL, letraA, NULL);
    pthread_create(&thread_B, NULL, letraB, NULL);
    pthread_create(&thread_C, NULL, letraC, NULL);
    pthread_create(&thread_D, NULL, letraD, NULL);
    pthread_create(&thread_E, NULL, letraE, NULL);

    pthread_join(thread_A, NULL);
    pthread_join(thread_B, NULL);
    pthread_join(thread_C, NULL);
    pthread_join(thread_D, NULL);
    pthread_join(thread_E, NULL);

    sem_destroy(&sem_A);
    sem_destroy(&sem_B);
    sem_destroy(&sem_C);
    sem_destroy(&sem_D);
    sem_destroy(&sem_E);

    return 0;
}
