#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Para usleep()

/* RESUMEN:
int pthread_mutex_trylock(pthread_mutex_t *mutex): -> puede retornar lo siguiente:
0: Éxito. El hilo logró bloquear el mutex y entrar en la sección crítica.
EBUSY: El mutex ya está bloqueado por otro hilo (o por el mismo hilo en caso de un mutex no reentrante).
EINVAL: El mutex no está correctamente inicializado.
*/

// Declaración del mutex y la variable compartida
pthread_mutex_t mutex;
int shared_variable = 0;

// Función del hilo que intenta sumar 1 usando try-lock
void* increment(void* arg) {
    for (int i = 0; i < 10; i++) {
        if (pthread_mutex_trylock(&mutex) == 0) {
            // Si se pudo bloquear el mutex, se incrementa la variable
            shared_variable++;
            printf("Hilo 1: Incrementó a %d\n", shared_variable);
            pthread_mutex_unlock(&mutex);  // Desbloquear el mutex
        } else {
            printf("Hilo 1: No pudo bloquear, reintentando...\n");
            usleep(100000);  // Espera 100ms antes de intentar de nuevo
        }
    }
    pthread_exit(NULL);
}

// Función del hilo que intenta restar 1 usando un enfoque wait-try
void* decrement(void* arg) {
    int reintentos = 0;
    while (reintentos < 10) {
        if (pthread_mutex_trylock(&mutex) == 0) {
            // Si se pudo bloquear el mutex, se decrementa la variable
            shared_variable--;
            printf("Hilo 2: Decrementó a %d\n", shared_variable);
            pthread_mutex_unlock(&mutex);  // Desbloquear el mutex
            break;  // Sale del bucle al completar la operación
        } else {
            printf("Hilo 2: Intento %d fallido, reintentando...\n", reintentos + 1);
            reintentos++;
            usleep(100000);  // Espera 100ms antes de intentar de nuevo
        }
    }
    if (reintentos == 10) {
        printf("Hilo 2: No pudo realizar la operación después de varios intentos.\n");
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
