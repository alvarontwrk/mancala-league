/*
 * Bot.cpp
 *
 *  Created on: 12 ene. 2018
 *      Author: manupc
 */

#include "Bot.h"

Bot::Bot() {
	player= NONE;
	timeout= 0;
}



void Bot::setPlayer(const Player p) {

	player= p;
}


Player Bot::getPlayer() {
	return player;
}



void Bot::setTimeOut(long t) {

	if (t>=0)
		timeout= t;
	else
		timeout= 0;
}


long Bot::getTimeOut() {
	return timeout;
}
