/*
 * SimulatorLink.cpp
 *
 *  Created on: 12 ene. 2018
 *      Author: manupc
 */

#include "SimulatorLink.h"
#include <cstring>
#include <string>
#include <cstdlib>
#include "GameState.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

char * strsep(char **sp, char *sep)
    {
        char *p, *s;
        if (sp == NULL || *sp == NULL || **sp == '\0') return(NULL);
        s = *sp;
        p = s + strcspn(s, sep);
        if (*p != '\0') *p++ = '\0';
        *sp = p;
        return(s);
    }

SimulatorLink::SimulatorLink() {

	bot= 0;
}


bool SimulatorLink::setStateFromString(const string &strState) {


	int currentPos= 0;
	GameState newState; // Estado auxiliar

	const char *cad= strState.c_str();

	char *tofree, *token, *str= new char[strlen(cad)+1];
	strcpy(str, cad);
	tofree = str;

	while ((token = strsep(&str, " "))) {

		if (currentPos == 0) {

			int jugador= atoi(token);

			if (jugador == 1) {
				newState.turno= J1;
			} else if (jugador == 2) {
				newState.turno= J2;
			} else {
				delete [] tofree;
				return false;
			}
			currentPos++;
		} else {

			int y= (currentPos-1)/7;
			int x= (currentPos-1)%7;
			newState.piezas[y][x]= atoi(token);
			currentPos++;
		}
	}

	delete [] tofree;

	// El string no tiene tantos valores como se requieren
	if (currentPos != 1+7*2)
		return false;

	// Comprobamos la validez del estado
	int total= 0;
	for (int i= 0; i<2; i++) {
		for (int j= 0; j<=6; j++) {

			if (newState.piezas[i][j] < 0 )
				return false;
			total+= newState.piezas[i][j];
		}
	}
	if (total != 48) return false;

	// Comprobamos si es estado compatible con final, y lo hacemos final

	for (int jug=0;jug<=1;jug++) {

		Player jActual= (Player)jug;

		// Comprobamos si el jugador se ha quedado sin fichas
		bool fin= true;
		for (int i= 1; i<=6 && fin; i++) {
			if (newState.piezas[jActual][i] > 0)
				fin= false;
		}
		if (fin) { // Si se ha quedado sin fichas, hacemos recuento del otro jugador
			int total= 0;
			for (int i= 1; i<=6; i++) {
				total+=newState.piezas[1-jActual][i];
				newState.piezas[1-jActual][i]= 0;
			}
			newState.piezas[1-jActual][0]+= total;
		}
	}


	// Copiamos el estado, dado que es válido
	state= newState;
	return true;
}


//string SimulatorLink::getStateAsString() {
//
//	string aux;
//
//	aux= state.turno;
//	for (int i= 0; i<2; i++) {
//		for (int j= 0; j<7; j++) {
//			aux+= " "+(int)state.piezas[i][j];
//		}
//	}
//
//	return aux;
//}



bool SimulatorLink::run() {

	bool continuar= true;
	string entrada, salida;
	char saux[100];

	if (bot == 0) {
		return false;
	}

	bot->initialize(); // Inicializamos el bot

	while (continuar) {

		// Vemos qué nos pide el simulador
		cin>>entrada;


		if (entrada == "END") { // Fin de la partida ok

			continuar= false;
			salida= "OK";

		} else if (entrada == "GETNAME") // El simulador nos pide el nombre del bot
			salida= bot->getName();

		else if (entrada == "BADNAME") { // El simulador nos dice que el nombre no es válido
			cout<<"OK"<<endl;
			return false;

		} else if (entrada == "PING") { // Nos hace un ping
			salida= "PONG";

		} else if (entrada == "SETPLAYER") { // Nos dice qué jugador somos

			// Cogemos el jugador que somos
			int aux;
			Player player;
			cin>>aux;
			if (aux == 0)
				player= J1;
			else if (aux == 1)
				player= J2;
			else {
				return false;
			}

			bot->setPlayer(player);
			salida= "OK";

		} else if (entrada == "GETMOVE") {

			// Leemos los movimientos realizados por el oponente
			vector<Move>moves;
			int num, aux;
			cin>>num;
			for (int i= 0; i<num; i++) {
				cin>>aux;
				moves.push_back((Move)aux);
			}

			// Leemos el estado actual del juego
			int turno;
			cin>>turno;
			if (turno == 0)
				state.turno= J1;
			else
				state.turno= J2;
			for (int i= 0; i<2; i++) {
				for (int j= 0; j<7; j++) {
					cin >> aux;
					state.piezas[i][j]= (unsigned char)aux;
				}
			}

			// Pedimos al bot que realice el movimiento
			Move m= bot->nextMove(moves, state);
            sprintf(saux, "%d", (int)m);
			salida= saux;
		} else if (entrada == "TIMEOUT") {
			long l;
			cin>>l;
			bot->setTimeOut(l);
		}

		// Damos como salida el dato requerido
		cout<<salida<<endl;
	}

	return true;
}


void SimulatorLink::setBot(Bot *b) {
	bot= b;
}
