#include "la_sombra_de_mac.h"
#include "utiles.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


const int MIN_COLUMNAS = 0;
const int MIN_FILAS = 0;
const int VIDAS_MAXIMAS = 3;
const int SIN_VIDAS = 0;
const int PUNTOS = 0;
const int COMPRAR_PUNTOS = 200;
const int SUMAR_VIDA = 1;
const int PIERDE_VIDA = 1;
const char ARRIBA = 'W';
const char ABAJO = 'S';
const char DERECHA = 'D';
const char IZQUIERDA = 'A';
const char MAC = 'M';
const char BLUE = 'B';
const char PUERTA = 'D';
const char ESCALERAS = 'E';
const char LLAVE = 'L';
const char MONEDAS = 'C';
const char POZOS = 'W';
const char INTERRUPTORES = 'O';
const char PORTALES = 'P';
const char VELAS = 'V';
const char VIDA = 'V';

const int ACCESO_NIVEL_1 = 0;
const int ACCESO_NIVEL_2 = 1;
const int ACCESO_NIVEL_3 = 2;
const int NIVEL_1 = 1;
const int NIVEL_2 = 2;
const int NIVEL_3 = 3;
const int DISTANCIA_PARA_GANAR = 1;
const int RESTAR_PUNTOS_VELA = 50;
const int JUGANDO = 0;
const int GANADO = 1;
const int PERDIDO = -1;


/*
*    Función que recibe dos coordenadas, devuelve true si las
* coordenadas son iguales, sino false.
*/
bool coordenadas_iguales(coordenada_t a, coordenada_t b){

    return ((a.fila == b.fila) && (a.col == b.col));

}

/*
*   Procedimiento que dejará la estructura personaje_t en un
* estado válido para iniciar el juego.
* 'arranque_personaje' es un parámetro que representa la posición 
* en la arrancará el personaje.
*/
void inicializar_personaje(personaje_t* ref_personaje, coordenada_t arranque_personaje){

    (*ref_personaje).vida = VIDAS_MAXIMAS;
    (*ref_personaje).puntos = PUNTOS;
    (*ref_personaje).tiene_llave = false;
    (*ref_personaje).interruptor_apretado = false;
    (*ref_personaje).posicion = arranque_personaje;
}


/*
*   Procedimiento que dejará una estructura elemento_t que
* represente a la sombra, en un estado válido para iniciar el juego. 
* 'arranque_sombra' es un parámetro que representa la posición 
* en la arrancará la sombra.
*/
void inicializar_sombra(sombra_t* ref_sombra, coordenada_t arranque_sombra){

    (*ref_sombra).posicion = arranque_sombra;
    (*ref_sombra).esta_viva = true;

}


/*
*   La función recibe un caracter y devuelve true
* si el movimiento recibido concuerda con la convención
* propuesta, sino false.
* -> W: Si el personaje debe moverse para la arriba. 
* -> A: Si el personaje debe moverse para la izquierda.
* -> S: Si el personaje debe moverse para la abajo.
* -> D: Si el personaje debe moverse para la derecha.
* -> V: Si el personaje puede comprar una vida.   
*/
bool es_movimiento_valido(char movimiento){
    
    return ((movimiento == ARRIBA) || (movimiento == ABAJO) || (movimiento == IZQUIERDA) || (movimiento == DERECHA) || (movimiento == VIDA));

}

/*
*   Procedimineto que se encargará de pedirle al usuario
* que ingrese un movimiento hasta que sea válido.
*/
void pedir_movimiento(char* ref_movimiento){
    
    *ref_movimiento = 'I';

    while ((es_movimiento_valido(*ref_movimiento)) == false){
        printf("Ingrese un movimiento válido: ");
        scanf(" %c", ref_movimiento);
    }
}


/*
*    La función recibe una coordenada, devuelve true
* si la posición dentro de las dimensiones dadas, sino false.
* Una posición nunca podrá tener un valor negativo y no tendrá un 
* valor más alto que los máximos.
*/
bool esta_dentro_rango(coordenada_t posicion, int max_alto, int max_ancho){

    return ((posicion.fila < max_alto) && (posicion.fila >= MIN_FILAS) && (posicion.col >= MIN_COLUMNAS) && (posicion.col < max_ancho));
}

/*
* La función recibe un paredes de elementos, su respectivo tope y una coordenada,
* devuelve true si la coordenada pasada coincide con la de una pared. 
*/
bool buscar_paredes(coordenada_t paredes[MAX_PAREDES], int tope, coordenada_t coordenada){

    int i = 0;
    int esta = false;

    while ((esta == false) && (i < tope)){
        if (paredes[i].col == coordenada.col && paredes[i].fila == coordenada.fila){
            esta = true;
        }
        else
        i++;
    }

    return esta;

}
/*
*    La función recibe un vector de elementos, su respectivo tope y una coordenada,
* devuelve la posición del elemento del vector, que coincida con la coordenada pasada 'posicion',
* si no se encuentra ningún elemento, se devolverá -1. 
*/
int buscar_elemento(elemento_t elementos[MAX_ELEMENTOS], int tope, coordenada_t posicion, char tipo){

    int i = 0;
    int esta = -1;

    while ((esta == -1) && (i < tope)){
        if ((elementos[i].coordenada.col == posicion.col) && (elementos[i].coordenada.fila == posicion.fila) && (elementos[i].tipo == tipo)){
            esta = i;
        }
        else
        i++;
    }

    return esta;
}



/*
* Comprueba si hay una pared en la posicion a chequear.
*/
bool hay_pared(juego_t* juego, coordenada_t posicion_a_chequear){
    bool esta = false;

    if ((buscar_paredes(juego->niveles[(juego->nivel_actual)-1].paredes, juego->niveles[(juego->nivel_actual)-1].tope_paredes, posicion_a_chequear))){
        esta = true;
	}
	else{
		esta = false;
	}
	return esta;
}

/*
* Comprueba si hay una escalera en la posicion a chequear.
*/
bool hay_escalera(juego_t* juego, coordenada_t posicion_a_chequear){
    bool esta = false;

    if ((buscar_elemento(juego->niveles[(juego->nivel_actual)-1].herramientas, juego->niveles[(juego->nivel_actual)-1].tope_herramientas,
	 posicion_a_chequear, ESCALERAS)) >= 0){
        esta = true;
	}
	else{
		esta = false;
	}
	return esta;
}



