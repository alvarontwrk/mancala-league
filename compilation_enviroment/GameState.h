/*
 * GameState.h
 *
 *  Created on: 15 ene. 2018
 *      Author: manupc
 */

#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include <string>

using namespace std;

// Representación de jugadores (Jugador 1 J1, Jugador 2 J2; ninguno NONE)
enum Player { J1 = 0, J2 = 1, NONE = 2 };

// Movimientos posibles de cada jugador
enum Move { M1 = 1, M2 = 2, M3 = 3, M4 = 4, M5 = 5, M6 = 6, M_NONE = 7 };

// Posiciones (casillas) de cada jugador
enum Position { GRANERO = 0, P1 = 1, P2 = 2, P3 = 3, P4 = 4, P5 = 5, P6 = 6 };

// Clase para representar el estado del juego en un punto de la partida
class GameState {

private:
  friend class SimulatorLink;

  // Turno del jugador al que le toca mover (valores válidos J1 o J2)
  Player turno;

  // Piezas de cada jugador. piezas[J1][] para el jugador 1, piezas[J2][] para
  // el jugador 2 piezas[x][i] es el número de piezas en la i-ésima casilla más
  // cercana al granero de cada jugador, de modo que piezas[J1][1] es el número
  // de piezas que hay justo antes del granero del jugador 1 y piezas[J1][6] el
  // número de piezas que hay en la casilla más alejada. Piezas[J1][0] son las
  // piezas que hay en el granero
  unsigned char piezas[2][7];

  void setBadMoveState(Player j); // Establece el estado final donde el jugador
                                  // j pierde por movimiento mal hecho

public:
  // Construye un estado del juego, inicialmente no válido
  GameState();

  // Construye un estado del juego como copia de otro pasado por argumento
  GameState(const GameState &other);

  // Operador de asignación
  GameState &operator=(const GameState &other);

  // Devuelve el jugador al que le toca jugar el turno
  Player getCurrentPlayer() const;

  // Devuelve las semillas existentes en una posición de un jugador.
  unsigned char getSeedsAt(Player p, Position pos) const;

  // Simula un movimiento de entrada, proporcionando como salida
  // un estado resultante de ejecutar el movimiento dado sobre el estado actual
  GameState simulateMove(Move mov) const;

  bool isFinalState() const; // Comprueba si el estado es final
  bool
  isValidState() const; // Devuelve true si el estado actual del juego es válido
  Player getWinner()
      const; // Devuelve el jugador ganador de la partida, en caso de que sea
             // estado final, o NONE en caso de que no sea estado final
  int getScore(Player p) const; // Devuelve la puntuación del jugador pasado por
                                // argumento. 0 si el jugador no es válido.
};

#endif /* GAMESTATE_H_ */
