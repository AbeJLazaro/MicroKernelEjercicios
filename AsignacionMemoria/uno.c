#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define TAMBLOQUE 127

// para los datos

// estructuras
struct dato{
  char* nombre;
  int tam;
};

// funciones
int leer();

// variables globales
struct dato* datos;
int sizeDatos;

// para las listas

// estructuras
typedef struct snodo{ //snodo es el nombre de la estructura
    int valor;
    struct snodo *sig; //El puntero siguiente para recorrer la lista enlazada
}tnodo; //tnodo es el tipo de dato para declarar la estructura
 
typedef tnodo *tpuntero; //Puntero al tipo de dato tnodo para no utilizar punteros de punteros
 
// funciones
void insertarEnLista(tpuntero *cabeza, int e);
int eliminarUltimo(tpuntero *cabeza);
int eliminarPrimero(tpuntero *cabeza);
void imprimirLista (tpuntero cabeza);
void borrarLista (tpuntero *cabeza);


// para la asignación de memoria

// estructuras
struct archivo{
  char nombre[100];
  tpuntero bloques;
  int fragmento;
};

// funciones
void agregarDatosArchivo();
void agregarArchivo();
void eliminarArchivo();
void menu();

// lista de archivos
typedef struct snodoa{ //snodoa es el nombre de la estructura
    struct archivo file;
    struct snodoa *sig; //El puntero siguiente para recorrer la lista enlazada
}tnodoa; //tnodoa es el tipo de dato para declarar la estructura
 
typedef tnodoa *tpunteroa; //Puntero al tipo de dato tnodoa para no utilizar punteros de punteros
 
// funciones
void insertarEnListaA(tpunteroa *cabeza, struct archivo e);
void imprimirListaA(tpunteroa cabeza);
int eliminarNodoListaA(tpunteroa *cabeza, tpunteroa nodo);
tpunteroa buscarArchivo(char* name);

// variables globales
tpuntero bloquesLibres;
tpunteroa listaArchivos;

// función main
int main() {
  // leemos el archivo
  int lecturaDatos = leer();
  // si nos regresa un -1, hubo un error
  if(lecturaDatos==-1){
    return -1;
  }
  // se agregan todos lo
  for (int i = 0; i < 60; ++i){
    insertarEnLista(&bloquesLibres, i);
  }
  agregarDatosArchivo();

  menu();
  return 0;
}

// función para leer los datos del archivo
int leer(){
  // variables para leer el texto del archivo
  char texto[100][100];
  char control;

  // apuntador del archivo
  FILE* fichero;
  fichero = fopen("datos.txt", "r");

  // comprobación de la apertura del archivo
  if(fichero==NULL){
    perror("Error al abrir el archivo");
    return -1;
  }

  // indices para los archivos
  int j = 0;
  int i = 1;
  int nombre = 0;

  do{
    do{
      control = fgetc(fichero);

      // fin de cadena \0 (recuerda que en C todas las cadenas se terminan 
      // con un byte cero, que se representa como \0)
      texto[j][i]='\0';

      // si el nombre contiene caracteres validos
      if (control>32 && control<=126){
        // si es un nombre que tenga una longitud valida, si no, se trata de 
        // un número
        if((i<=12 && nombre==0) || nombre == 1){
          texto[j][i-1] = control;
        }
      }

      // si el nombre del archivo llega a tener más de 12 caracteres
      if(i==13 && nombre ==0){
        printf("Se detectó un nombre muy largo, el máximo es 12 carateres\n");
      }
      i++;

    // mientras el caracter leido sea un caracter valido
    }while (control>32 && control<=126);
    // printf ("cadena: %s \n", texto[j]);
    i=1;
    j++;

    nombre = !nombre;
  // mientras no se llegue al fin del archivo
  }while(control!=EOF);

  // se genera un arreglo de datos
  struct dato otro[j/2];

  // se guardan los datos en el arreglo
  int k = 0;
  for (i = 0; i < j/2; i++){
    otro[i].nombre = texto[k];
    otro[i].tam = atoi(texto[k+1]);
    k=k+2;
  }

  /*
  // comprobación de que si se generan bien los datos
  for (i = 0; i < j/2; i++){
    printf("%s,%i\n",otro[i].nombre,otro[i].tam);
  }

  */

  // se guardan los datos en las variables globales
  datos = otro;
  sizeDatos = j/2;

  // se cierra el archivo
  fclose(fichero);
  
  return 0;
}

