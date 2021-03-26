#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

int factorial(int n);

int main(){

  // variables que nos ayudarán
  pid_t pid;
  // a es el pipe cliente-sistema, b sistema-cliente
  int a[2],b[2];
  char msg[256];
  int resultado,contador=0,auxiliar;

  // se limpia la pantalla
  system("clear");

  // creación de los pipes
  if (pipe(a)<0){
    perror("\nError en la creación del pipe a");
    exit(1);
  }

  if (pipe(b)<0){
    perror("\nError en la creación del pipe b");
    exit(1);
  }

  // 0 es de lectura
  // 1 es de escritura

  printf("Proceso cliente con pid=%i\n",getpid());
  printf("==================================================\n\n");

  pid=fork();
  switch(pid){
    // entra aquí en caso de que no se pueda crear el hijo
    case -1: 
      printf("No se pudo clonar el proceso\n");
      exit(-1);
      break;
    // si es el proceso sistema, entra aquí
    case 0:
      // se cierra la escritura del canal a
      close(a[1]);
      // se cierra la lectura del canal b
      close(b[0]);
      while(contador<10){
        // primero lee del archivo canal a
        read(a[0],msg,sizeof(msg));
        
        // transforma ese número a entero
        auxiliar=atoi(msg);

        // mensaje del sistema
        printf("\t--> Sistema (pid=%i) recibe numero: %i\n",getpid(),auxiliar);

        // se calcula el factorial del numero
        resultado=factorial(auxiliar);

        // escribe el mensaje en el archivo
        sprintf(msg,"%i",resultado);

        // mensaje del sistema
        printf("\t--> Sistema (pid=%i) envia resultado: %i\n",getpid(),resultado);

        write(b[1],msg,sizeof(msg));

        // aumenta el contador
        contador++;
      }
      exit(0);
      break;

    // si es el proceso cliente, entra aquí
    default:
      // se genera la semilla para número random
      srand(time(NULL));
      // se cierra la lectura del canal a
      close(a[0]);
      // se cierra la escritura del canal b
      close(b[1]);

      // ciclo de 10 iteraciones
      while(contador<10){

        // calculamos un numero aleatorio
        resultado = rand()%10;
        if(resultado==0){
          resultado=10;
        }
        sprintf(msg,"%i",resultado);

        // mensaje del cliente
        printf("Cliente (pid=%i), pide factorial de %i\n",getpid(),resultado);

        // escribimos en el pipe
        write(a[1],msg,sizeof(msg));

        // pequeño delay
        // sleep(0.7);

        // leemos el canal b e imprimimos el resultado
        read(b[0],msg,sizeof(msg));
        printf("Cliente (pid=%i), recibe información, factorial de %i = %s\n",getpid(),resultado,msg);
        contador++;

        printf("================================================\n\n");
      }
      break;
  }
  wait(NULL);
  printf("Terminó el proceso\n");
  return 1;
}

// función para calcular el factorial de un número n
int factorial(int n){
  switch(n){
    // si es cero o uno, regresa 1
    case 0:
    case 1:
      return 1;
    // se compara cualquier otro caso
    default:
      // si es menor a 0, regresa un -1
      if(n<0){
        return -1;
      // si no, se calcula con un ciclo while
      }else{
        int resultado=1;
        while(n>1){
          resultado=resultado*n;
          n--;
        }
        return resultado;
      }
  }

}