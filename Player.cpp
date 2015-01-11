/*
 * Player.cpp
 *
 *  Created on: Apr 24, 2010
 *      Author: arnaud
 */

#include "Player.h"

Player::Player() :
	Joint(NULL, SPINE_FRONT) {
	this->set_drawn_by_cylinder();
	this->set_rtg_file("player.rtg");//, Point3(0, -3.5f, 0), 1);
  this->weight_folder = MAYA_OPENGL_BRIDGE_PATH "textures/skin/player/";

	// left arm
	Joint* left_shoulder = new Joint(this, FRONT_LEFT_ARM);
	left_shoulder->set_drawn_by_cylinder();
	Joint* left_elbow = new Joint(left_shoulder, FRONT_LEFT_ELBOW);
	left_elbow->set_drawn_by_cylinder();
	Joint* left_hand = new Joint(left_elbow, FRONT_LEFT_HAND);
	left_hand->set_drawn_by_cylinder();

	// right arm
	Joint* right_shoulder = Joint::clone(left_shoulder, FRONT_RIGHT_ARM);
	right_shoulder->first_child()->set_name(FRONT_RIGHT_ELBOW);
	right_shoulder->first_child()->first_child()->set_name(FRONT_RIGHT_HAND);

	move_to_reference_position();
	compute_matrices();
	set_reference_position();
}

void Player::move_to_reference_position() {
	this->set_relative_position(Point3(0, 14.7, 0));
	this->set_relative_rotation(Point4(90, 0, 1, 0));

	// left shoulder
	Joint* left_shoulder = find(FRONT_LEFT_ARM);
	left_shoulder->set_relative_rotation(Point4(90, 0, 1, 0));
	left_shoulder->set_relative_position(Point3(0, 0, -1.8));

	// left elbow
	Joint* left_elbow = find(FRONT_LEFT_ELBOW);
	left_elbow->set_relative_rotation(Point4(0, 0, 0, 1));
	left_elbow->set_relative_position(Point3(2.6, 0, 0));

	// left hand
	Joint* left_hand = find(FRONT_LEFT_HAND);
	left_hand->set_relative_rotation(Point4(0, 0, 0, 1));
	left_hand->set_relative_position(Point3(2.8, 0, 0));

	// right arm
	Joint* right_shoulder = find(FRONT_RIGHT_ARM);
	right_shoulder->set_relative_rotation(Point4(-90, 0, 1, 0));
	right_shoulder->set_relative_position(Point3(0, 0, 1.8));
	find(FRONT_RIGHT_ELBOW)->copy_relative_params(left_elbow);
	find(FRONT_RIGHT_HAND)->copy_relative_params(left_hand);
}

Player::~Player() {
}
