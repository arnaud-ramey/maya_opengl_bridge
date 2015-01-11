/*
 * Man.h
 *
 *  Created on: Apr 24, 2010
 *      Author: arnaud
 */

#ifndef MAN_H_
#define MAN_H_

#include "Joint.h"

class Man: public Joint {
public:
	Man();
	virtual ~Man();
	void move_to_reference_position();
};

#endif /* MAN_H_ */