/*
*   Procedimiento que se encarga de mover el personaje hacia arriba si
*   esta dentro del rango y no hay pared.
*/
void mover_personaje_arriba(personaje_t* ref_personaje, juego_t* juego){

            coordenada_t aux_personaje = (*ref_personaje).posicion;
			aux_personaje.fila--;

			if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_personaje)))){
			    (*ref_personaje).posicion.fila--;
			}
			if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (hay_escalera(juego, aux_personaje))){
			    (*ref_personaje).posicion.fila--;
			}

}
/*
*   Procedimiento que mueve la asombra hacia arriba comprobando que este dentro del rango.
*/
void mover_sombra_arriba(sombra_t* ref_sombra, juego_t* juego){

	        coordenada_t aux_sombra = (*ref_sombra).posicion;
			aux_sombra.fila--;

			if (esta_dentro_rango(aux_sombra, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_sombra)))){
				(*ref_sombra).posicion.fila--;
			}
}

/*
*   Procedimiento que se encarga de mover el personaje hacia la izquierda si
*   esta dentro del rango y no hay pared.
*/
void mover_personaje_izquierda(personaje_t* ref_personaje, juego_t* juego){
            
            coordenada_t aux_personaje = (*ref_personaje).posicion;
			aux_personaje.col--;

			if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_personaje)))){
			    (*ref_personaje).posicion.col--;
			}
			if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (hay_escalera(juego, aux_personaje))){
			    (*ref_personaje).posicion.col--;
			}

}
/*
*   Procedimiento que mueve la asombra hacia la izquierda comprobando que este dentro del rango.
*/
void mover_sombra_izquierda(sombra_t* ref_sombra, juego_t* juego){

	        coordenada_t aux_sombra = (*ref_sombra).posicion;
			aux_sombra.col--;

			if (esta_dentro_rango(aux_sombra, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_sombra)))){
				(*ref_sombra).posicion.col--;
			}
}

/*
*   Procedimiento que se encarga de mover el personaje hacia abajo si
*   esta dentro del rango y no hay pared.
*/
void mover_personaje_abajo(personaje_t* ref_personaje, juego_t* juego){
            
			coordenada_t aux_personaje = (*ref_personaje).posicion;
			aux_personaje.fila++;

			if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_personaje)))){
			    (*ref_personaje).posicion.fila++;
			}
			if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (hay_escalera(juego, aux_personaje))){
			    (*ref_personaje).posicion.fila++;
			}
}
/*
*   Procedimiento que mueve la asombra hacia abajo comprobando que este dentro del rango.
*/
void mover_sombra_abajo(sombra_t* ref_sombra, juego_t* juego){

	        coordenada_t aux_sombra = (*ref_sombra).posicion;
			aux_sombra.fila++;

			if (esta_dentro_rango(aux_sombra, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_sombra)))){
				(*ref_sombra).posicion.fila++;
			}
}

/*
*   Procedimiento que se encarga de mover el personaje hacia la derecha si
*   esta dentro del rango y no hay pared.
*/
void mover_personaje_derecha(personaje_t* ref_personaje, juego_t* juego){

	        coordenada_t aux_personaje = (*ref_personaje).posicion;
			aux_personaje.col++;

			if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_personaje)))){
			    (*ref_personaje).posicion.col++;
			}
			if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (hay_escalera(juego, aux_personaje))){
			    (*ref_personaje).posicion.col++;
			}
}
/*
*   Procedimiento que se encarga de mover a la sombra hacia la derecha si esta dentro del rango
*/
void mover_sombra_derecha(sombra_t* ref_sombra, juego_t* juego){

	        coordenada_t aux_sombra = (*ref_sombra).posicion;
			aux_sombra.col++;

			if (esta_dentro_rango(aux_sombra, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_sombra)))){
				(*ref_sombra).posicion.col++;
			}
}

/*
* Comprueba si el personaje esta en la misma posicion que una herramienta.
*/
bool jugador_piso_herramientas(juego_t* juego, int* posicion_herramienta){
	bool piso = false;

	for (int i = 0; i < (juego->niveles[(juego->nivel_actual)-1].tope_herramientas); ++i){
		if((juego->niveles[(juego->nivel_actual)-1].herramientas[i].coordenada.fila == juego->personaje.posicion.fila) && (juego->niveles[(juego->nivel_actual)-1].herramientas[i].coordenada.col == juego->personaje.posicion.col)){
			piso = true;
			*posicion_herramienta = i;
		}
	}
	return piso;
}

/*
* Comprueba si el personaje esta en la misma posicion que un obstaculo.
*/
bool personaje_piso_obstaculos(juego_t* juego, int* posicion_obstaculo){
	bool piso = false;

	for (int i = 0; i < (juego->niveles[(juego->nivel_actual)-1].tope_obstaculos); ++i){
		if((juego->niveles[(juego->nivel_actual)-1].obstaculos[i].coordenada.fila == juego->personaje.posicion.fila) && (juego->niveles[(juego->nivel_actual)-1].obstaculos[i].coordenada.col == juego->personaje.posicion.col)){
			piso = true;
			*posicion_obstaculo = i;
		}
	}
	return piso;
}

/*
* Comprueba si la sombra esta en la misma posicion que una herramienta.
*/
bool sombra_piso_herramientas(juego_t* juego, int* posicion_herramienta){
	bool piso = false;

	for (int i = 0; i < (juego->niveles[(juego->nivel_actual)-1].tope_herramientas); ++i){
		if((juego->niveles[(juego->nivel_actual)-1].herramientas[i].coordenada.fila == juego->sombra.posicion.fila) && 
		(juego->niveles[(juego->nivel_actual)-1].herramientas[i].coordenada.col == juego->sombra.posicion.col)){
			piso = true;
			*posicion_herramienta = i;
		}
	}
	return piso;
}

/*
* Comprueba si la sombra esta en la misma posicion que un obstaculo.
*/
bool sombra_piso_obstaculos(juego_t* juego, int* posicion_obstaculo){
	bool piso = false;

	for (int i = 0; i < (juego->niveles[(juego->nivel_actual)-1].tope_obstaculos); ++i){
		if((juego->niveles[(juego->nivel_actual)-1].obstaculos[i].coordenada.fila == juego->sombra.posicion.fila) && 
		(juego->niveles[(juego->nivel_actual)-1].obstaculos[i].coordenada.col == juego->sombra.posicion.col)){
			piso = true;
			*posicion_obstaculo = i;
		}
	}
	return piso;
}

