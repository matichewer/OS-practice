#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int x = 2; // Variable compartida entre hilos

void* routine_A(void* arg) {
    sleep(2);
    x++; // esto modifica la variable global y afecta a todos los hilos
    printf("Value of x in routine A: %d\n", x);
    return NULL; 
}

void* routine_B(void* arg) {
    printf("Value of x in routine B: %d\n", x); 
    sleep(4);
    printf("Value of x in routine B: %d\n", x); 
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t t1, t2; // Declarar los identificadores de los hilos

    // Crear el primer hilo
    if (pthread_create(&t1, NULL, routine_A, NULL)) {
        fprintf(stderr, "Error creating thread 1\n");
        return 1;
    }

    // Crear el segundo hilo
    if (pthread_create(&t2, NULL, routine_B, NULL)) {
        fprintf(stderr, "Error creating thread 2\n");
        return 1;
    }

    // Esperar a que ambos hilos terminen
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0; // Salir del programa
}
