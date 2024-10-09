#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// ABABABACABABABAC...

#define MSG_TYPE_A 1
#define MSG_TYPE_B 2
#define MSG_TYPE_C 3

struct message {
    long mtype;
    char text[1];
};

int msg_queue_id;

void* tareaA(void* arg) {
    struct message msg;

    while (1) {
        msgrcv(msg_queue_id, &msg, sizeof(msg.text), MSG_TYPE_A, 0);
        printf("A");
        fflush(stdout);
        msg.mtype = MSG_TYPE_C;
        msgsnd(msg_queue_id, &msg, sizeof(msg.text), 0);
        sleep(1);
    }
}

void* tareaB(void* arg) {
    struct message msg;

    while (1) {
        msgrcv(msg_queue_id, &msg, sizeof(msg.text), MSG_TYPE_B, 0);
        printf("B");
        fflush(stdout);
        msg.mtype = MSG_TYPE_A;
        msgsnd(msg_queue_id, &msg, sizeof(msg.text), 0);
        sleep(1);
    }
}

void* tareaC(void* arg) {
    struct message msg;

    while (1) {
        msgrcv(msg_queue_id, &msg, sizeof(msg.text), MSG_TYPE_C, 0);
        msg.mtype = MSG_TYPE_B;
        msgsnd(msg_queue_id, &msg, sizeof(msg.text), 0);
        
        msgrcv(msg_queue_id, &msg, sizeof(msg.text), MSG_TYPE_C, 0);
        msg.mtype = MSG_TYPE_B;
        msgsnd(msg_queue_id, &msg, sizeof(msg.text), 0);
        
        msgrcv(msg_queue_id, &msg, sizeof(msg.text), MSG_TYPE_C, 0);
        msg.mtype = MSG_TYPE_B;
        msgsnd(msg_queue_id, &msg, sizeof(msg.text), 0);

        msgrcv(msg_queue_id, &msg, sizeof(msg.text), MSG_TYPE_C, 0); // Esperar un A
        printf("C\n");
        fflush(stdout);
        msg.mtype = MSG_TYPE_A;
        msgsnd(msg_queue_id, &msg, sizeof(msg.text), 0);
        sleep(1);
    }
}

int main() {
    pthread_t threadA, threadB, threadC;

    msg_queue_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);

    // Initialize message queues with initial messages
    struct message msg;
    msg.mtype = MSG_TYPE_A;
    msgsnd(msg_queue_id, &msg, sizeof(msg.text), 0);

    pthread_create(&threadA, NULL, tareaA, NULL);
    pthread_create(&threadB, NULL, tareaB, NULL);
    pthread_create(&threadC, NULL, tareaC, NULL);

    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);

    msgctl(msg_queue_id, IPC_RMID, NULL);

    return 0;
}