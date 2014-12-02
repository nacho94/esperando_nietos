##include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>


void manejador(int signo) {
	
	if(signo == SIGUSR1) {


    printf("recibida SIGUSR1\n");
	
	}else if(signo == SIGUSR2) {


    printf("recibida SIGUSR2\n");
	
 	}
  
}

int CalculaAleatorios(int min,int max) {
	return rand() % (max-min+1) + min;
}

int main(void){

	srand (time(NULL));
	int numero = 0;
 	int i=0;
	pid_t pid;
	signal(SIGUSR1,manejador);
	signal(SIGUSR2,manejador);
 	pid=fork();
 	if (pid==-1){
 		perror("Error en la llamada a fork()");
 		exit(0);
 	} else if (pid == 0){
 
		pause();
		sleep(1); 
		printf("el medico comunica si esta disponible\n");
		int i = CalculaAleatorios(0,1);
		printf("i= %d\n", i);

		switch(i) {
			case 0:
				kill(getppid(),SIGUSR1);
				break;
			case 1:
				kill(getppid(),SIGUSR2);
				break;

		}

 	} else {
 
 		printf("El abuelo comprueba si el medico esta disponible\n");
 		sleep(1);
 		kill(pid,SIGUSR1);
 		pause();
 
		printf("El medico no esta disponible\n");

 	}
 

 return 0;
}