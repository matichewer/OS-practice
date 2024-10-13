#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 5

// Declaración del mutex global
pthread_mutex_t mutex;

// Variable compartida entre los hilos
int shared_counter = 0;

// Función que será ejecutada por cada hilo
void* increment_counter(void* arg) {
    // Bloquear el mutex para garantizar exclusión mutua
    pthread_mutex_lock(&mutex);

    // Sección crítica: modificar la variable compartida
    shared_counter++;
    printf("Hilo %ld incrementó el contador a %d\n", (long) arg, shared_counter);

    // Desbloquear el mutex para permitir acceso a otros hilos
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int i;

    // Inicializar el mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Error al inicializar el mutex\n");
        return 1;
    }

    // Crear los hilos
    for (i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, (void*) (long) i) != 0) {
            printf("Error al crear el hilo %d\n", i);
            return 1;
        }
    }

    // Esperar a que todos los hilos terminen
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Valor final del contador: %d\n", shared_counter);

    // Destruir el mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
