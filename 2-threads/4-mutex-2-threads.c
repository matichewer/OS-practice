#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// Declaración del mutex y la variable compartida
pthread_mutex_t mutex;
int shared_variable = 0;
int COUNT = 1000000;

// Función del hilo que suma 1 a la variable
void* increment(void* arg) {
    for (int i = 0; i < COUNT; i++) {
        // TIP: probar de descomentar los mutex y observar el resultado
        pthread_mutex_lock(&mutex);  // Bloquear mutex
        shared_variable++;
        pthread_mutex_unlock(&mutex);  // Desbloquear mutex
    }
    pthread_exit(NULL);
}

// Función del hilo que resta 1 a la variable
void* decrement(void* arg) {
    for (int i = 0; i < COUNT; i++) {
        pthread_mutex_lock(&mutex);  // Bloquear mutex
        shared_variable--;
        pthread_mutex_unlock(&mutex);  // Desbloquear mutex
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;

    // Inicializar el mutex
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Error al inicializar el mutex\n");
        return 1;
    }

    // Crear los dos hilos
    if (pthread_create(&thread1, NULL, increment, NULL) != 0) {
        printf("Error al crear el hilo 1\n");
        return 1;
    }
    if (pthread_create(&thread2, NULL, decrement, NULL) != 0) {
        printf("Error al crear el hilo 2\n");
        return 1;
    }

    // Esperar a que los hilos terminen
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Mostrar el valor final de la variable compartida
    printf("Valor final de la variable compartida: %d\n", shared_variable);

    // Destruir el mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
