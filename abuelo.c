#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int CalculaAleatorios(int min,int max) {
	return rand() % (max-min+1) + min;
}
void enforce(int n,char* msj) {
	if(n==-1) {
		perror(msj);
		exit(n);
	}

}
void nieto() {
	
	int numero = CalculaAleatorios(2,6);
	printf("Soy el nieto %d, mi padre es %d , tengo que dormir %d segundos\n",getpid(),getppid(),numero);
	sleep(numero);
	numero= CalculaAleatorios(0,1);
	printf("Soy el nieto %d, mi padre es %d , %s estoy resfriado\n",getpid(),getppid(),numero==0 ? "no" : "si");
	exit(numero);
}

void padre() {
	printf("Soy el padre %d, mi padre es %d\n",getpid(),getppid());
	pid_t pid_hijo;
	int estado;
	int num_enfermos = 0;
	for(int i=0; i<3; i++) {
		pid_hijo=fork();
		
		enforce(pid_hijo,"no se pueden crear los hijos");

		if(pid_hijo==0) {
			nieto();
		}
			
	}
	if(pid_hijo!=0){

		printf("Soy el padre %d, mi padre es %d, y estoy esperando por mis hijos\n",getpid(),getppid());
		pid_t p = wait(&estado);
		while (p > 0) {
			p = wait(&estado);
			num_enfermos+=WEXITSTATUS(estado);
				
		}
		printf("Soy el padre %d, mi padre es %d, y hay %d hijos enfermos\n",getpid(),getppid(),num_enfermos);
	
		exit(num_enfermos);

	}
	
}
void manejador(int signo) {
	
	if(signo == SIGUSR1) {


    printf("recibida SIGUSR1\n");
	
	}else if(signo == SIGUSR2) {


    printf("recibida SIGUSR2\n");
	
 	}
  
}

void Crear_padres(){
	pid_t ret; 
	int i;
	int estado;
 	int numero;
 	int contador=0;
	for (i=0; i<2; i++) {
		ret = fork();
		if (ret == 0) {
			/* estamos en alguno de los padres. */
			switch(i) {
				case 0:
					/* tratamiento padre 1. */
					printf("Padre1: Mi pid es %d\n", getpid());
					printf("Padre1: mi padre es %d\n", getppid());
					exit(1);
				case 1:
					/* tratamiento padre 2. */
					printf("Padre2: Mi pid es %d\n", getpid());
					printf("Padre2: mi padre es %d\n", getppid());
					exit(2);
			}
		} else if (ret > 0) {
			/* tratamiento del padre */
			ret = wait(&estado);
			while (ret > 0) {
				ret = wait(&estado);
				numero=WEXITSTATUS(estado);
				contador=contador + numero;
				printf("numero= %d\n", numero);
				printf("contador= %d\n",contador);

			}
		} else if (ret == -1) {
			perror("fallo en fork");
			exit(EXIT_FAILURE);
		}
	}
	
	
}

void abuelo() {
	/*wait(&estado);
		numero=WEXITSTATUS(estado);
		printf("el medico dice= %d\n",numero );
		padres();
		printf("Abuelo: Mi pid es %d\n", getpid());
		printf("Abuelo: mi padre es %d\n", getppid());*/
}


int main(void){
	srand (time(NULL));
	padre();
	
	/*int numero = 0;
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
 
*/
 return 0;
}