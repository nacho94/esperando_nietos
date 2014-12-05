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

void manejador_medico(int s) {
	printf("Soy el medico %d, y he recibido %d\n",getpid(),s);

}

void medico() {
	signal(SIGUSR1,manejador_medico);
	printf("Soy el medico %d, y esoty esperando por la señal del abuelo\n",getpid());
	pause();
	sleep(1); 
	int i = CalculaAleatorios(0,1);
	printf("Soy el medico %d, y el abuelo me ha despertado. %s estoy disponible\n",getpid(),i==0 ? "no" : "si");

	switch(i) {
		case 0:
			kill(getppid(),SIGUSR1);
			break;
		case 1:
			kill(getppid(),SIGUSR2);
			break;

	}

}



void padres(){
	pid_t pid_padre; 
	int i;
	int estado;
 	int num_enfermos;
 	
	for (i=0; i<2; i++) {
		pid_padre = fork();
		enforce(pid_padre,"no se han podido crear los padres");
		if (pid_padre == 0) {
			padre();
		}
	}
	if(pid_padre!=0){

		printf("Soy el abuelo %d, y estoy esperando por mis hijos y nietos\n",getpid());
		pid_t p = wait(&estado);
		while (p > 0) {
			p = wait(&estado);
			num_enfermos+=WEXITSTATUS(estado);
				
		}
		printf("Soy el abuelo %d, y hay %d nietos enfermos\n",getpid(),num_enfermos);

	}
	
}

void manejador_abuelo(int signo) {

	printf("Soy el abuelo %d, y he recibido %d\n",getpid(),signo);
	
	if(signo==SIGUSR1) {
		printf("Soy el abuelo %d, el medico no esta disponible y no dejo salir a los nietos\n",getpid());
	}
	if(signo==SIGUSR2)  {
		printf("Soy el abuelo %d, el medico si esta disponible y dejo salir a mis nietos\n",getpid());
		padres();
	}
  
}

void abuelo() {
	printf("Soy el abuelo %d\n",getpid());
	pid_t pid_medico;

	pid_medico=fork();
	enforce(pid_medico,"no se puede crear el medico");

	if(pid_medico==0) {
		medico();
	}else {
		printf("Soy el abuelo %d, y voy a pregunrle al medico %d si esta disponible\n",getpid(),pid_medico);
		sleep(1);
		int estado;
		signal(SIGUSR1,manejador_abuelo);
		signal(SIGUSR2,manejador_abuelo);
		kill(pid_medico,SIGUSR1);

		pause();
		wait(&estado);
	}
	/*wait(&estado);
		numero=WEXITSTATUS(estado);
		printf("el medico dice= %d\n",numero );
		padres();
		printf("Abuelo: Mi pid es %d\n", getpid());
		printf("Abuelo: mi padre es %d\n", getppid());*/
}


int main(void){
	srand (time(NULL));
	//padre();
	abuelo();
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