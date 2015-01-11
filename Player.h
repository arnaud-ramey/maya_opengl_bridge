/*
 * Player.h
 *
 *  Created on: Apr 24, 2010
 *      Author: arnaud
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Joint.h"

class Player: public Joint {
public:
	Player();
	virtual ~Player();
	void move_to_reference_position();
};

#endif /* PLAYER_H_ */
