/*
 * Animation.cpp
 *
 *  Created on: Apr 25, 2010
 *      Author: arnaud
 */

#include "Animation.h"

/*
 * Still
 */
Still::Still(Joint* model) :
	Animation(model) {
	j = model;
	speed = 0;
}

void Still::anim(double t) {
	j->move_to_reference_position();

	j->compute_matrices();
}

void set_angle(Joint* j, string name, double angle) {
	j->find(name)->set_relative_rotation(Point4(angle, 0, 0, 1));
}

double get_angle(Joint* j, string name) {
	return j->find(name)->get_relative_rotation().w;
}

void set_angle_var(Joint* j, string name, double angle_var) {
	set_angle(j, name, get_angle(j, name) + angle_var);
}

/*
 * ShakeArms
 */
ShakeArms::ShakeArms(Joint* model) :
	Animation(model) {
	j = model;
	speed = .9;
}

void ShakeArms::anim(double t0) {
	j->move_to_reference_position();
	double t = t0 * 4;
	j->find(FRONT_LEFT_ARM)->set_relative_rotation(Point4(135 + 40 * cos(t), 0,
			1, -1));
	j->find(FRONT_RIGHT_ARM)->set_relative_rotation(Point4(-90 + 40 * cos(t/2
			+ 90), 0, 1, 1));

	j->compute_matrices();
}

ShakeArms2::ShakeArms2(Joint* model) :
	Animation(model) {
	j = model;
	speed = .9;
}

void ShakeArms2::anim(double t0) {
	j->move_to_reference_position();
	double t = t0 * 2;
	//	j->set_relative_position(Point3(0, 5, 0));
	//	j->set_relative_rotation(Point4(45 + 15 * cos(t), 1, 0, 0));
	j->find(FRONT_LEFT_ARM)->set_relative_rotation(Point4(90 + 40 * cos(t), 0,
			1, 0));
	j->find(FRONT_LEFT_ELBOW)->set_relative_rotation(Point4(45 + 45 * cos(t),
			0, 1, 0));
	j->find(FRONT_RIGHT_ARM)->set_relative_rotation(Point4(-90 + 40 * cos(t/2
			+ 90), 0, 1, 1));
	j->find(FRONT_RIGHT_ELBOW)->set_relative_rotation(Point4(-60 - 15 * cos(t
			+ 30), 0, 1, 0));

	j->compute_matrices();
}