/*
* Procedimiento que elimina una herramienta del vector herramientas.
*/
void eliminar_herramienta(elemento_t herramientas[MAX_ELEMENTOS],int *tope_herramientas, int posicion_a_eliminar){
	for (int i = posicion_a_eliminar; i < (*tope_herramientas-1); ++i){
		herramientas[i] = herramientas[i+1];
	}
	(*tope_herramientas)--;
}

/*
* Procedimiento que elimina un obstaculo del vector obstaculos.
*/
void eliminar_obstaculo(elemento_t obstaculos[MAX_ELEMENTOS],int *tope_obstaculos, int posicion_a_eliminar){
	for (int i = posicion_a_eliminar; i < (*tope_obstaculos-1); ++i){
		obstaculos[i] = obstaculos[i+1];
	}
	(*tope_obstaculos)--;
}

/*
* Procedimiento que se encarga de efectuar el movimiento hacia la derecha en caso de haber pisado una escalera.
*/
void mover_derecha_en_escalera(personaje_t* ref_personaje, char movimiento, juego_t* juego, int posicion_vector){
	coordenada_t aux_personaje = (*ref_personaje).posicion;
	aux_personaje.col++;

	if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_personaje)))){
		(*ref_personaje).posicion.col++;
		eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), posicion_vector);
	}
	if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (hay_escalera(juego, aux_personaje))){
		(*ref_personaje).posicion.col++;
		aux_personaje.col++;
		eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), posicion_vector);
		if(esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_personaje)))){
			int nueva_escalera = buscar_elemento(juego->niveles[(juego->nivel_actual)-1].herramientas, (juego->niveles[(juego->nivel_actual)-1].tope_herramientas),
			(*ref_personaje).posicion, ESCALERAS);
			eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), nueva_escalera);
			(*ref_personaje).posicion.col++;
		}
	}
}

/*
* Procedimiento que se encarga de efectuar el movimiento hacia la izquierda en caso de haber pisado una escalera.
*/
void mover_izquierda_en_escalera(personaje_t* ref_personaje, char movimiento, juego_t* juego, int posicion_vector){
	coordenada_t aux_personaje = (*ref_personaje).posicion;
	aux_personaje.col--;

	if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_personaje)))){
		(*ref_personaje).posicion.col--;
		eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), posicion_vector);
	}
	if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (hay_escalera(juego, aux_personaje))){
		(*ref_personaje).posicion.col--;
		aux_personaje.col--;
		eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), posicion_vector);
		if(esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_personaje)))){
			int nueva_escalera = buscar_elemento(juego->niveles[(juego->nivel_actual)-1].herramientas, (juego->niveles[(juego->nivel_actual)-1].tope_herramientas),
			(*ref_personaje).posicion, ESCALERAS);
			eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), nueva_escalera);
			(*ref_personaje).posicion.col--;
		}
	}
}

/*
* Procedimiento que se encarga de efectuar el movimiento hacia abajo en caso de haber pisado una escalera.
*/
void mover_abajo_en_escalera(personaje_t* ref_personaje, char movimiento, juego_t* juego, int posicion_vector){
	coordenada_t aux_personaje = (*ref_personaje).posicion;
	aux_personaje.fila++;

	if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_personaje)))){
		(*ref_personaje).posicion.fila++;
		eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), posicion_vector);
	}
	if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (hay_escalera(juego, aux_personaje))){
		(*ref_personaje).posicion.fila++;
		aux_personaje.fila++;
		eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), posicion_vector);
		if(esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_personaje)))){
			int nueva_escalera = buscar_elemento(juego->niveles[(juego->nivel_actual)-1].herramientas, (juego->niveles[(juego->nivel_actual)-1].tope_herramientas),
			(*ref_personaje).posicion, ESCALERAS);
			eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), nueva_escalera);
			(*ref_personaje).posicion.fila++;
		}
	}
}

/*
* Procedimiento que se encarga de efectuar el movimiento hacia arriba en caso de haber pisado una escalera.
*/
void mover_arriba_en_escalera(personaje_t* ref_personaje, char movimiento, juego_t* juego, int posicion_vector){
	coordenada_t aux_personaje = (*ref_personaje).posicion;
	aux_personaje.fila--;

	if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_personaje)))){
		(*ref_personaje).posicion.fila--;
		eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), posicion_vector);
	}
	if (esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (hay_escalera(juego, aux_personaje))){
		(*ref_personaje).posicion.fila--;
		aux_personaje.fila--;
		eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), posicion_vector);
		while(esta_dentro_rango(aux_personaje, MAX_FILAS, MAX_COLUMNAS) && (!(hay_pared(juego, aux_personaje)))){
			int nueva_escalera = buscar_elemento(juego->niveles[(juego->nivel_actual)-1].herramientas, (juego->niveles[(juego->nivel_actual)-1].tope_herramientas),
			(*ref_personaje).posicion, ESCALERAS);
			eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), nueva_escalera);
			(*ref_personaje).posicion.fila--;
		}
	}
}

/*
* Procedimiento que hace accionar el movimiento del personaje de acuerdo a si pisa una escalera
*/
void movimiento_escalera(personaje_t* ref_personaje, char movimiento, juego_t* juego, int posicion_vector){
    
	if (movimiento == ARRIBA){
		mover_arriba_en_escalera(ref_personaje, movimiento, juego, posicion_vector);
	}
	if (movimiento == DERECHA){
		mover_derecha_en_escalera(ref_personaje, movimiento, juego, posicion_vector);
	}
	if (movimiento == ABAJO){
		mover_abajo_en_escalera(ref_personaje, movimiento, juego, posicion_vector);
	}
	if (movimiento == IZQUIERDA){
		mover_izquierda_en_escalera(ref_personaje, movimiento, juego, posicion_vector);
	}
}




/*
*  Funcion que comprueba si la sombra entro en la matriz de 3x3 correspondiente a una vela.
*/
bool esta_en_matriz_vela(juego_t* juego){
    bool entro = false;
	
	for (int i=0; i < (juego->niveles[(juego->nivel_actual)-1].tope_obstaculos); i++){
        if (juego->niveles[(juego->nivel_actual)-1].obstaculos[i].tipo == VELAS){
			if ((abs((juego->niveles[juego->nivel_actual-1].obstaculos[(i)].coordenada.fila) - (juego->sombra.posicion.fila)) <= 1)
	&& (abs((juego->niveles[juego->nivel_actual-1].obstaculos[(i)].coordenada.col) - (juego->sombra.posicion.col)) <= 1)){
		        entro = true;
	        }
			else{
			}
		}
	}
	return entro;
}



