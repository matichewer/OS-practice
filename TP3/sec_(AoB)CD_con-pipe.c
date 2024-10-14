#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

//(AoB)CD (AoB)CD ...

int pipeAB[2];
int pipeC[2];
int pipeD[2];

void a(){
  int entero;
	//pipes de lectura
	close(pipeAB[1]);
	
	//pipes de escritura
	close(pipeC[0]);

	//otros
	close(pipeD[0]);
	close(pipeD[1]);

	while (1)
	{
		read(pipeAB[0], &entero, sizeof(int)); // W(AoB)
		printf("A");
		fflush(stdout);
	//	sleep(1);
		write(pipeC[1], &entero, sizeof(int)); // S(C)
	}
}

void b(){
  int entero;
	//pipes de lectura
	close(pipeAB[1]);
	
	//pipes de escritura
	close(pipeC[0]);

	//otros
	close(pipeD[0]);
	close(pipeD[1]);

	while (1)
	{
		read(pipeAB[0], &entero, sizeof(int));
		printf("B");
		fflush(stdout);
	//	sleep(1);
		write(pipeC[1], &entero, sizeof(int));
	}
}

void c(){
  int entero;
	//pipes de lectura
	close(pipeC[1]);
	
	//pipes de escritura
	close(pipeD[0]);

	//otros
	close(pipeAB[0]);
	close(pipeAB[1]);

	while (1)
	{
		read(pipeC[0], &entero, sizeof(int));
		printf("C");
		fflush(stdout);
//		sleep(1);
		write(pipeD[1], &entero, sizeof(int));
	}
}

void d(){
  int entero;
	//pipes de lectura
	close(pipeD[1]);
	
	//pipes de escritura
	close(pipeAB[0]);

	//otros
	close(pipeC[0]);
	close(pipeC[1]);

	while (1)
	{
		read(pipeD[0], &entero, sizeof(int));
		printf("D\n");
		fflush(stdout);
		sleep(1);
		write(pipeAB[1], &entero, sizeof(int));
	}
}

int main(){
    int pid;
	int entero;
	
    if(pipe(pipeAB) == -1){
		printf("Error creando un pipe\n");
        exit(-1);
	}
    if(pipe(pipeC) == -1){
		printf("Error creando un pipe\n");
        exit(-1);
	}
    if(pipe(pipeD) == -1){
		printf("Error creando un pipe\n");
        exit(-1);
	}
    
    entero = 1; 
    //Inicializamos los pipes correspondientes
    write(pipeAB[1], &entero, sizeof(int));
    
    //Empezaremos a crear los hijos del padre
    pid = fork(); 
    if(pid < 0){
        printf("Error al crear un proceso\n");
        exit(-1);
	}
    if(pid == 0){
        a();       
    } else {
		pid = fork(); 
		if(pid < 0){
			printf("Error al crear un proceso\n");
			exit(-1);
		}
		if(pid==0){
			b();       
		} else {
			pid = fork(); 
			if(pid < 0){
				printf("Error al crear un proceso ");
				exit(-1);
			} 
			if(pid==0){
				c();       
			} else { 
				pid = fork(); 
				if(pid < 0){
					printf("Error al crear un proceso ");
					exit(-1);
				}
				if(pid==0){
					d();       
				}
			}
		}
}

		for(int i = 0; i<4; i++){
			wait(NULL);
		}
}
