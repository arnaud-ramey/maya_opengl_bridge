/*
 * Includes.h
 *
 *  Created on: Apr 22, 2010
 *      Author: arnaud
 */

#ifndef INCLUDES_H_
#define INCLUDES_H_

// STL imports
#include <iostream>
#include <iomanip>    // for setw
#include <vector>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>    // for malloc
#include <string.h>    // for strlen
#include <math.h>    // for cos and sin
#include <vector>    // for vectors
#include <sys/time.h>  // for gettimeofdray
using namespace std;

extern "C" {
// #include "libs/glee/GLee.h"
#include "libs/glut/glut.h"
#include "libs/rtg/RTGlib.h"
}
#include "libs/bitmap/Bitmap.h"

/* some constants for the whole program */
#define  PI     3.14159
#define DEBUG      false
/* define a constant if we wanna save an image illustrating where the UV
 * are asked on the skin maps */
#define UV_IMAGE   false

// folders
#include "maya_opengl_bridge_path.h"
#define TEXTURES_FOLDER          MAYA_OPENGL_BRIDGE_PATH "textures/"
#define RTG_FOLDER               MAYA_OPENGL_BRIDGE_PATH "rtg/"
#define TEXTURES_WINDOWS_FOLDER  "D:/Arnaud/4A-kth/DH2640-Graphics_and_Interaction_Programming/lab3-opengl/code/textures/"

// primitives for the drawing
#define DRAWING_MODE_NO_DRAWN   0
#define DRAWING_MODE_AXES    1
#define DRAWING_MODE_CYLINDERS   2
#define DRAWING_MODE_RTG        3

// names of the parts of a horse
#define SPINE_FRONT     "spine_front"
#define FRONT_LEFT_ARM     "front_left_arm"
#define FRONT_LEFT_ELBOW  "front_left_elbow"
#define FRONT_LEFT_HAND    "front_left_hand"
#define FRONT_RIGHT_ARM    "front_right_arm"
#define FRONT_RIGHT_ELBOW   "front_right_elbow"
#define FRONT_RIGHT_HAND  "front_right_hand"
#define SPINE_BACK       "spine_back"
#define BACK_LEFT_LEG    "back_left_leg"
#define BACK_LEFT_KNEE    "back_left_knee"
#define BACK_LEFT_FOOT    "back_left_foot"
#define BACK_RIGHT_LEG    "back_right_leg"
#define BACK_RIGHT_KNEE    "back_right_knee"
#define BACK_RIGHT_FOOT    "back_right_foot"
#define NECK_START    "base_encolure"
#define NECK_END      "neck_end"
#define MOUTH        "mouth"

#endif /* INCLUDES_H_ */
