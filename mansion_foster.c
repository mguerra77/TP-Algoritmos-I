#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "la_sombra_de_mac.h"
#include "utiles.h"

#define MAX_NOMBRE 100
#define COMANDO_ORDENAR "ordenar_partidas"
#define COMANDO_CONFIG "config_juego"
#define COMANDO_AGREGAR "agregar_partida"
#define COMANDO_ELIMINAR "eliminar_partida"
#define LECTURA "%[^;];%i;%i;%i;%[^\n]\n"
#define ESCRITURA "%s;%i;%i;%i;%s\n"
#define NIVEL_1 1
#define NIVEL_2 2
#define NIVEL_3 3


typedef struct partida {
	char jugador[MAX_NOMBRE ];
	int nivel_llegado;
	int puntos;
	int vidas_restantes;
	bool gano;
} partida_t;

/* 
* Procedimiento que agrega una partida del archivo principal.
*/

void agregar_partida(FILE* archivo, FILE* agregar_partida_aux, partida_t partida_a_agregar){
	partida_t partida;
	bool insertado = false;
	char gano[MAX_NOMBRE];

	int leidos = fscanf(archivo, LECTURA, partida.jugador, &(partida.nivel_llegado), &(partida.puntos),
	&(partida.vidas_restantes), gano);
	while (leidos == 5){
		if (strcmp(gano, "Si") == 0){
			partida.gano = true;
		}
		if (strcmp(gano, "No") == 0){
			partida.gano = false;
		}
		if (strcmp(partida.jugador, partida_a_agregar.jugador) >= 0 && (!insertado)){
			if (strcmp(gano, "Si") == 0){
				partida_a_agregar.gano = true;
			}
			else{
				partida_a_agregar.gano = false;
			}
			fprintf(agregar_partida_aux, ESCRITURA, partida_a_agregar.jugador, partida_a_agregar.nivel_llegado, partida_a_agregar.puntos, partida_a_agregar.vidas_restantes, gano);
			insertado = true;
		}
		else{

			fprintf(agregar_partida_aux, ESCRITURA, partida.jugador, partida.nivel_llegado, partida.puntos, partida.vidas_restantes, gano);

			leidos = fscanf(archivo, LECTURA, partida.jugador, &(partida.nivel_llegado), &(partida.puntos), 
			&(partida.vidas_restantes), gano);
		}
	}
}

/* 
* Procedimiento que elimina una partida del archivo principal.
*/
void eliminar_partida(FILE* archivo, FILE* agregar_partida_aux, char partida_a_eliminar[MAX_NOMBRE]){
	partida_t partida;
	bool eliminado = false;
	char gano[MAX_NOMBRE];

	int leidos = fscanf(archivo, LECTURA, partida.jugador, &(partida.nivel_llegado), &(partida.puntos),
	&(partida.vidas_restantes), gano);
	while (leidos == 5){
		if (strcmp(gano, "Si") == 0){
			partida.gano = true;
		}
		if (strcmp(gano, "No") == 0){
			partida.gano = false;
		}
		if (strcmp(partida.jugador, partida_a_eliminar) != 0 || (strcmp(partida.jugador, partida_a_eliminar) == 0 && (eliminado))){
			fprintf(agregar_partida_aux, ESCRITURA, partida.jugador, partida.nivel_llegado, partida.puntos, partida.vidas_restantes, gano);
		}
		else{
			eliminado = true;
		}
		leidos = fscanf(archivo, LECTURA, partida.jugador, &(partida.nivel_llegado), &(partida.puntos),
		&(partida.vidas_restantes), gano);
	}
}

/* 
* Procedimiento que ordena los nombres alfabeticamente.
*/
void ordenar_nombres(partida_t* nombres , int tope ){
	char auxiliar;
 	bool esta_ordenado = false;
	int j = 0;
 	while ((j < tope) && (!esta_ordenado)){
	  	esta_ordenado = true;
 		for (int i=0; i < tope; i++) {
 			if(nombres->jugador[i]>nombres->jugador[i+1]){
 				auxiliar = nombres->jugador[i];
 				nombres->jugador[i] = nombres->jugador[i+1];
 				nombres->jugador[i+1] = auxiliar;
 				esta_ordenado = false;
 			}
 		}
 		j++;
 	}
}

