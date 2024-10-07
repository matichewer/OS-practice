#include <stdio.h> // sirve para usar printf()
#include <stdlib.h>
#include <unistd.h> // sirve para usar pipe() y fork()
#include <sys/types.h> // sirve para definir tipos de datos como pid_t


int main(int argc, char *argv[]) {
    // fd[0] es el file descriptor para leer
    // fd[1] es el file descriptor para escribir
    int fd[2]; // file descriptor

    if (pipe(fd) == -1) { // pipe() crea un pipe y devuelve dos file descriptors
        printf("Error al crear el pipe\n");
        return 1;
    }

    pid_t id = fork();
    if (id == 0) {
        close(fd[0]); // esto es para cerrar el file descriptor de lectura
        int x;
        printf("Ingrese un número: ");
        scanf("%d", &x);

        // esto es para escribir en el pipe el número ingresado
        if (write(fd[1], &x, sizeof(int)) == -1) {
            perror("Error al escribir en el pipe");
            exit(1);
        }

        close(fd[1]); // esto es para cerrar el file descriptor de escritura
    } else {
        close(fd[1]); // esto es para cerrar el file descriptor de escritura
        int y;
        // esto es para leer del pipe el número ingresado
        if (read(fd[0], &y, sizeof(int)) == -1) {
            perror("Error al leer del pipe");
            exit(1);
        }
        printf("Mi hijo ingreso el numero: %d\n", y);
        close(fd[0]); // esto es para cerrar el file descriptor de lectura
    }


    return 0;
}