/*
*   Función que dados una altura máxima y un ancho máximo
* devuelve una coordenada aleatoria. Una posición nunca podrá
* tener un valor negativo, ni ser mayor a sus máximos.
*/
coordenada_t coordenada_aleatoria(int max_alto, int max_ancho){

    coordenada_t coordenada_random;
    coordenada_random.fila = rand() % max_alto;
    coordenada_random.col = rand() % max_ancho;
    return coordenada_random;

}

/*
*   Funcion que dada la posicion del personaje Mac devuelve la posicion de Blue al inicio
*   del juego en funcion de este
*
*/
coordenada_t crear_coordenada_blue(coordenada_t coordenada_mac){

    coordenada_t coordenada_inicio_blue;
    coordenada_inicio_blue.fila = coordenada_mac.fila;
    coordenada_inicio_blue.col = ((MAX_COLUMNAS-1) - coordenada_mac.col);
    
    return coordenada_inicio_blue;
}

/*
*   Procedimiento que dado el juego, personaje, sombra (por referencia) hace y llama a las acciones 
*   necesarias en caso de que pise una escalera el personaje.
*
*/
void piso_escalera(juego_t* juego, personaje_t* ref_personaje, char movimiento){
    int posicion = -1;

	while (jugador_piso_herramientas(juego, &(posicion))){
		if (juego->niveles[(juego->nivel_actual)-1].herramientas[(posicion)].tipo == ESCALERAS){
			movimiento_escalera(ref_personaje, movimiento, juego, posicion);
		}
	}
}

/*
*   Procedimiento que dado el juego, personaje, sombra (por referencia) hace las acciones necesarias en caso de que pise una moneda el personaje/sombra.
*
*/
void piso_monedas(juego_t* juego, personaje_t* ref_personaje, sombra_t* ref_sombra){
    int posicion = -1;
	if (jugador_piso_herramientas(juego, &(posicion))){
        if (juego->niveles[(juego->nivel_actual)-1].herramientas[(posicion)].tipo == MONEDAS){
			int puntos_aleatorios = rand() % 11 + 10;
			ref_personaje->puntos += puntos_aleatorios;
		    eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), posicion);
		}
	}
	if (sombra_piso_herramientas(juego, &(posicion))){
        if (juego->niveles[(juego->nivel_actual)-1].herramientas[(posicion)].tipo == MONEDAS){
			int puntos_aleatorios = rand() % 11 + 10;
			ref_personaje->puntos += puntos_aleatorios;
		    eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), posicion);
		}
	}
}

/*
*   Procedimiento que dado el juego, personaje, sombra (por referencia) hace las acciones necesarias en caso de que pise una llave el personaje.
*
*/
void piso_llave(juego_t* juego, personaje_t* ref_personaje){
    int posicion = -1;
    if (jugador_piso_herramientas(juego, &(posicion))){
	    if (juego->niveles[(juego->nivel_actual)-1].herramientas[(posicion)].tipo == LLAVE){
			ref_personaje->tiene_llave = true;
		    eliminar_herramienta(juego->niveles[(juego->nivel_actual)-1].herramientas, &(juego->niveles[(juego->nivel_actual)-1].tope_herramientas), posicion);
		}
    }
}

/*
*   Procedimiento que dado el juego, personaje, sombra (por referencia) hace las acciones necesarias en caso de que pise una vela el personaje.
*
*/
void piso_vela(juego_t* juego, personaje_t* ref_personaje, sombra_t* ref_sombra){
	int posicion = -1;
	if (personaje_piso_obstaculos(juego, &(posicion))){
		if (juego->niveles[(juego->nivel_actual)-1].obstaculos[(posicion)].tipo == VELAS){
			eliminar_obstaculo(juego->niveles[(juego->nivel_actual)-1].obstaculos, &(juego->niveles[(juego->nivel_actual)-1].tope_obstaculos), posicion);
		}
	}
}

/*
*   Procedimiento que dado el juego, personaje, sombra (por referencia) hace las acciones necesarias en caso de que pise un pozo el personaje.
*
*/
void piso_pozo(juego_t* juego, personaje_t* ref_personaje, sombra_t* ref_sombra){
	int posicion = -1;
	coordenada_t coordenada_vieja;
	if (personaje_piso_obstaculos(juego, &(posicion))){
        if (juego->niveles[(juego->nivel_actual)-1].obstaculos[(posicion)].tipo == POZOS){
			ref_personaje->vida -= PIERDE_VIDA;
			coordenada_vieja = ref_personaje->posicion;
			ref_personaje->posicion = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);

			while (coordenadas_iguales(ref_personaje->posicion, coordenada_vieja)){
				ref_personaje->posicion = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
			}
			ref_sombra->posicion = crear_coordenada_blue(ref_personaje->posicion);

		}
	}
}

/*
*   Procedimiento que dado el juego, personaje, sombra (por referencia) hace las acciones necesarias en caso de que pise un interruptor el personaje.
*
*/
void piso_interruptor(juego_t* juego, personaje_t* ref_personaje){
	int posicion = -1;
    if (personaje_piso_obstaculos(juego, &(posicion))){
        if (juego->niveles[(juego->nivel_actual)-1].obstaculos[(posicion)].tipo == INTERRUPTORES){
			if (ref_personaje->interruptor_apretado == true){
				ref_personaje->interruptor_apretado = false;
			}
			else{
				ref_personaje->interruptor_apretado = true;
			}
		}
	} 
}

/*
*   Procedimiento que dado el juego, personaje, sombra (por referencia) hace las acciones necesarias en caso de que pise un portal la sombra.
*
*/
void piso_portal(juego_t* juego, personaje_t* ref_personaje, sombra_t* ref_sombra){
	int posicion = -1;
    if (sombra_piso_obstaculos(juego, &(posicion))){
		if (juego->niveles[(juego->nivel_actual)-1].obstaculos[(posicion)].tipo == PORTALES){
			coordenada_t posicion_aux;
			posicion_aux = ref_personaje->posicion;
			ref_personaje->posicion = ref_sombra->posicion;
			ref_sombra->posicion = posicion_aux;
			eliminar_obstaculo(juego->niveles[(juego->nivel_actual)-1].obstaculos, &(juego->niveles[(juego->nivel_actual)-1].tope_obstaculos), posicion);
		}
	}
}

