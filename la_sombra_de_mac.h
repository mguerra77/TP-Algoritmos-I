#ifndef __LA_SOMBRA_DE_MAC__
#define __LA_SOMBRA_DE_MAC__

#include <stdbool.h>

#define MAX_FILAS 20
#define MAX_COLUMNAS 25
#define MAX_ELEMENTOS 500
#define MAX_NIVELES 3
#define MAX_PAREDES 200
#define MAX_NOMBRE 100
#define CANTIDAD_POZOS_NIVEL_1 15
#define CANTIDAD_VELAS_NIVEL_1 5
#define CANTIDAD_INTERRUPTORES_NIVEL_1 1
#define CANTIDAD_PORTALES_NIVEL_1 0
#define CANTIDAD_ESCALERAS_NIVEL_1 10
#define CANTIDAD_MONEDAS_NIVEL_1 10
#define CANTIDAD_LLAVES_NIVEL_1 0
#define CANTIDAD_PUERTAS_NIVEL_1 1
#define CANTIDAD_POZOS_NIVEL_2 20
#define CANTIDAD_VELAS_NIVEL_2 10
#define CANTIDAD_INTERRUPTORES_NIVEL_2 2
#define CANTIDAD_PORTALES_NIVEL_2 2
#define CANTIDAD_ESCALERAS_NIVEL_2 15
#define CANTIDAD_MONEDAS_NIVEL_2 15
#define CANTIDAD_LLAVES_NIVEL_2 1
#define CANTIDAD_PUERTAS_NIVEL_2 1
#define CANTIDAD_POZOS_NIVEL_3 30
#define CANTIDAD_VELAS_NIVEL_3 12
#define CANTIDAD_INTERRUPTORES_NIVEL_3 4
#define CANTIDAD_PORTALES_NIVEL_3 4
#define CANTIDAD_ESCALERAS_NIVEL_3 15
#define CANTIDAD_MONEDAS_NIVEL_3 15
#define CANTIDAD_LLAVES_NIVEL_3 1
#define CANTIDAD_PUERTAS_NIVEL_3 1

typedef struct coordenada {
    int fila;
    int col;
}coordenada_t;

typedef struct personaje {
    coordenada_t posicion;
    int vida;
    int puntos;
    bool tiene_llave;
    bool interruptor_apretado;
}personaje_t;

typedef struct sombra {
    coordenada_t posicion;
    bool esta_viva;
}sombra_t;

typedef struct elemento {
    char tipo;
    coordenada_t coordenada;
}elemento_t;

typedef struct nivel {
    int numero_nivel;
    coordenada_t paredes[MAX_PAREDES];
    int tope_paredes;
    elemento_t obstaculos[MAX_ELEMENTOS];
    int tope_obstaculos;
    elemento_t herramientas[MAX_ELEMENTOS];
    int tope_herramientas;
}nivel_t;

typedef struct juego {
    personaje_t personaje;
    sombra_t sombra;
    nivel_t niveles[MAX_NIVELES];
    int nivel_actual;
}juego_t;

typedef struct elementos {
    char tipo[MAX_NOMBRE];
	int cantidad;
	int numero_nivel;
} objeto_t;


/*
 * Procedimiento que recibe el juego e imprime toda su información por pantalla.
 */
void imprimir_terreno(juego_t juego);

/* 
 *Inicializará el juego, cargando toda la información inicial, los datos del personaje, y los 3 niveles.
 */
void inicializar_juego(juego_t* juego, objeto_t elementos[MAX_ELEMENTOS]);

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
int estado_juego(juego_t juego);

/*
 * El nivel se dará por terminado, si ambos personajes pasaron por la puerta teniendo la 
 * llave correspondiente.
 * Devolverá:
 * -> 0 si el estado es jugando.
 * -> 1 si el estado es ganado.
 */
int estado_nivel(juego_t juego);

/*
 * Moverá el personaje, y realizará la acción necesaria en caso de chocar con un elemento
 */
void realizar_jugada(juego_t* juego);
     

#endif