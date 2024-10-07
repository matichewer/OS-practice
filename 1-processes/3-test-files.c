#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    FILE *fp = fopen("test.txt", "w");

    if (fp == NULL) {
        perror("Error abriendo el archivo");
        return 1;
    }

    fprintf(fp, "Hola desde PID: %d\n", getpid());
    fflush(fp); // para limpiar el buffer

    pid_t pid = fork();

    if (pid == 0) {
        fprintf(fp, "Estamos en el hijo porque el fork retornó %d. Pero getpid() es %d\n", pid, getpid());
        fclose(fp);
    } else {
        wait(NULL);
        fprintf(fp, "Estamos en el padre porque el fork retornó %d. Pero el getpid() es %d\n", pid, getpid());
        fclose(fp);
    };

    return 0;
}
