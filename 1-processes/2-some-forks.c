#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int pid1 = fork();
    int pid2 = fork();

    printf("PID1: %d\n", pid1);
    printf("PID2: %d\n", pid2);

    if (pid1 == 0)
        printf("PID1 - El hijo tiene el PID: %d\n", pid1);
    sleep(1);
    printf("\n");

    if (pid1 > 0)
        printf("PID1 - El padre tiene el PID: %d\n", pid1);
    sleep(1);
    printf("\n");

    if (pid2 == 0)
         printf("PID2 - El hijo tiene el PID: %d\n", pid2);
    sleep(1);
    printf("\n");

    if (pid2 > 0)
        printf("PID2 - El padre tiene el PID: %d\n", pid2);

    return 0;
}