/*
*   Procedimiento que dado el juego, personaje, sombra (por referencia) y el movimiento llama a las herramientas para accionarlas.
*
*/
void accionar_herramientas(juego_t* juego, personaje_t* ref_personaje, sombra_t* ref_sombra, char movimiento){
	piso_escalera(juego, ref_personaje, movimiento);
	piso_monedas(juego, ref_personaje, ref_sombra);
	piso_llave(juego, ref_personaje);
}

/*
*   Procedimiento que dado el juego, personaje, sombra (por referencia) llama a los obstaculos para accionarlos.
*
*/
void accionar_obstaculos(juego_t* juego, personaje_t* ref_personaje, sombra_t* ref_sombra){
	piso_pozo(juego, ref_personaje, ref_sombra);
	piso_vela(juego, ref_personaje, ref_sombra);
	piso_interruptor(juego, ref_personaje);
	piso_portal(juego, ref_personaje, ref_sombra);
}

/*
*   Procedimiento que dado el juego, personaje, sombra (por referencia) y movimiento mueve ambos de forma coordinada.
*
*/
void mover_espejados(juego_t* juego, personaje_t* ref_personaje, sombra_t* ref_sombra, char movimiento){

	if (movimiento == ARRIBA){
		mover_personaje_arriba(ref_personaje, juego);
		mover_sombra_arriba(ref_sombra, juego);
	}
	if (movimiento == IZQUIERDA){
	    mover_personaje_izquierda(ref_personaje, juego);
		mover_sombra_derecha(ref_sombra, juego);		    
	}
	if (movimiento == ABAJO){
	    mover_personaje_abajo(ref_personaje, juego);
		mover_sombra_abajo(ref_sombra, juego);	        
	}
	if (movimiento == DERECHA){
		mover_personaje_derecha(ref_personaje, juego);
		mover_sombra_izquierda(ref_sombra, juego);		   
	}
}

/*
*   Procedimiento que dado el juego, personaje, sombra (por referencia) y movimiento mueve ambos de forma espejada en sentido horizontal.
*
*/
void mover_coordinados(juego_t* juego, personaje_t* ref_personaje, sombra_t* ref_sombra, char movimiento){

	if (movimiento == ARRIBA){
		mover_personaje_arriba(ref_personaje, juego);
		mover_sombra_arriba(ref_sombra, juego);
	}
	if (movimiento == IZQUIERDA){
		mover_personaje_izquierda(ref_personaje, juego);
		mover_sombra_izquierda(ref_sombra, juego);		    
	}
	if (movimiento == ABAJO){
		mover_personaje_abajo(ref_personaje, juego);
		mover_sombra_abajo(ref_sombra, juego);	        
	}
	if (movimiento == DERECHA){
		mover_personaje_derecha(ref_personaje, juego);
		mover_sombra_derecha(ref_sombra, juego);		   
	}
}
/*
*   Procedimiento que dado el juego, movimiento y personaje por referencia mueve unicamente a este.
*
*/
void mover_solo_personaje(juego_t* juego, personaje_t* ref_personaje, char movimiento){

	if (movimiento == ARRIBA){
		mover_personaje_arriba(ref_personaje, juego);
	}
	if (movimiento == IZQUIERDA){
		mover_personaje_izquierda(ref_personaje, juego);		    
	}
	if (movimiento == ABAJO){
		mover_personaje_abajo(ref_personaje, juego);       
	}
	if (movimiento == DERECHA){
		mover_personaje_derecha(ref_personaje, juego);	   
	}
}

/*
*   Procedimiento que dado el personaje por referncia le aumenta una vida dadas las condiciones.
*/
void comprar_vida(personaje_t* ref_personaje){
	if ((ref_personaje->vida > SIN_VIDAS) && (ref_personaje->vida < VIDAS_MAXIMAS) && (ref_personaje->puntos >= COMPRAR_PUNTOS)){
		ref_personaje->puntos -= COMPRAR_PUNTOS;
		ref_personaje->vida += SUMAR_VIDA;
	}
}
/*
*    El procedimiento recibe el juego, personaje y la sombra por referencia y el movimiento,
* y según este último junto a las condiciones de los personajes, los mueve acordemente y realiza las acciones necesarias
* que repercutan en el personaje y la sombra en caso de encontrarse de con un obstaculo, herramienta o solicitar una vida. 
*/
void mover_personaje(juego_t* juego, personaje_t* ref_personaje, sombra_t* ref_sombra, char movimiento){
    
    if (es_movimiento_valido(movimiento)){

        if ((ref_personaje->interruptor_apretado == false) && (ref_sombra->esta_viva == true)){	
			mover_espejados(juego, ref_personaje, ref_sombra, movimiento);	
		}

        if (ref_personaje->interruptor_apretado == true && (ref_sombra->esta_viva == true)){
			mover_coordinados(juego, ref_personaje, ref_sombra, movimiento);
		}
        if (ref_sombra->esta_viva == false){
			mover_solo_personaje(juego, ref_personaje, movimiento);
		}
		if (movimiento == VIDA){
			comprar_vida(ref_personaje);
		}
		accionar_herramientas(juego, (&(juego->personaje)), (&(juego->sombra)), (movimiento));
	    accionar_obstaculos(juego, (&(juego->personaje)), (&(juego->sombra)));

		if (esta_en_matriz_vela(juego)){
			ref_sombra->esta_viva = false;
		}
		if (coordenadas_iguales(ref_personaje->posicion, ref_sombra->posicion) && (!(esta_en_matriz_vela(juego)))){
			ref_sombra->esta_viva = true;
			ref_personaje->puntos -= RESTAR_PUNTOS_VELA;			
		}
		if (coordenadas_iguales(ref_personaje->posicion, ref_sombra->posicion) && (esta_en_matriz_vela(juego))){
			ref_personaje->puntos -= RESTAR_PUNTOS_VELA;
		}
	}
}
/*
* Busca un elemento dentro de un vector de elementos de acuerdo a su tipo y devuelve su posicion.
*/
int buscar_elemento_por_tipo(elemento_t elementos[MAX_ELEMENTOS], int tope, char tipo){

    int i = 0;
    int esta = -1;

    while ((esta == -1) && (i < tope)){
        if (elementos[i].tipo == tipo){
            esta = i;
        }
        else
        i++;
    }

    return esta;

}