/* 
* Procedimiento que ordena las partidas por nombre alfabeticamente.
*/
void ordenar_partidas(FILE* archivo, FILE* agregar_partida_aux){
	partida_t partida;
	partida_t nombres[MAX_ELEMENTOS];
	char gano[MAX_NOMBRE];
	int tope = 0;
	int i = 0;
	int leidos = fscanf(archivo, LECTURA, partida.jugador, &(partida.nivel_llegado), &(partida.puntos),
	&(partida.vidas_restantes), gano);

    while (leidos == 5 && (!feof(archivo))){
		strcpy(nombres[i].jugador, partida.jugador);
        tope++;
		i++;
	}
    ordenar_nombres((nombres), tope);
	while  (leidos == 5){
		fprintf(agregar_partida_aux, ESCRITURA, nombres->jugador, partida.nivel_llegado, partida.puntos, partida.vidas_restantes, gano);
	}
}

/* 
* Procedimiento que lee la configuracion de acuerdo a cada nivel los valores del archivo.
*/
void leer_por_nivel(int nivel, FILE* archivo){
    objeto_t elementos[MAX_ELEMENTOS];
    int i = 0;
	char linea[100];
    char* elemento;
	char* cantidad;
    int leidos = fscanf(archivo, "%[^\n]\n", linea);
     while (leidos == 1){
        elementos[i].numero_nivel = nivel;
        elemento = strtok(NULL, "=");
        if (strcmp(elemento, "POZOS")==0){
            strcpy(elementos[i].tipo, elemento);
            cantidad = strtok(NULL, "\0");
            elementos[i].cantidad = atoi(cantidad);
            i++;
            leidos = fscanf(archivo, "%[^\n]\n", linea);
            
        }
        if (strcmp(elemento, "PORTALES")==0){
            strcpy(elementos[i].tipo, elemento);
            cantidad = strtok(NULL, "\0");
            elementos[i].cantidad = atoi(cantidad);
            i++;
            leidos = fscanf(archivo, "%[^\n]\n", linea);
        }
        if (strcmp(elemento, "INTERRUPTORES")==0){
            strcpy(elementos[i].tipo, elemento);
            cantidad = strtok(NULL, "\0");
            elementos[i].cantidad = atoi(cantidad);
            i++;
            leidos = fscanf(archivo, "%[^\n]\n", linea);
        }
        if (strcmp(elemento, "MONEDAS")==0){
            strcpy(elementos[i].tipo, elemento);
            cantidad = strtok(NULL, "\0");
            elementos[i].cantidad = atoi(cantidad);
            i++;
            leidos = fscanf(archivo, "%[^\n]\n", linea);
        }
        if (strcmp(elemento, "ESCALERAS")==0){
            strcpy(elementos[i].tipo, elemento);
            cantidad = strtok(NULL, "\0");
            elementos[i].cantidad = atoi(cantidad);
            i++;
            leidos = fscanf(archivo, "%[^\n]\n", linea);
        }
        if (strcmp(elemento, "VELAS")==0){
            strcpy(elementos[i].tipo, elemento);
            cantidad = strtok(NULL, "\0");
            elementos[i].cantidad = atoi(cantidad);
            i++;
            leidos = fscanf(archivo, "%[^\n]\n", linea);
        }
        if (strcmp(elemento, "LLAVES")==0){
            strcpy(elementos[i].tipo, elemento);
            cantidad = strtok(NULL, "\0");
            elementos[i].cantidad = atoi(cantidad);
            i++;
            leidos = fscanf(archivo, "%[^\n]\n", linea);
        }
    }
}  

