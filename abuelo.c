#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

int CalculaAleatorios(int min,int max) { // Funcion que genera los numeros aleatorios
	return rand() % (max-min+1) + min;
}

void enforce(int n,char* msj) { // Funcion que se ejecuta en el caso de que la creacion de los hijos sea erronea
	if(n==-1) {
		perror(msj);
		exit(n);
	}

}

void nieto() { // Funcion en la que se realizan la tareas que debe llevar a cabo cada uno de los nietos

	srand(getpid());
	int numero = CalculaAleatorios(2,6);
	printf("Soy el nieto %d, mi padre es %d , tengo que dormir %d segundos\n",getpid(),getppid(),numero);
	sleep(numero);
	numero= CalculaAleatorios(0,1);
	printf("Soy el nieto %d, mi padre es %d , %s estoy resfriado\n",getpid(),getppid(),numero==0 ? "no" : "si");
	// Si el numero calculado es cero se imprime que no esta resfriado y si es un uno que si los estan
	exit(numero);
}

void padre() { // Funcion en la que se realizan las  tareas que deben llevar a cabo los padres
	printf("Soy el padre %d, mi padre es %d\n",getpid(),getppid());
	pid_t pid_hijo;
	int estado;
	int num_enfermos = 0;// Contador para saber cuatos nietos estan enfermos
	int i;
	for(i=0; i<3; i++) {

		pid_hijo=fork(); // Se crean los nietos con la llamada a fork()
		
		enforce(pid_hijo,"no se pueden crear los hijos"); // Se ejcuta en caso de error en la creacion de los nietos

		if(pid_hijo==0) { // Codigo que ejcutan los nietos
			nieto();
		}
			
	}
	if(pid_hijo!=0){ // Codigo que ejecutan los padres

		printf("Soy el padre %d, mi padre es %d, y estoy esperando por mis hijos\n",getpid(),getppid());
		int j=0;
		while (pid_hijo> 0 && j<3) { // Bucle para esperar por todos los hijos del padre

			pid_hijo = wait(&estado);
			
			num_enfermos+=WEXITSTATUS(estado); // Se suma al contador un cero si no estan enfermos y un uno en caso contrario
			j++;	
		}
		printf("Soy el padre %d, mi padre es %d, y tengo %d hijos enfermos\n",getpid(),getppid(),num_enfermos);
	
		exit(num_enfermos);

	}
	
}

void manejador_medico(int s) { // Manejadora del madico

	printf("Soy el medico %d, y he recibido la señal SIGUSR1\n",getpid());

}

void medico() {
	signal(SIGUSR1,manejador_medico); // Se trata la señal SIGUSR1

	printf("Soy el medico %d, y estoy esperando por la señal del abuelo\n",getpid());

	pause(); // El medico espera sin hacer nada mas hasta recibir la señal del abuelo

	sleep(1); 
	int i = CalculaAleatorios(0,1);
	printf("Soy el medico %d, y el abuelo me ha despertado. %s estoy disponible\n",getpid(),i==0 ? "no" : "si");

	switch(i) {
		case 0:
			kill(getppid(),SIGUSR1); // Envia la señal SIGUSR1 al abuelo
			break;
		case 1:
			kill(getppid(),SIGUSR2); // Envia la señal SIGUSR2 al abuelo
			break;

	}

}



void padres(){ // Funcion en la que se crean los padres y se espera por ellos
	pid_t pid_padre; 
	int i;
	int estado;
 	int num_enfermos=0;
 	
	for (i=0; i<2; i++) {
		pid_padre = fork();
		enforce(pid_padre,"no se han podido crear los padres");
		if (pid_padre == 0) { // Codigo que ejecutan los padres
			padre();
		}
	}
	if(pid_padre!=0){ // Codigo que ejecuta el abuelo

		printf("Soy el abuelo %d, y estoy esperando por mis hijos y nietos\n",getpid());
		int j=0;
		while (pid_padre > 0 && j<3) { // Bucle para esperar por todos los hijos del abuelo
			pid_padre = wait(&estado);
			
			num_enfermos+=WEXITSTATUS(estado); // Se suman el numero de nietos enfermos que ha devuelto cada padre
			j++;	
		}
		printf("Soy el abuelo %d, tengo %d nietos que han llegado a casa enfermos,%s habra que ir al doctor\n",getpid(),num_enfermos,num_enfermos==0 ? " no" : "");

	}
	
}

void manejador_abuelo(int signo) { // Manejadora del abuelo

	
	
	if(signo==SIGUSR1) { // Si la señal recibida es SIGUSR1, se realiza el codigo contenido en el if
		printf("Soy el abuelo %d, y he recibido la señal SIGUSR1\n",getpid());
		printf("Soy el abuelo %d, el medico no esta disponible y no dejo salir a los nietos\n",getpid());
	}
	if(signo==SIGUSR2)  { // Si la señal recibida es SIGUSR2, se realiza el codigo contenido en el if
		printf("Soy el abuelo %d, y he recibido la señal SIGUSR2\n",getpid());
		printf("Soy el abuelo %d, el medico si esta disponible y dejo salir a mis nietos\n",getpid());
		// Como el abuelo a recibido la señal SIGUSR1, se llama a la funcion padres que creara los padres
		padres();
	}
  
}

void abuelo() { // Funcion en la que se realizan la tareas del abuelo
	printf("Soy el abuelo %d\n",getpid());
	pid_t pid_medico;

	pid_medico=fork(); // Se crea el medico
	enforce(pid_medico,"no se puede crear el medico");

	if(pid_medico==0) {
		medico();
	}else {
		printf("Soy el abuelo %d, y voy a preguntarle al medico %d si esta disponible\n",getpid(),pid_medico);
		sleep(1);
		int estado;
		signal(SIGUSR1,manejador_abuelo); // Se trata la señal SIGUSR1
		signal(SIGUSR2,manejador_abuelo); // Se trata la señal SIGUSR2
		kill(pid_medico,SIGUSR1); // El abuelo manda la señal SIGUSR1 la medico

		pause(); // Espera hasta recibir una señal del medico que sera tratada en la manejadora del propio abuelo
		wait(&estado);
	}
}


int main(void){

	srand (time(NULL)); // Inicializacion de una semilla de numeros aleatorios

	abuelo(); // Llamamos a la funcion abuelo

 	return 0;
}