// función para agregar datos a la lista
void insertarEnLista(tpuntero *cabeza, int e){
  tpuntero nuevo; // creamos un nuevo nodo

  // utilizamos malloc para reservar memoria para ese nodo
  nuevo = malloc(sizeof(tnodo)); 

  // le asignamos el valor
  nuevo->valor = e; 
  nuevo->sig = NULL; 

  // si la lista esta vacía
  if(*cabeza == NULL){
    *cabeza = nuevo;
    return;
  }

  // puntero auxiliar para movernos por la lista
  tpuntero aux = *cabeza;
  while(aux->sig!=NULL){
    aux = aux->sig;
  }

  aux->sig = nuevo; // agregamos el nodo al final
}

// función para eliminar el ultimo dato de la lista
int eliminarUltimo(tpuntero *cabeza){
  // apuntador auxiliar
  tpuntero aux = *cabeza;

  // si no tiene datos
  if(aux == NULL){
    return -1;
  }
  // si solo tiene uno
  if(aux->sig == NULL){
    *cabeza = NULL;
    int num = aux->valor;
    free(aux);
    return num;
  }
  // si tiene más de uno
  while(aux->sig->sig!=NULL){
    aux = aux->sig;
  }
  // puntero auxiliar para liberar la memoria del nodo que se elimina
  tpuntero otro = aux->sig;
  int num = otro->valor;
  aux->sig = NULL;
  free(otro);
  return num;
}

// función para eliminar el primer dato de la lista
int eliminarPrimero(tpuntero *cabeza){
  // apuntador auxiliar
  tpuntero aux = *cabeza;

  // si no tiene datos
  if(aux == NULL){
    return -1;
  }
  // si solo tiene uno
  if(aux->sig == NULL){
    int num = aux->valor;
    *cabeza = NULL;
    free(aux);
    return num;
  }
  // si tiene más de uno
  int num = (*cabeza)->valor;
  *cabeza = (*cabeza)->sig;
  free(aux);
  return num;
}

// función para imprimir la lista
void imprimirLista(tpuntero cabeza){
  // mientras cabeza no sea NULL
  while(cabeza != NULL){
    // imprimimos el valor del nodo
    printf("%d, ",cabeza->valor); 
    // pasamos al siguiente nodo
    cabeza = cabeza->sig; 
  }
}
 
// función para eliminar una lista
void borrarLista(tpuntero *cabeza){ 
  // puntero auxiliar para eliminar la lista
  tpuntero actual; 
  // mientras cabeza no sea NULL
  while(*cabeza != NULL){ 
    // actual toma el valor de cabeza
    actual = *cabeza; 
    // cabeza avanza 1 posicion en la lista
    *cabeza = (*cabeza)->sig; 
    // se libera la memoria de la posicion de Actual (el primer nodo), 
    // y cabeza queda apuntando al que ahora es el primero
    free(actual); 
  }
}

// lista de archivos

// función para agregar datos a la lista
void insertarEnListaA(tpunteroa *cabeza, struct archivo e){
  tpunteroa nuevo; // creamos un nuevo nodo

  // utilizamos malloc para reservar memoria para ese nodo
  nuevo = malloc(sizeof(tnodoa)); 

  // le asignamos el valor
  nuevo->file = e; 
  nuevo->sig = NULL; 

  // si la lista esta vacía
  if(*cabeza == NULL){
    *cabeza = nuevo;
    return;
  }

  // puntero auxiliar para movernos por la lista
  tpunteroa aux = *cabeza;
  while(aux->sig!=NULL){
    aux = aux->sig;
  }

  aux->sig = nuevo; // agregamos el nodo al final
}

