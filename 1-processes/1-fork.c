#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int pid = fork();

    if (pid == 0) {
        printf("El hijo tiene el PID: %d\n", pid);
    } else {
        printf("El padre tiene el PID: %d\n", pid);
    };

    return 0;
}
