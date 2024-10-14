#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_ITEMS 5  // Máximo número de elementos que el productor puede generar

// Variables compartidas
int buffer = 0;  // Contador de elementos

// Mutex
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
// la anterior linea de codigo es lo mismo que hacer las siguientes 2 lineas:
// pthread_mutex_t mut;
// pthread_mutex_init(&mut, NULL);  // Inicializa el mutex

// Variable de condición
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// la anterior linea de codigo es lo mismo que hacer las siguientes 2 lineas:
// pthread_cond_t cond;
// pthread_cond_init(&cond, NULL);  // Inicializa la variable de condición



// Función del productor
void* productor(void* arg) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        pthread_mutex_lock(&mut);  // Bloquea el mutex
        buffer++;  // Produce un elemento
        printf("Productor: produjo 1 elemento, total = %d\n", buffer);

        pthread_cond_signal(&cond);  // Envía señal al consumidor
        pthread_mutex_unlock(&mut);  // Desbloquea el mutex

        sleep(1);  // Simula tiempo de producción
    }
    return NULL;
}

// Función del consumidor
void* consumidor(void* arg) {
    while (1) {
        pthread_mutex_lock(&mut);  // Bloquea el mutex

        // Espera hasta que haya elementos en el buffer
        while (buffer == 0) {
            printf("Consumidor: esperando...\n");
            pthread_cond_wait(&cond, &mut);  // Espera la señal
        }

        buffer--;  // Consume un elemento
        printf("Consumidor: consumió 1 elemento, total = %d\n", buffer);

        pthread_mutex_unlock(&mut);  // Desbloquea el mutex
        sleep(2);  // Simula tiempo de consumo
    }
    return NULL;
}

int main() {
    pthread_t hilo_productor, hilo_consumidor;

    // Crear hilos
    pthread_create(&hilo_productor, NULL, productor, NULL);
    pthread_create(&hilo_consumidor, NULL, consumidor, NULL);

    // Esperar a que termine el hilo productor
    pthread_join(hilo_productor, NULL);

    // Cancelar al consumidor ya que es un bucle infinito
    pthread_cancel(hilo_consumidor);
    pthread_join(hilo_consumidor, NULL);

    // Destruir el mutex y la variable de condición
    pthread_mutex_destroy(&mut);
    pthread_cond_destroy(&cond);

    return 0;
}
