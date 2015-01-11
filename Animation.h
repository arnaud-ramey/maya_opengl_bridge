/*
 * Animation.h
 *
 *  Created on: Apr 25, 2010
 *      Author: arnaud
 */

#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "Joint.h"
#include "Man.h"
#include "Player.h"

class Animation {
public:
	Joint* j;
	double speed; // pixels per second

	Animation(Joint* model) {
		j = model;
	}
	~Animation() {
	}
	virtual void anim(double t) {
		cout << "Animation::anim(t)" << endl;
	}
};

class Still: public Animation {
public:
	Still(Joint* model);
	void anim(double t);
};


class ShakeArms: public Animation {
public:
	ShakeArms(Joint* model);
	void anim(double t);
};

class ShakeArms2: public Animation {
public:
	ShakeArms2(Joint* model);
	void anim(double t);
};


#endif /* ANIMATION_H_ */
