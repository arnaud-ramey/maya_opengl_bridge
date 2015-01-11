/*
 * Movement.cpp
 *
 *  Created on: Jan 12, 2010
 *      Author: arnaud
 */

/*!
 * refresh the display
 */
void refresh() {
	glutPostRedisplay();
}

void camera_track() {
	//	double X = 10;
	//	double Y = 5;
	//	double Z = 10;
	//	double lx = skeleton_position.x - X;
	//	double lz = skeleton_position.z - Z;
	//	double ly = 0;
	//	gluLookAt(X, Y, Z, X + lx, Y + ly, Z + lz, 0.0f, 1.0f, 0.0f);
}

void free_track() {
	gluLookAt(X, Y, Z, X + lx, Y + ly, Z + lz, 0.0f, 1.0f, 0.0f);
}

void reload_view() {
	glMatrixMode( GL_MODELVIEW);
	glLoadIdentity();
	// apply the compulsoory move if needed
	free_track();
	refresh();
}

/*! Reshape callback */
void reshape(int w, int h) {
	cout << "reshape()" << endl;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	h = (h <= 0 ? 1 : h);

	width = w;
	height = h;

	// Reset the coordinate system before modifying
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the clipping volume
	gluPerspective(45, 1.0f * w / h, 1, 1000);

	reload_view();
}

/*!
 * apply a rotation of an angle theta on place on lx0 and lz0
 * @param theta angle in radians
 * @param lx0
 * @param lz0
 */
void applyRotation(double theta, float* lx0, float* lz0) {
	double newLx = cos(theta) * *lx0 - sin(theta) * *lz0;
	double newLz = sin(theta) * *lx0 + cos(theta) * *lz0;
	*lx0 = newLx;
	*lz0 = newLz;
}

/*!
 * rotate the point of view from an angle
 * @param theta the angle in radians
 */
void rotate(double theta) {
	applyRotation(theta, &lx, &lz);
	reload_view();
}

/*!
 * advance the point of view from a certain distance
 * @param step the distance
 */
void advance(double step) {
	X += step * lx;
	Y += step * ly;
	Z += step * lz;
	reload_view();
}

/*!
 * advance perpediculary to the direction looked
 * @param step the distance to advance
 */
void strafe(double step) {
	float lx2 = lx, lz2 = lz;
	applyRotation(PI / 2.0f, &lx2, &lz2);
	X += step * lx2;
	Z += step * lz2;
	reload_view();
}

void switch_mode() {
	if (drawing_mode == DRAWING_MODE_AXES)
		drawing_mode = DRAWING_MODE_CYLINDERS;
	else if (drawing_mode == DRAWING_MODE_CYLINDERS)
		drawing_mode = DRAWING_MODE_RTG;
	else if (drawing_mode == DRAWING_MODE_RTG)
		drawing_mode = DRAWING_MODE_AXES;
}

void switch_anim() {
	anim_running = !anim_running;
}

/*!
 * the routine for handling key input
 * @param key the input key
 * @param x
 * @param y
 */
void keyboard(unsigned char key, int x, int y) {
	//just to remove unused variables
	x = x;
	y = y;
	double base_move = .5f;

	switch (key) {
	case 'q':
	case 'Q':
	case 27:
		exit(0);
		break;
		// moves
	case '1':
		strafe(-base_move);
		break;
	case '3':
		strafe(base_move);
		break;
	case '4':
		rotate(-.1f);
		break;
	case '6':
		rotate(.1f);
		break;
	case '8':
		advance(base_move);
		break;
	case '2':
		advance(-base_move);
		break;
	case '+':
		Y += base_move;
		reload_view();
		break;
	case '-':
		Y -= base_move;
		reload_view();
		break;
	case ' ':
		skeleton->set_next_animation();
		break;
	case 'r':
	case 'R':
		switch_mode();
		break;
	case 's':
	case 'S':
		switch_anim();
		break;
	default:
		break;//Don't do anything if any other keys are pressed
	}
}

void process_menu_anim(int option) {
	anim_running = (option == 1);
}

void process_menu_rendering(int option) {
	drawing_mode = option;
}

void process_menu_gait(int option) {
	skeleton->selected_animation = option;
}

//void process_menu_camera(int option) {
//	cout << endl << option << endl;
//	camera_mode = option;
//}

void createGLUTMenus() {
	int submenu_anim = glutCreateMenu(process_menu_anim);
	glutAddMenuEntry("Off", 0);
	glutAddMenuEntry("On", 1);

	int submenu_rendering = glutCreateMenu(process_menu_rendering);
	glutAddMenuEntry("Nothing", DRAWING_MODE_NO_DRAWN);
	glutAddMenuEntry("Axes", DRAWING_MODE_AXES);
	glutAddMenuEntry("Cylinders", DRAWING_MODE_CYLINDERS);
	glutAddMenuEntry("RTG", DRAWING_MODE_RTG);

	int submenu_gait = glutCreateMenu(process_menu_gait);
	glutAddMenuEntry("Still", 0);
	glutAddMenuEntry("Move1", 1);
	glutAddMenuEntry("Move2", 2);

	//	int submenu_camera = glutCreateMenu(process_menu_camera);
	//	glutAddMenuEntry("Free (keypad,+,- to move)", CAMERA_MODE_FREE);
	//	glutAddMenuEntry("Tracking horse", CAMERA_MODE_TRACK);

	// create the menu and
	// tell glut that "processMenuEvents" will
	// handle the events
	//	int menu =
	glutCreateMenu( NULL);
	glutAddSubMenu("Anim", submenu_anim);
	glutAddSubMenu("Rendering", submenu_rendering);
	glutAddSubMenu("Gait", submenu_gait);
	//	glutAddSubMenu("Camera", submenu_camera);

	// attach the menu to the right button
	glutAttachMenu( GLUT_RIGHT_BUTTON);
}