/*
* Carga los obstaculos de acuerdo al nivel requerido.
*/
void cargar_obstaculos_nivel(juego_t* juego, int cant_velas, int cant_pozos, int cant_interruptores, int cant_portales){
    
    for (int i = 0; i < cant_velas; i++){
		(*juego).niveles[juego->nivel_actual-1].obstaculos[i].tipo = VELAS;
	}
	for (int i = cant_velas; i < cant_pozos + cant_velas; i++){
		(*juego).niveles[juego->nivel_actual-1].obstaculos[i].tipo = POZOS;
	}
	for (int i = cant_pozos+ cant_velas; i < cant_interruptores + cant_pozos + cant_velas; i++){
		(*juego).niveles[juego->nivel_actual-1].obstaculos[i].tipo = INTERRUPTORES;
	}
	for (int i = cant_interruptores + cant_pozos + cant_velas; i < cant_portales + cant_interruptores + cant_pozos + cant_velas; i++){
		(*juego).niveles[juego->nivel_actual-1].obstaculos[i].tipo = PORTALES;
	}
}

/*
* Carga las herramientas de acuerdo al nivel requerido.
*/
void cargar_herramientas_nivel(juego_t* juego, int cant_escaleras, int cant_monedas, int cant_llaves, int cant_puertas){
    
    for (int i = 0; i < cant_escaleras; i++){
		(*juego).niveles[juego->nivel_actual-1].herramientas[i].tipo = ESCALERAS;
	}
	for (int i = cant_escaleras; i < cant_monedas + cant_escaleras; i++){
		(*juego).niveles[juego->nivel_actual-1].herramientas[i].tipo = MONEDAS;
	}
	for (int i = cant_monedas+ cant_escaleras; i < cant_llaves + cant_monedas + cant_escaleras; i++){
		(*juego).niveles[juego->nivel_actual-1].herramientas[i].tipo = LLAVE;
	}
	for (int i = cant_llaves + cant_monedas + cant_escaleras; i < cant_puertas + cant_llaves + cant_monedas + cant_escaleras; i++){
		(*juego).niveles[juego->nivel_actual-1].herramientas[i].tipo = PUERTA;
	}
}

/*
* Comprueba si una posicion esta ocupada en la matriz.
*/
bool esta_pos_ocupada(coordenada_t coordenadas_ocupadas[MAX_ELEMENTOS], int tope_coord_ocupadas, coordenada_t* coordenada_elemento){
    int i=0;
    bool esta_ocupada = false;
   
	while((i<tope_coord_ocupadas) && !(esta_ocupada)){
        if (coordenadas_iguales(coordenadas_ocupadas[i], (*coordenada_elemento)) == true){
			esta_ocupada = true;
		}
	    i++;
	}
	return esta_ocupada;
}


/*
* Recibe los tres vectores de herramientas, obstaculos y paredes y les asigna una posicion de tal manera que no se superpongan entre si.
*/
void posicionar_elementos(elemento_t vector_herramientas[MAX_ELEMENTOS], elemento_t vector_obstaculos[MAX_ELEMENTOS], coordenada_t vector_paredes[MAX_PAREDES],
 int tope_herramientas, int tope_obstaculos, int tope_paredes){

    
	coordenada_t nueva_coordenada;
    
	coordenada_t coordenadas_ocupadas[MAX_ELEMENTOS];

	int tope = 0;


    for (int i=0; i < tope_paredes; i++){
		coordenadas_ocupadas[tope] = vector_paredes[i];
		tope++;
	}

	for (int i = 0; i < tope_herramientas; i++){
        nueva_coordenada = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);


        while(esta_pos_ocupada(coordenadas_ocupadas, tope, (&(nueva_coordenada)))){
            nueva_coordenada = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
	    }
        (vector_herramientas)[i].coordenada.fila = nueva_coordenada.fila;
		(vector_herramientas)[i].coordenada.col = nueva_coordenada.col;

        coordenadas_ocupadas[tope] = (vector_herramientas)[i].coordenada;

        tope++;
	}

	


	for (int i = 0; i < tope_obstaculos; i++){
        nueva_coordenada = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);


        while(esta_pos_ocupada(coordenadas_ocupadas, tope, (&(nueva_coordenada)))){
            nueva_coordenada = coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS);
	    }

        (vector_obstaculos)[i].coordenada.fila = nueva_coordenada.fila;
		(vector_obstaculos)[i].coordenada.col = nueva_coordenada.col;

        coordenadas_ocupadas[tope] = (vector_obstaculos)[i].coordenada;

        tope++;
	}

}

/*
* Recibe el vector de herramientas y paredes las posiciona a las escaleras en funcion de las paredes
*/
void posicionar_escaleras(elemento_t vector_herramientas[MAX_ELEMENTOS], int tope_herramientas, coordenada_t vector_paredes[MAX_PAREDES],
 int tope_paredes, juego_t* juego){
    
    int i = 0;
    coordenada_t coordenadas_ocupadas[MAX_ELEMENTOS];
	int tope = 0;
	while ((i < tope_herramientas) && (vector_herramientas[i].tipo == ESCALERAS)){
		int num_random = rand() % tope_paredes;

        if(!esta_pos_ocupada(coordenadas_ocupadas, tope, (&(vector_herramientas[i].coordenada))) && (!(hay_escalera(juego, vector_paredes[num_random])))){
			vector_herramientas[i].coordenada.fila = vector_paredes[num_random].fila;
		    vector_herramientas[i].coordenada.col = vector_paredes[num_random].col;
			coordenadas_ocupadas[tope] = vector_herramientas[i].coordenada;
            tope++;
			i++;
		}
	}
}

/*
* Calcula la distancia (manhattan) entre dos coordenadas
*/
int calcular_distancia(coordenada_t posicion_personaje_o_sombra,coordenada_t posicion_puerta){
	int suma_filas=0;
	int suma_columnas=0;
	suma_filas = abs(posicion_puerta.fila-posicion_personaje_o_sombra.fila);
	suma_columnas = abs(posicion_puerta.col-posicion_personaje_o_sombra.col);
	return (suma_columnas+suma_filas);
}
/*
* Comprueba si dos coordenadas estan a distancia manhattan
*/
bool esta_distancia_manhattan(coordenada_t posicion_personaje_o_sombra, coordenada_t posicion_puerta){
	return (calcular_distancia(posicion_personaje_o_sombra, posicion_puerta)<=DISTANCIA_PARA_GANAR);
}

