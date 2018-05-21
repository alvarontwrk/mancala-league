/*
 * SimulatorLink.h
 *
 *  Created on: 12 ene. 2018
 *      Author: manupc
 */

#ifndef SIMULATORLINK_H_
#define SIMULATORLINK_H_

#include "GameState.h"
#include "Bot.h"
#include<string>
using namespace std;


class SimulatorLink {

private:
	Bot *bot; // Bot asociado a la partida actual
	GameState state; // Estado actual del juego
public:

	// Constructor por defecto. Inicia el enlace del simulador
	SimulatorLink();

	// Establece el bot que ejecutará la simulación
	void setBot(Bot *b);

	// Ejecuta una partida con el bot existente. Devuelve true si termina ok,
	// o false si hubo un error
	bool run();

	// Establece el estado del juego desde un string como entrada conteniendo el estado del juego.
	// El string es una secuencia de números enteros: "Turno G1 J11 J12 J13 J14 J15 J16 G2 J21 J22 J23 J24 J25 J26"
	//   turno: valor 1 para indicar que le toca mover al jugador 1; 2 para el jugador 2
	//   Gx: Piezas en el granero del jugador x
	//   Jxy: Piezas en la casilla y del jugador x (x=1,2; y=1,2,3,4,5,6)
	// En caso de error, el motor inicializa el estado del juego a no válido
	// Ejemplo de estado válido: "1 0 4 4 4 4 4 4 0 4 4 4 4 4 4", que es el estado inicial.
	// La función devuelve true si el estado se estableció con éxito y false en otro caso
	bool setStateFromString(const string &strState);

};

#endif /* SIMULATORLINK_H_ */
