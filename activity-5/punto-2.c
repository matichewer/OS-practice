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

void procesoA(int cola_id) {
    mensaje msg;
    while(1) {
        // Esperar mensaje de tipo A
        msgrcv(cola_id, &msg, sizeof(msg) - sizeof(long), A, 0);

        printf("A");
	    fflush(stdout);
        // Enviar mensaje de tipo C
        msg.mtype = C;
        msgsnd(cola_id, &msg, sizeof(msg) - sizeof(long), 0);
        sleep(1);
    }
}

void procesoB(int cola_id) {
    mensaje msg;
    while(1) {
        // Esperar mensaje de tipo B
        msgrcv(cola_id, &msg, sizeof(msg) - sizeof(long), B, 0);

        // Imprimir B
        printf("B");
	    fflush(stdout);
        msg.mtype = A;
        msgsnd(cola_id, &msg, sizeof(msg) - sizeof(long), 0);
        sleep(1);
    }
}

void procesoC(int cola_id) {
    mensaje msg;
    while(1) {
        
        msgrcv(cola_id, &msg, sizeof(msg) - sizeof(long), C, 0); // espera C
        msg.mtype = B;
        msgsnd(cola_id, &msg, sizeof(msg) - sizeof(long), 0); // envia B
		msgrcv(cola_id, &msg, sizeof(msg) - sizeof(long), C, 0); // espera C
        msg.mtype = B;
        msgsnd(cola_id, &msg, sizeof(msg) - sizeof(long), 0); // envia B

	    msgrcv(cola_id, &msg, sizeof(msg) - sizeof(long), C, 0); // espera C
        msg.mtype = B;
        msgsnd(cola_id, &msg, sizeof(msg) - sizeof(long), 0); // envia B
	    msgrcv(cola_id, &msg, sizeof(msg) - sizeof(long), C, 0); // espera C

        printf("C\n");
	    fflush(stdout);
        // Enviar mensaje de tipo A
        msg.mtype = A;
        msgsnd(cola_id, &msg, sizeof(msg) - sizeof(long), 0); // envia A
        sleep(1);
    }
}

int main() {

    // Crear cola de mensajes
    int cola_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
    if (cola_id == -1) {
        perror("Error al crear la cola de mensajes");
        exit(1);
    }

    // Crear procesos
    pid_t pidA, pidB, pidC;
    pidA = fork();
    if (pidA == 0) {	
        procesoA(cola_id);
    } else {
        pidB = fork();
        if (pidB == 0) {
            procesoB(cola_id);
        } else {
            pidC = fork();
            if (pidC == 0) {
                procesoC(cola_id);
            }
        }
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

    return 0;
}
