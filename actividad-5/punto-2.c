#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#define A 1
#define B 2
#define C 3

typedef struct {
    long mtype;  // Tipo de mensaje
} mensaje;

void procesoA(int cola_id, int reps) {
    mensaje msg;
    for (int i = 0; i < reps * 4; i++) {
        // Esperar mensaje de tipo A
        msgrcv(cola_id, &msg, sizeof(msg) - sizeof(long), A, 0);

        // Imprimir A
        printf("A");
	fflush(stdout);
        // Enviar mensaje de tipo C
        msg.mtype = C;
        msgsnd(cola_id, &msg, sizeof(msg) - sizeof(long), 0);
    }
}

void procesoB(int cola_id, int reps) {
    mensaje msg;
    for (int i = 0; i < reps * 3; i++) {
        // Esperar mensaje de tipo B
        msgrcv(cola_id, &msg, sizeof(msg) - sizeof(long), B, 0);

        // Imprimir B
        printf("B");
	fflush(stdout);
        // Enviar mensaje de tipo A
        msg.mtype = A;
        msgsnd(cola_id, &msg, sizeof(msg) - sizeof(long), 0);
    }
}

void procesoC(int cola_id, int reps) {
    mensaje msg;
    for (int i = 0; i < reps; i++) {
        // Esperar y enviar mensajes 4 veces
        
            msgrcv(cola_id, &msg, sizeof(msg) - sizeof(long), C, 0);
            msg.mtype = B;
            msgsnd(cola_id, &msg, sizeof(msg) - sizeof(long), 0);
		msgrcv(cola_id, &msg, sizeof(msg) - sizeof(long), C, 0);
            msg.mtype = B;
            msgsnd(cola_id, &msg, sizeof(msg) - sizeof(long), 0);

	msgrcv(cola_id, &msg, sizeof(msg) - sizeof(long), C, 0);
            msg.mtype = B;
            msgsnd(cola_id, &msg, sizeof(msg) - sizeof(long), 0);
	msgrcv(cola_id, &msg, sizeof(msg) - sizeof(long), C, 0);

	// Imprimir C
        printf("C");
	fflush(stdout);
        // Enviar mensaje de tipo A
        msg.mtype = A;
        msgsnd(cola_id, &msg, sizeof(msg) - sizeof(long), 0);
    }
}

int main() {
    int repeticiones;

    do {
        printf("\nIngrese la cantidad de repeticiones: ");
        scanf("%d", &repeticiones);
    } while (repeticiones <= 0);

    // Crear cola de mensajes
    int cola_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if (cola_id == -1) {
        perror("Error al crear la cola de mensajes");
        exit(1);
    }

    // Crear procesos
    pid_t pidA = fork();
    if (pidA == 0) {
	
        procesoA(cola_id, repeticiones);
        exit(0);
    }

    pid_t pidB = fork();
    if (pidB == 0) {
        procesoB(cola_id, repeticiones);
        exit(0);
    }

    pid_t pidC = fork();
    if (pidC == 0) {
        procesoC(cola_id, repeticiones);
        exit(0);
    }

    // Inicializar la secuencia enviando el primer mensaje a A
    mensaje msg;
    msg.mtype = A;
    msgsnd(cola_id, &msg, sizeof(msg) - sizeof(long), 0);

    // Esperar a que todos los procesos terminen
    wait(NULL);
    wait(NULL);
    wait(NULL);

    // Eliminar la cola de mensajes
    msgctl(cola_id, IPC_RMID, NULL);

    printf("\nLa secuencia terminó\n");

    return 0;
}