// función para eliminar datos de la lista
int eliminarNodoListaA(tpunteroa *cabeza, tpunteroa nodo){
  tpunteroa aux = *cabeza;
  // si el que se va a eliminar es la cabeza
  if(aux == nodo){
    *cabeza = (*cabeza)->sig;
    while(aux->file.bloques!=NULL){
      int a = eliminarPrimero(&(aux->file.bloques));
      insertarEnLista(&bloquesLibres,a);
    }
    free(aux);
    return 1;
  }
  // si el que se elimina está en medio
  while(aux->sig != nodo && aux->sig != NULL){
    aux = aux->sig;
  }

  if(aux->sig == nodo){
    tpunteroa eliminar = aux->sig;
    aux->sig = eliminar->sig;
    while(eliminar->file.bloques!=NULL){
      int a = eliminarPrimero(&(eliminar->file.bloques));
      insertarEnLista(&bloquesLibres,a);
    }
    free(eliminar);
    return 1;
  }
  // si ocurre un error
  return -1;
}

// función para imprimir la lista
void imprimirListaA(tpunteroa cabeza){
  // mientras cabeza no sea NULL
  while(cabeza != NULL){
    // imprimimos el valor del nodo
    printf("nombre: %s, bloques:",cabeza->file.nombre);
    imprimirLista(cabeza->file.bloques);
    printf("fragmento: %d \n",cabeza->file.fragmento);
    // pasamos al siguiente nodo
    cabeza = cabeza->sig; 
  }
}

// función para reorganizar la lista de menor a mayor
void ordenarListaA(){

  tpuntero aux = bloquesLibres;
  int i=0;

  while(aux!=NULL){
    i++;
    aux=aux->sig;
  }

  if(i==0){
    return;
  }

  int numeros[i],j=0;
  aux = bloquesLibres;

  while(aux!=NULL){
    numeros[j]=aux->valor;
    j++;
    aux=aux->sig;
  }

  int cantidad = i;
  int otro;
  for (i = 0; i < cantidad; ++i){
    for (j = i+1; j < cantidad; ++j){
      if(numeros[i]>numeros[j]){
        otro = numeros[i];
        numeros[i] = numeros[j];
        numeros[j] = otro;
      }
    }
  }

  tpuntero nueva = NULL;
  for (i = 0; i < cantidad; ++i){
    insertarEnLista(&nueva,numeros[i]);
  }

  bloquesLibres = nueva;
}

// programa principal

// función para agregar los archivos del archivo de datos
void agregarDatosArchivo(){
  // puntero para una lista de archivos ingresados
  tpunteroa cabeza;
  cabeza = NULL;

  // para cada archivo leido
  for (int i = 0; i < sizeDatos; ++i){
    // creamos un archivo auxiliar
    struct archivo aux;

    // agregamos el nombre
    strcpy(aux.nombre,datos[i].nombre);

    // obtenemos el tamaño para determinar el numero de bloques que ocupará
    int tam = datos[i].tam;
    int numBloques = (int)(tam/TAMBLOQUE);
    if((tam%TAMBLOQUE)!=0){
      numBloques++;
    }
    // iniciamos la lista de bloques (numeros)
    aux.bloques = NULL;
    // para cada bloque que es necesario
    for (int j = 0; j < numBloques; ++j){
      // obtenemos un bloque de la lista de bloques libres
      int bloqueSeleccionado = eliminarPrimero(&bloquesLibres);
      // si el bloque regresado es -1 significa que ya no hay bloques
      if(bloqueSeleccionado==-1){
        printf("Se acabaron los bloques, no se puede continuar con la inserción de archivos\n");
        printf("No se pueded agregar el archivo %s\n",aux.nombre);
        return;
      }
      // si no es -1, se agrega el bloque a la lista de bloques para almacenar el archivo
      insertarEnLista(&(aux.bloques), bloqueSeleccionado);
    }
    
    // se agrega el fragmento sobrante del archivo
    aux.fragmento = (TAMBLOQUE*numBloques)-datos[i].tam;
    // se inserta este archivo a la lista de archivos
    insertarEnListaA(&cabeza, aux);
  }

  // se imprime la lista
  imprimirListaA(cabeza);

  listaArchivos = cabeza;
}

