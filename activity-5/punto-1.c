#include <stdio.h> // para printf
#include <pthread.h> // para hilos
#include <semaphore.h> // para semaforos
#include <unistd.h> // para sleep


sem_t semA, semB, semC;

// ABABABACABABABAC...


/*
void*: El tipo de retorno de la función es void*.
       Esto significa que la función devuelve un puntero genérico (void*), 
       que puede apuntar a cualquier tipo de dato.
       Esta es una convención para las funciones que serán ejecutadas por hilos en C.

void* arg:  El argumento de la función es un puntero genérico (void*). 
            Esto permite pasar cualquier tipo de dato al hilo cuando se crea. 
            En este caso, arg no se usa dentro de la función, pero está presente 
            para cumplir con la convención de funciones que se ejecutan en hilos.
*/
void* tareaA(void* arg) {
    while(1){
        sem_wait(&semA); // Espera a que el semáforo A esté disponible
        printf("A");
        fflush(stdout);
        sem_post(&semC); // Libera el semáforo C
        sleep(1);
    }
    return NULL;
}

void* tareaB(void* arg) {
    while(1){
        sem_wait(&semB); // Espera a que el semáforo B esté disponible
        printf("B");
        fflush(stdout);
        sem_post(&semA); // Libera el semáforo A
        sleep(1);
    }
    return NULL;
}

void* tareaC(void* arg) {
    while(1){
        sem_wait(&semC); 
        sem_post(&semB); 
        sem_wait(&semC); 
        sem_post(&semB); 
        sem_wait(&semC); 
        sem_post(&semB); 
        sem_wait(&semC);

        printf("C\n");
        fflush(stdout);

        sem_post(&semA);
        sleep(1);
    }
    return NULL;
}


int main(int argc, char* argv[]) {
    pthread_t t1, t2, t3;

    /*
    1er argumento: &semA es la referencia al semáforo semA que controlará la sincronización de la tarea tareaA.
    2do argumento: (0) indica que el semáforo es local para los hilos en el mismo proceso. (Normalmente es asi)
    3er argumento: (1) es el valor inicial del semáforo, es un entero no signado. 
                        Aquí se inicializa a 1, lo que significa que el semáforo A está "abierto" o 
                        disponible desde el inicio. 
                        Esto permite que el hilo tareaA comience imprimiendo la letra A al inicio.
    */
    sem_init(&semA, 0, 1); // inicia en 1
    sem_init(&semB, 0, 0); // inicia en 0
    sem_init(&semC, 0, 0); // inicia en 0

    pthread_create(&t1, NULL, tareaA, NULL); // Crea el hilo t1 que ejecutará tareaA
    pthread_create(&t2, NULL, tareaB, NULL); // Crea el hilo t2 que ejecutará tareaB
    pthread_create(&t3, NULL, tareaC, NULL); // Crea el hilo t3 que ejecutará tareaC

    pthread_join(t1, NULL); // Espera a que el hilo t1 termine
    pthread_join(t2, NULL); // Espera a que el hilo t2 termine
    pthread_join(t3, NULL); // Espera a que el hilo t3 termine

    // Destruye los semáforos una vez que no se necesiten más (en este caso nunca se alcanza)
    sem_destroy(&semA);
    sem_destroy(&semB);
    sem_destroy(&semC);

    return 0;
}