/* 
* Procedimiento que lee la configuracion con los valores del archivo.
*/
void leer_configuracion(FILE* archivo){
    
    objeto_t elementos[MAX_ELEMENTOS];
    int tope = 21;
	int i = 0;
	char linea[100];
	int leidos = fscanf(archivo, "%[^\n]\n", linea);
	char* nivel;

    while (leidos == 1 && i < tope){

		nivel = strtok(linea, "_");
		if (strcmp(nivel, "N1") == 0){
			leer_por_nivel(NIVEL_1, archivo);
		}
		if (strcmp(nivel, "N2") == 0){
			leer_por_nivel(NIVEL_2, archivo);
		}
		if (strcmp(nivel, "N3") == 0){
			leer_por_nivel(NIVEL_3, archivo);
		}
	}
    
	for (int i = 0; i < tope; i++){
		    printf("%i nivel %s = %i\n", elementos[i].numero_nivel, elementos[i].tipo, elementos[i].cantidad);
	}

	fclose(archivo);
}
/* Procedimiento que devuelve la configuracion con los valores por constantes.
*/
void configuracion_estandar(objeto_t elementos[MAX_ELEMENTOS]){

    elementos[0].cantidad = CANTIDAD_POZOS_NIVEL_1;
	elementos[1].cantidad = CANTIDAD_VELAS_NIVEL_1;
	elementos[2].cantidad = CANTIDAD_INTERRUPTORES_NIVEL_1;
	elementos[3].cantidad = CANTIDAD_PORTALES_NIVEL_1;
	elementos[4].cantidad = CANTIDAD_ESCALERAS_NIVEL_1;
    elementos[5].cantidad = CANTIDAD_MONEDAS_NIVEL_1;
    elementos[6].cantidad = CANTIDAD_LLAVES_NIVEL_1;
    elementos[7].cantidad = CANTIDAD_POZOS_NIVEL_2;
	elementos[8].cantidad = CANTIDAD_VELAS_NIVEL_2;
	elementos[9].cantidad = CANTIDAD_INTERRUPTORES_NIVEL_2;
	elementos[10].cantidad = CANTIDAD_PORTALES_NIVEL_2;
	elementos[11].cantidad = CANTIDAD_ESCALERAS_NIVEL_2;
    elementos[12].cantidad = CANTIDAD_MONEDAS_NIVEL_2;
    elementos[13].cantidad = CANTIDAD_LLAVES_NIVEL_2;
	elementos[14].cantidad = CANTIDAD_POZOS_NIVEL_3;
	elementos[15].cantidad = CANTIDAD_VELAS_NIVEL_3;
	elementos[16].cantidad = CANTIDAD_INTERRUPTORES_NIVEL_3;
	elementos[17].cantidad = CANTIDAD_PORTALES_NIVEL_3;
	elementos[18].cantidad = CANTIDAD_ESCALERAS_NIVEL_3;
    elementos[19].cantidad = CANTIDAD_MONEDAS_NIVEL_3;
    elementos[20].cantidad = CANTIDAD_LLAVES_NIVEL_3;
}

/* Procedimiento que le pide los datos correspondientes al usuario que va agregar una partida.
*/
void pedir_datos(partida_t partida_a_agregar){
	    char gano[MAX_NOMBRE];
	    printf("Nombre: ");
		scanf("%s", (partida_a_agregar.jugador));
		printf("Nivel_llegado: ");
		scanf("%d", &(partida_a_agregar.nivel_llegado));
		printf("Puntos: ");
		scanf("%d", &(partida_a_agregar.puntos));
		printf("Vidas restantes: ");
		scanf("%d", &(partida_a_agregar.vidas_restantes));
		printf("Gano el juego? (Si/No): ");
		scanf("%s", (gano));
}

int main(int argc, char* argv[]){
    
	FILE* nueva_partida;
	FILE* nueva_partida_aux;
    FILE* archivo;
    objeto_t elementos;

	if (strcmp(argv[1], "config_juego") == 0){
		archivo = fopen("config.txt", "r");
		leer_configuracion(archivo);
	}

	if (!archivo){
		configuracion_estandar(&(elementos));
	}
    
    if (strcmp(argv[1], "ordenar_partidas") == 0){
		nueva_partida = fopen(argv[2], "r");
		nueva_partida_aux = fopen("partidas_editada.csv", "w");
		ordenar_partidas(nueva_partida, nueva_partida_aux);
		rename("partidas_editada.csv", "partidas.csv");
	}

	else if (strcmp(argv[1], "eliminar_partida") == 0){
		nueva_partida = fopen(argv[2], "r");
		nueva_partida_aux = fopen("partidas_editada.csv", "w");
		eliminar_partida(nueva_partida, nueva_partida_aux, argv[3]);
		rename("partidas_editada.csv", "partidas.csv");
	}

	else if (strcmp(argv[1], "agregar_partida") == 0){
		nueva_partida = fopen(argv[2], "r");
		nueva_partida_aux = fopen("partidas_editada.csv", "w");
		partida_t partida_a_agregar;
		pedir_datos(partida_a_agregar);
		agregar_partida(nueva_partida, nueva_partida_aux, partida_a_agregar);
		rename("partidas_editada.csv", "partidas.csv");
	}

	if (!nueva_partida){
		perror("No se pudo abrir");
		fclose(nueva_partida);
		return -1;
	}
	if (!nueva_partida_aux){
		perror("No se pudo abrir");
		fclose(nueva_partida_aux);
		return -1;
	}

	fclose(nueva_partida);
    fclose(nueva_partida_aux);
    

	srand((unsigned)time(NULL));
    
    juego_t juego;

    inicializar_juego(&(juego), &(elementos));

    while (estado_juego((juego)) == 0){   
       imprimir_terreno(juego);
       realizar_jugada(&(juego));
    }
	return 0;
}
