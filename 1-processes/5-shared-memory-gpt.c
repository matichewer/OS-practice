#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int shm_id;
    int *shared_mem;
    pid_t pid;

    // Crear un segmento de memoria compartida
    shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget");
        exit(1);
    }

    // Conectar el proceso padre a la memoria compartida
    shared_mem = (int *)shmat(shm_id, NULL, 0);
    if (shared_mem == (int *)-1) {
        perror("shmat");
        exit(1);
    }

    // Inicializar el valor compartido
    *shared_mem = 0;

    // Crear un proceso hijo
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {  // Proceso hijo
        printf("Proceso hijo: valor inicial %d\n", *shared_mem);
        *shared_mem = 42;  // Cambiar el valor en memoria compartida
        printf("Proceso hijo: valor actualizado %d\n", *shared_mem);
        shmdt(shared_mem);  // Desconectar el hijo de la memoria compartida
        exit(0);
    } else {  // Proceso padre
        wait(NULL);  // Esperar a que el hijo termine
        printf("Proceso padre: valor final %d\n", *shared_mem);
        shmdt(shared_mem);  // Desconectar el padre de la memoria compartida
    
        // Liberar la memoria compartida
        shmctl(shm_id, IPC_RMID, NULL);
    }

    return 0;
}