/*
* Inicializa un nivel de manera general, pasandole como parametros el juego, los obstaculos, las herramientas y un numero de acceso al
indice del vector niveles
*/
void inicializar_nivel(juego_t* juego, int nivel, int cantidad_velas, int cantidad_pozos, int cantidad_portales, int cantidad_interruptores, int cantidad_escaleras,
int cantidad_llaves, int cantidad_monedas, int cantidad_puertas){
    
	juego->nivel_actual = nivel;
	obtener_mapa((juego->niveles[juego->nivel_actual-1].paredes), &(juego->niveles[juego->nivel_actual-1].tope_paredes));
    juego->niveles[juego->nivel_actual-1].numero_nivel = nivel;
	
    juego->niveles[juego->nivel_actual-1].tope_obstaculos = cantidad_velas + cantidad_pozos + cantidad_portales + cantidad_interruptores;
    juego->niveles[juego->nivel_actual-1].tope_herramientas = cantidad_escaleras + cantidad_llaves + cantidad_monedas + cantidad_puertas;

	cargar_herramientas_nivel(juego, cantidad_escaleras, cantidad_monedas, cantidad_llaves, cantidad_puertas);
	cargar_obstaculos_nivel(juego, cantidad_velas, cantidad_pozos, cantidad_interruptores, cantidad_portales);

    posicionar_elementos(juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].obstaculos, juego->niveles[juego->nivel_actual-1].paredes, 
	juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->niveles[juego->nivel_actual-1].tope_obstaculos, juego->niveles[juego->nivel_actual-1].tope_paredes);
    
	posicionar_escaleras(juego->niveles[juego->nivel_actual-1].herramientas, juego->niveles[juego->nivel_actual-1].tope_herramientas, juego->niveles[juego->nivel_actual-1].paredes, 
	juego->niveles[juego->nivel_actual-1].tope_paredes, juego);

    inicializar_personaje(&(juego->personaje), coordenada_aleatoria(MAX_FILAS, MAX_COLUMNAS));
    inicializar_sombra(&(juego->sombra), crear_coordenada_blue(juego->personaje.posicion));
}

/* 
 *Inicializará el juego, cargando toda la información inicial, los datos del personaje, y los 3 niveles.
 */
void inicializar_juego(juego_t* juego, objeto_t elementos[MAX_ELEMENTOS]){
    
    cargar_mapas();
    int tope_elementos = 21;
	int cantidad_velas1 = 0;
	int cantidad_velas2 = 0;
	int cantidad_velas3 = 0;
	int cantidad_pozos1 = 0;
	int cantidad_pozos2 = 0;
	int cantidad_pozos3 = 0;
	int cantidad_portales1 = 0;
	int cantidad_portales2 = 0;
	int cantidad_portales3 = 0;
	int cantidad_interruptores1 = 0;
	int cantidad_interruptores2 = 0;
	int cantidad_interruptores3 = 0;
	int cantidad_escaleras1 = 0;
	int cantidad_escaleras2 = 0;
	int cantidad_escaleras3 = 0;
	int cantidad_llaves1 = 0;
	int cantidad_llaves2 = 0;
	int cantidad_llaves3 = 0;
	int cantidad_monedas1 = 0;
	int cantidad_monedas2 = 0;
	int cantidad_monedas3 = 0;

	for (int i = 0; i < tope_elementos; i++){
		if (elementos[i].numero_nivel == NIVEL_1){
			if (strcmp(elementos[i].tipo, "LLAVES") == 0){
				cantidad_llaves1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "POZOS") == 0){
				cantidad_pozos1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "INTERRUPTORES") == 0){
				cantidad_interruptores1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "MONEDAS") == 0){
				cantidad_monedas1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "PORTALES") == 0){
				cantidad_portales1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "VELAS") == 0){
				cantidad_velas1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "ESCALERAS") == 0){
				cantidad_escaleras1 = elementos[i].cantidad;
			}
		}
		if (elementos[i].numero_nivel == NIVEL_2){
			if (strcmp(elementos[i].tipo, "LLAVES") == 0){
				cantidad_llaves1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "POZOS") == 0){
				cantidad_pozos1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "INTERRUPTORES") == 0){
				cantidad_interruptores1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "MONEDAS") == 0){
				cantidad_monedas1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "PORTALES") == 0){
				cantidad_portales1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "VELAS") == 0){
				cantidad_velas1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "ESCALERAS") == 0){
				cantidad_escaleras1 = elementos[i].cantidad;
			}
		}
		if (elementos[i].numero_nivel == NIVEL_3){
			if (strcmp(elementos[i].tipo, "LLAVES") == 0){
				cantidad_llaves1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "POZOS") == 0){
				cantidad_pozos1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "INTERRUPTORES") == 0){
				cantidad_interruptores1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "MONEDAS") == 0){
				cantidad_monedas1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "PORTALES") == 0){
				cantidad_portales1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "VELAS") == 0){
				cantidad_velas1 = elementos[i].cantidad;
			}
			if (strcmp(elementos[i].tipo, "ESCALERAS") == 0){
				cantidad_escaleras1 = elementos[i].cantidad;
			}
		}
		else{
			cantidad_velas1 = CANTIDAD_VELAS_NIVEL_1;
			cantidad_velas2 = CANTIDAD_VELAS_NIVEL_2;
			cantidad_velas3 = CANTIDAD_VELAS_NIVEL_3;
			cantidad_pozos1 = CANTIDAD_POZOS_NIVEL_1;
			cantidad_pozos2 = CANTIDAD_POZOS_NIVEL_2;
			cantidad_pozos3 = CANTIDAD_POZOS_NIVEL_3;
			cantidad_portales1 = CANTIDAD_PORTALES_NIVEL_1;
			cantidad_portales2 = CANTIDAD_PORTALES_NIVEL_2;
			cantidad_portales3 = CANTIDAD_PORTALES_NIVEL_3;
			cantidad_interruptores1 = CANTIDAD_INTERRUPTORES_NIVEL_1;
			cantidad_interruptores2 = CANTIDAD_INTERRUPTORES_NIVEL_2;
			cantidad_interruptores3 = CANTIDAD_INTERRUPTORES_NIVEL_3;
			cantidad_escaleras1 = CANTIDAD_ESCALERAS_NIVEL_1;
			cantidad_escaleras2 = CANTIDAD_ESCALERAS_NIVEL_2;
			cantidad_escaleras3 = CANTIDAD_ESCALERAS_NIVEL_3;
			cantidad_llaves1 = CANTIDAD_LLAVES_NIVEL_1;
			cantidad_llaves2 = CANTIDAD_LLAVES_NIVEL_2;
			cantidad_llaves3 = CANTIDAD_LLAVES_NIVEL_3;
			cantidad_monedas1 = CANTIDAD_MONEDAS_NIVEL_1;
			cantidad_monedas2 = CANTIDAD_MONEDAS_NIVEL_2;
			cantidad_monedas3 = CANTIDAD_MONEDAS_NIVEL_3;
		}

	}
	inicializar_nivel(juego, NIVEL_1, cantidad_velas1, cantidad_pozos1, cantidad_portales1, cantidad_interruptores1,
	cantidad_escaleras1, cantidad_llaves1, cantidad_monedas1, CANTIDAD_PUERTAS_NIVEL_1);
    inicializar_nivel(juego, NIVEL_2, cantidad_velas2, cantidad_pozos2, cantidad_portales2, cantidad_interruptores2,
	cantidad_escaleras2, cantidad_llaves2, cantidad_monedas2, CANTIDAD_PUERTAS_NIVEL_2);
	inicializar_nivel(juego, NIVEL_3, cantidad_velas3, cantidad_pozos3, cantidad_portales3, cantidad_interruptores3,
	cantidad_escaleras3, cantidad_llaves3, cantidad_monedas3, CANTIDAD_PUERTAS_NIVEL_3);
	juego->nivel_actual = NIVEL_1;
	
}

