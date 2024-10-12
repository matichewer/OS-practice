#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

#define SEGSIZE 1024
#define KEY 1234

struct info {
    char mensaje[100];
};

int main(int argc, char *argv[]) {
    int id;
    struct info *ctrl;

    // Crear el primer proceso (padre)
    pid_t pid = fork();

    if (pid < 0) {
        // Si falla la creación del proceso
        perror("Error en fork");
        exit(1);
    }

    if (pid == 0) {
        // Código del proceso hijo
        // Obtener un identificador de memoria compartida
        id = shmget(KEY, SEGSIZE, 0);
        if (id < 0) {
            printf("Fallo el shmget en proceso hijo\n");
            exit(1);
        }

        // Asociar la memoria compartida a la estructura local
        ctrl = (struct info *)shmat(id, 0, 0);
        if (ctrl == (struct info *)(-1)) {
            printf("Error en el shmat en proceso hijo\n");
            exit(2);
        }

        // Operar sobre memoria compartida
        if (argc == 1) {
            strcpy(ctrl->mensaje, "\0");  // Si no hay argumento, limpiar el mensaje
        } else {
            strcpy(ctrl->mensaje, argv[1]);  // Si hay argumento, copiarlo al mensaje
        }

        // Desasociar memoria compartida
        shmdt((void *) ctrl);

        printf("Proceso hijo: mensaje actualizado a '%s'\n", ctrl->mensaje);
    } else {
        // Código del proceso padre
        // Crear memoria compartida
        id = shmget(KEY, SEGSIZE, IPC_CREAT | 0666);
        if (id < 0) {
            printf("Error, fallo el shmget en proceso padre\n");
            exit(1);
        }

        // Asociar la memoria compartida a la estructura local
        ctrl = (struct info *)shmat(id, 0, 0);
        if (ctrl == (struct info *)(-1)) {
            printf("Error, fallo el shmat en proceso padre\n");
            exit(2);
        }

        // Inicializar la memoria compartida con un valor
        strcpy(ctrl->mensaje, "Sistemas Operativos");
        printf("Proceso padre: mensaje inicial '%s'\n", ctrl->mensaje);

        // Esperar a que el proceso hijo termine
        wait(NULL);

        // Leer el valor actualizado por el hijo
        printf("Proceso padre: mensaje después del proceso hijo '%s'\n", ctrl->mensaje);

        // Desasociar y eliminar la memoria compartida
        shmdt((void *) ctrl);
        shmctl(id, IPC_RMID, NULL);
    }

    return 0;
}