// función para agregar archivos
void agregarArchivo(){
  int tam;
  char namef[100];
  int bandera = 0;
  struct archivo aux;

  // pedimos los datos
  printf("Ingresa el nombre del archivo\n");
  scanf("%s",namef);
  for (int i = 0; i <=12; ++i){
    if(namef[i]=='\0'){
      bandera = 1;
    }
  }
  if(bandera==0){
    printf("Nombre muy largo\n");
    return;
  }

  strcpy(aux.nombre,namef);

  printf("Ingresa el tamaño en bytes\n");
  scanf("%i",&tam);

  // determinar el numero de bloques que ocupará
  int numBloques = (int)(tam/TAMBLOQUE);
  if((tam%TAMBLOQUE)!=0){
    numBloques++;
  }

  aux.bloques = NULL;
  // para cada bloque que es necesario
  for (int j = 0; j < numBloques; ++j){
    // obtenemos un bloque de la lista de bloques libres
    int bloqueSeleccionado = eliminarPrimero(&bloquesLibres);
    // si el bloque regresado es -1 significa que ya no hay bloques
    if(bloqueSeleccionado==-1){
      printf("Se acabaron los bloques, no se puede continuar con la inserción de archivos\n");
      printf("No se pueded agregar el archivo %s\n",aux.nombre);
      return;
    }
    // si no es -1, se agrega el bloque a la lista de bloques para almacenar el archivo
    insertarEnLista(&(aux.bloques), bloqueSeleccionado);
  }

  // se agrega el fragmento sobrante del archivo
  aux.fragmento = (TAMBLOQUE*numBloques)-tam;
  // se inserta este archivo a la lista de archivos
  insertarEnListaA(&listaArchivos, aux);

  printf("Inserción completa\n");
} 

// función para agregar archivos
void eliminarArchivo(){
  // se pide el nombre
  char name[12];

  printf("Ingresa el nombre del archivo a eliminar\n");
  scanf("%s",name);

  // se busca el nodo
  tpunteroa nodoB = buscarArchivo(name);
  // si no se encuentra el archivo, se envia el error
  if(nodoB==NULL){
    printf("No se encontró el archivo con el nombre %s\n",name);
    return;
  }
  
  // creamos una bandera como resultado de eliminar el archivo
  int bandera = eliminarNodoListaA(&listaArchivos,nodoB);
  // si es -1, indica un error
  if(bandera!=1){
    printf("Ocurrió un error al eliminar el archivo\n");
    return;
  }
}

// fución para buscar un archivo
tpunteroa buscarArchivo(char* name){
  // puntero auxiliar
  tpunteroa aux = listaArchivos;
  // mientras el puntero no sea nulo
  while(aux!=NULL){
    // comparamos el nombre del archivo en el puntero y el pasado como argumento
    if(strcmp(aux->file.nombre, name)==0){
      // si son iguales, retornamos el nodo
      return aux;
    }
    // si no son iguales, avanzamos en la lista
    aux = aux->sig;
  }
  // si no lo encuentra, devuelve un nulo
  return NULL;
}

void menu(){
  int opcion;
  do{
    printf("¿Qué desea hacer?\n");
    printf("1. agregar un archivo\n");
    printf("2. eliminar un archivo\n");
    printf("3. mostrar lista de archivos\n");
    printf("4. mostrar lista de bloques libres\n");
    printf("0. salir\n");
    scanf("%i",&opcion);

    switch(opcion){
      case 1:
        agregarArchivo();
        imprimirListaA(listaArchivos);
        break;
      case 2:
        eliminarArchivo();
        imprimirListaA(listaArchivos);
        printf("Lista de bloques libres: \n");
        //******************************************************************************************
        ordenarListaA();
        imprimirLista(bloquesLibres);
        printf("\n");
        break;
      case 3:
        imprimirListaA(listaArchivos);
        break;
      case 4:
        printf("Lista de bloques libres: \n");
        imprimirLista(bloquesLibres);
        printf("\n");
        break;
      default:
        opcion=0;
    }
  }while(opcion!=0);
}