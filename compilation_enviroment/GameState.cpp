/*
 * GameState.cpp
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#include "GameState.h"
#include <cstring>
#include <string>
using namespace std;

GameState::GameState() {

	// Se construye un estado no válido

	// Ningún jugador inicializado
	turno= NONE;


	// Cada casilla de cada jugador contiene 4 granos inicialmente (total= 6*4= 24)
	for (int i= 0; i<=6; i++) {
		piezas[J1][i]= piezas[J2][i]= 0;
	}
}


GameState::GameState(const GameState &other) {

	*this= other;
}


GameState & GameState::operator=(const GameState &other) {

	if (this == &other) return *this;

	turno= other.turno;

	memcpy(piezas[J1], other.piezas[J1], sizeof(unsigned char)*7);
	memcpy(piezas[J2], other.piezas[J2], sizeof(unsigned char)*7);
	return *this;
}



bool GameState::isFinalState() const {

	if (!isValidState()) return false;

	for (int i= 1; i<=6; i++)
		if (piezas[J1][i] > 0 || piezas[J2][i] > 0) return false;

	return true;
}


bool GameState::isValidState() const {

	int totalFichas= 0;

	if (turno != J1 && turno != J2) return false;

	for (int i= 0; i<2; i++) {
		for (int j= 0; j<7; j++) {
			totalFichas+=piezas[i][j];
			if (piezas[i][j]<0) return false;
		}
	}
	if (totalFichas != 48) return false;

	return true;
}


Player GameState::getWinner() const {

	if (!isValidState() || !isFinalState()) return NONE;

	if (piezas[J1][0] > piezas[J2][0])
		return J1;
	else if (piezas[J1][0] < piezas[J2][0])
		return J2;

	return NONE;
}


int GameState::getScore(Player p) const {

	switch(p) {
	case J1:
		return piezas[J1][0];
	case J2:
		return piezas[J2][0];
	default:
		return 0;
	}

}




Player GameState::getCurrentPlayer() const {

	return turno;
}


unsigned char GameState::getSeedsAt(Player p, Position pos) const {

	if (p<0 || p>1 || pos<0 || pos>6) return 0;
	return piezas[p][pos];
}


void GameState::setBadMoveState(Player j) {

	if (j != J1 && j!= J2) return;

	Player perdedor= j, ganador;
	if (j==J1) ganador=J2; else ganador= J1;

	memset(piezas[J1]+1, 0, sizeof(unsigned char)*6);
	memset(piezas[J2]+1, 0, sizeof(unsigned char)*6);

	piezas[ganador][0]= 48;
	piezas[perdedor][0]= 0;
}



GameState GameState::simulateMove(Move move) const {

	GameState state= *this;

	if (!isValidState()) {

		state.turno= NONE;
		for (int i= 0; i<7; i++) {
			state.piezas[0][i]= state.piezas[1][i]= 0;
		}
		return state;
	}

	bool turnoExtra= false;
	Player jActual= state.turno;


	if (move<=0 || move>6 || state.piezas[jActual][move] == 0) {
		state.setBadMoveState(jActual);
	} else {

		unsigned char piezas= state.piezas[jActual][move];
		int pos= move;

		// Quitamos las piezas de la casilla
		state.piezas[jActual][pos]= 0;
		pos--;

		// Repartimos todas las piezas
		while (piezas>0) {

			// Repartimos las piezas por la zona del jugador que juega
			while (pos>=0 && piezas>0) {
				state.piezas[jActual][pos]++;
				piezas--;
				if (piezas>0)
					pos--;
			}

			// Si no quedan piezas y se terminó en el granero, turno extra
			if (pos == 0 && piezas == 0) {
				turnoExtra= true;
			}


			// Si no quedan piezas en la última casilla (que no es granero), y
			// el otro jugador sí tiene piezas en la casilla opuesta, se hace combo
			if (pos>0 && state.piezas[jActual][pos]==1 && state.piezas[1-jActual][7-pos]>0) {

				state.piezas[jActual][0]+= state.piezas[1-jActual][7-pos]+1;
				state.piezas[jActual][pos]= 0;
				state.piezas[1-jActual][7-pos]= 0;
			}

			// Si sigen quedando piezas, seguimos repartiendo por el lado del otro jugador

			if (piezas > 0) {
				// Pasamos a repartir al otro jugador
				pos= 6;
				while (pos>=1 && piezas>0) {
					state.piezas[1-jActual][pos]++;
					piezas--;
					pos--;
				}

				// Pasamos de nuevo al jugador inicial
				pos= 6;
			}
		}
	}

	// Comprobamos si el jugador se ha quedado sin fichas
	bool fin= false;
	for (int p= 0; p<2 && !fin; p++) {
		fin= true;
		for (int i= 1; i<=6 && fin; i++) {
			if (state.piezas[p][i] > 0)
				fin= false;
		}

	}
	if (fin) { // Si se ha quedado sin fichas, hacemos recuento del otro jugador

		for (int p= 0; p<2; p++) {
			int total= 0;
			for (int i= 1; i<=6; i++) {
				total+=state.piezas[p][i];
				state.piezas[p][i]= 0;
			}
			state.piezas[p][0]+= total;
		}
	}


	// Pasamos el turno
	if (!turnoExtra) {
		if (jActual == J1)
			state.turno= J2;
		else
			state.turno= J1;
	}


	return state;
}



