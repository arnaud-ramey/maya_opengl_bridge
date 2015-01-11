/*
 * JointShower.h
 *
 *  Created on: Apr 24, 2010
 *      Author: arnaud
 */

#ifndef JOINTSHOWER_H_
#define JOINTSHOWER_H_

#include "Joint.h"
#include "Animation.h"

void init_jointshower(int w, int h);
void start_jointshower();
void set_skeleton(Joint* by);
void set_rtg_file(RTGFile* file);

#endif /* JOINTSHOWER_H_ */