/*
 * Recibe un juego con todas sus estructuras válidas.
 *
 * El juego se dará por ganado, si terminó todos los niveles. O perdido, si el personaje queda
 * sin vida. 
 * Devolverá:
 * -> 0 si el estado es jugando. 
 * -> -1 si el estado es perdido.
 * -> 1 si el estado es ganado.
 */
int estado_juego(juego_t juego){

    int estado = JUGANDO;
    int puerta = buscar_elemento_por_tipo(juego.niveles[juego.nivel_actual-1].herramientas, juego.niveles[juego.nivel_actual-1].tope_herramientas, PUERTA);

	if ((juego.nivel_actual == NIVEL_3) && (esta_distancia_manhattan(juego.personaje.posicion, juego.niveles[juego.nivel_actual-1].herramientas[puerta].coordenada)) && 
	esta_distancia_manhattan(juego.sombra.posicion, juego.niveles[juego.nivel_actual-1].herramientas[puerta].coordenada)){
        estado = GANADO;
	}
    if (juego.personaje.vida == SIN_VIDAS){
		estado = PERDIDO;
	}
	else{
		estado = JUGANDO;
	}

	return estado;
	
}

/*
 * El nivel se dará por terminado, si ambos personajes pasaron por la puerta teniendo la 
 * llave correspondiente.
 * Devolverá:
 * -> 0 si el estado es jugando.
 * -> 1 si el estado es ganado.
 */
int estado_nivel(juego_t juego){

    int estado = 0;
    int puerta = buscar_elemento_por_tipo(juego.niveles[juego.nivel_actual-1].herramientas, juego.niveles[juego.nivel_actual-1].tope_herramientas,  PUERTA);

	if (juego.nivel_actual == NIVEL_1){
	    if ((esta_distancia_manhattan(juego.personaje.posicion, juego.niveles[juego.nivel_actual-1].herramientas[puerta].coordenada)) && 
	    (esta_distancia_manhattan(juego.sombra.posicion, juego.niveles[juego.nivel_actual-1].herramientas[puerta].coordenada))
		&& (juego.sombra.esta_viva == true)){
            estado = GANADO;
		}
	}
	if (juego.nivel_actual == NIVEL_2 || juego.nivel_actual == NIVEL_2){
		if ((juego.personaje.tiene_llave == true) && (esta_distancia_manhattan(juego.personaje.posicion, juego.niveles[juego.nivel_actual-1].herramientas[puerta].coordenada))
		&&  (esta_distancia_manhattan(juego.sombra.posicion, juego.niveles[juego.nivel_actual-1].herramientas[puerta].coordenada))
		&& (juego.sombra.esta_viva == true)){
            estado = GANADO;
		}
	}
	else{
		estado = JUGANDO;
	}

	return estado;
	
}

/*
 * Moverá el personaje, y realizará la acción necesaria en caso de chocar con un elemento
 */
void realizar_jugada(juego_t* juego){
    char movimiento;
	pedir_movimiento(&(movimiento));
	mover_personaje(juego, (&(juego->personaje)), (&(juego->sombra)), (movimiento));
    if (estado_nivel((*juego)) == 1){
		juego->nivel_actual++;
    }
}


/*
*    Procedimiento que recibe el juego e imprime 
* toda su información por pantalla.
*/
void imprimir_terreno(juego_t juego){

    char matriz[MAX_FILAS][MAX_COLUMNAS];
    for (int i = 0; i < MAX_FILAS; i++){
        for (int j = 0; j < MAX_COLUMNAS; j++){
			matriz[i][j] = '.';
    }
    }

	for (int i=0; i < juego.niveles[juego.nivel_actual-1].tope_obstaculos; i++){
        matriz[juego.niveles[juego.nivel_actual-1].obstaculos[i].coordenada.fila][juego.niveles[juego.nivel_actual-1].obstaculos[i].coordenada.col] = 
		juego.niveles[juego.nivel_actual-1].obstaculos[i].tipo;
	}
	for (int i=0; i < juego.niveles[juego.nivel_actual-1].tope_paredes; i++){
        matriz[juego.niveles[juego.nivel_actual-1].paredes[i].fila][juego.niveles[juego.nivel_actual-1].paredes[i].col] = '#';
	}

	for (int i=0; i < juego.niveles[juego.nivel_actual-1].tope_herramientas; i++){
        matriz[juego.niveles[juego.nivel_actual-1].herramientas[i].coordenada.fila][juego.niveles[juego.nivel_actual-1].herramientas[i].coordenada.col] = 
		juego.niveles[juego.nivel_actual-1].herramientas[i].tipo;
	}

	matriz[juego.personaje.posicion.fila][juego.personaje.posicion.col] = MAC;
    matriz[juego.sombra.posicion.fila][juego.sombra.posicion.col] = BLUE;

    for (int i = 0; i < MAX_FILAS; i++){
		for (int j = 0; j < MAX_COLUMNAS; j++){
			printf(" %c ", matriz[i][j]);
		}
		printf("\n");
    }
    }