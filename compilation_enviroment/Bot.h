/*
 * Bot.h
 *
 *  Created on: 12 ene. 2018
 *      Author: manupc
 */

#ifndef BOT_H_
#define BOT_H_

#include <string>
#include <vector>
#include "GameState.h"
using namespace std;

class Bot {

private:
	// Jugador asociado al bot (J1 o J2. Valor NONE para jugador no válido)
	Player player;

	// Tiempo (en milisegundos) que el bot tendrá para responder por turno. Valor 0 para tiempo indefinido.
	long timeout;

public:

	/**
	 * Constructor. Crea un bot por defecto con nombre válido. Inicializa
	 * los valores del objeto que el alumno necesite.
	 */
	Bot();

	virtual ~Bot() {}

	/**
	 * Establece el bot como jugador J1 o J2. Esta función se llama automáticamente
	 * desde el simulador.
	 */
	void setPlayer(const Player p);


	/**
	 * Indica si el bot actúa como jugador J1 o J2. Valor NONE si no está establecido
	 */
	Player getPlayer();


	/**
	 * Establece el límite de tiempo (en milisegundos) que el bot tendrá para responder por turno. Valor 0 para tiempo indefinido.
	 */
	void setTimeOut(long t);


	/**
	 * Devuelve el límite de tiempo (en milisegundos) que el bot tendrá por turno para responder. Valor 0 para tiempo indefinido.
	 */
	long getTimeOut();


	/**
	 * Función para inicializar el bot. Esta función se llama por el simulador
	 * al comienzo de cada partida, para que el alumno inicialice sus variables
	 * (en caso de que haya) antes de comenzar el juego.
	 */
	virtual void initialize()= 0;


	/**
	 * Devuelve el nombre del bot
	 */
	virtual string getName()= 0;

	/**
	 * Dada la serialización de un estado del juego como cadena de entrada,
	 * esta función devuelve el siguiente movimiento a realizar por el bot.
	 *
	 * El string es una secuencia de números enteros: "Turno G1 J11 J12 J13 J14 J15 J16 G2 J21 J22 J23 J24 J25 J26"
	 *   turno: valor 1 para indicar que le toca mover al jugador 1; 2 para el jugador 2
	 *   Gx: Piezas en el granero del jugador x
	 *   Jxy: Piezas en la casilla y del jugador x (x=1,2; y=1,2,3,4,5,6)
	 * En caso de error, el motor inicializa el estado del juego a no válido
	 * Ejemplo de estado válido: "1 0 4 4 4 4 4 4 0 4 4 4 4 4 4", que es el estado inicial.
	 */
	virtual Move nextMove(const vector<Move> &adversary, const GameState &state)= 0;


};

#endif /* BOT_H_ */
