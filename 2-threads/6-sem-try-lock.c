#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Para usleep()


/* RESUMEN:
int sem_trywait(sem_t *sem) -> puede retornar lo siguiente:
0: Éxito. El hilo logró decrementar el valor del semáforo y entrar en la sección crítica.
-1: Error. El semáforo ya está bloqueado (valor 0), por lo que no pudo acceder a la sección crítica.
    En este caso, errno se establece con uno de los siguientes valores:
        EAGAIN: El semáforo tiene valor 0 (no disponible en ese momento).
        EINVAL: El semáforo es inválido o no ha sido inicializado correctamente.
*/

// Declaración del semáforo y la variable compartida
sem_t semaforo;
int shared_variable = 0;

// Función del hilo que intenta sumar 1 usando sem_trywait()
void* increment(void* arg) {
    for (int i = 0; i < 10; i++) {
        if (sem_trywait(&semaforo) == 0) {
            // Sección crítica
            shared_variable++;
            printf("Hilo 1: Incrementó a %d\n", shared_variable);
            sem_post(&semaforo);  // Liberar el semáforo
        } else {
            printf("Hilo 1: No pudo acceder, reintentando...\n");
            usleep(100000);  // Esperar 100ms antes de reintentar
        }
    }
    pthread_exit(NULL);
}

// Función del hilo que intenta restar 1 usando un enfoque de wait-try
void* decrement(void* arg) {
    int reintentos = 0;
    while (reintentos < 10) {
        if (sem_trywait(&semaforo) == 0) {
            // Sección crítica
            shared_variable--;
            printf("Hilo 2: Decrementó a %d\n", shared_variable);
            sem_post(&semaforo);  // Liberar el semáforo
            break;  // Salir del bucle al completar la operación
        } else {
            printf("Hilo 2: Intento %d fallido, reintentando...\n", reintentos + 1);
            reintentos++;
            usleep(100000);  // Esperar 100ms antes de intentar de nuevo
        }
    }
    if (reintentos == 10) {
        printf("Hilo 2: No pudo realizar la operación después de varios intentos.\n");
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread1, thread2;

    // Inicializar el semáforo con valor inicial 1
    if (sem_init(&semaforo, 0, 1) != 0) {
        printf("Error al inicializar el semáforo\n");
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

    // Destruir el semáforo
    sem_destroy(&semaforo);

    return 0;
}
