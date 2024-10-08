#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
 
void* routine() {
    printf("Hello from thread\n");
    printf("All threads are in the same process. Proccess ID: %d\n", getpid());
    sleep(3);
    printf("Bye from thread\n");
    return NULL;
}

int main (int argc, char **argv) {
    pthread_t t1, t2;

    // creamos el hilo
    // los parametros son el hilo, los atributos del hilo, la funcion y los parametros de la funcion
    if (pthread_create(&t1, NULL, &routine, NULL) != 0) {
        printf("Error al crear el hilo\n");
        return 1;
    }

    if (pthread_create(&t2, NULL, &routine, NULL) != 0) {
        printf("Error al crear el hilo\n");
        return 1;
    }

    // esperamos a que el hilo termine
    if (pthread_join(t1, NULL) != 0) {
        printf("Error al hacer join del hilo\n");
        return 1;
    }

    if (pthread_join(t2, NULL) != 0) {
        printf("Error al hacer join del hilo\n");
        return 1;
    }

    return 0;
}
