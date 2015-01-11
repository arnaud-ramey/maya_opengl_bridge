/*
 * Joint.cpp
 *
 *  Created on: Apr 22, 2010
 *      Author: arnaud
 *
 *      For the references about how to use a skeleton and a mesh
 *      http://www.runes-mmorpg.com/index.php?option=com_content&view=article&id=70
 */

#include "Joint.h"
#include "Animation.h"

Joint::Joint(Joint* father, string name) {
	//	cout << "Joint::Joint()" << endl;
	//	weights_image = NULL;
	set_name(name);

	// init the matrices
	matrix_absolute = new double[16];
	matrix_relative = new double[16];
	matrix_inverse = new double[16];
	matrix_finale = new double[16];

	// init the body
	// init the relative params
	relative_position = Point3(0, 0, 0);
	relative_rotation = Point4(0, 1, 0, 0);
	relative_scale = Point3(1, 1, 1);
	absolute_position = Point3(0, 0, 0);
	move_to_reference_position();

	// set the father
	this->father = NULL;
	set_father(father);

	weights_image = NULL;

	set_drawn_by_cylinder();
	set_no_animation();

}

Joint::~Joint() {
	// delete the sons
	for (vector<Joint*>::iterator it = children.begin(); it < children.end(); ++it)
		delete *it;

	// delete the anim
	for (vector<Animation*>::iterator it = animations.begin(); it
			< animations.end(); ++it) {
		delete *it;
	}

	// delete the image
	delete weights_image;

	delete[] matrix_absolute;
	delete[] matrix_relative;
	delete[] matrix_inverse;
	delete[] matrix_finale;
}

Joint* Joint::clone(Joint* toCopy, string name) {
	Joint* ans = clone(toCopy, toCopy->get_father());
	ans->set_name(name);
	return ans;
}

Joint* Joint::clone(Joint* toCopy, Joint* newFather) {
	cout << toCopy->get_name() << "::clone()" << endl;
	//	cout << "clone !!!" << endl;

	Joint* ans = new Joint(newFather, "");
	// copy params
	ans->relative_position = toCopy->relative_position.clone();
	ans->relative_scale = toCopy->relative_scale.clone();
	ans->relative_rotation = toCopy->relative_rotation.clone();
	//	ans->compute_matrices();

	// set mode
	ans->type_of_drawin = toCopy->type_of_drawin;
	ans->rtg_filename = toCopy->rtg_filename;
	//	ans->rtg_scale = toCopy->rtg_scale;
	//	ans->rtg_position = toCopy->rtg_position;

	// copy children
	for (vector<Joint*>::iterator it = toCopy->children.begin(); it
			< toCopy->children.end(); ++it) {
		Joint::clone(*it, ans);
	}

	return ans;
}

void Joint::move_to_reference_position() {
	/// leave this empty, it is implemented by the sons of Joint
	/// (Horse for instance)
}

void Joint::set_reference_position() {
	cout << get_name() << "::set_reference_position()" << endl;
	compute_inverse_absolute_matrix();
	load_weights_image();
}

void Joint::set_name(string name) {
	cout << "Joint::set_name(\"" << name << "\")" << endl;
	//	has_weights_image = false;
	this->name = name;
	if (name == "")
		return;
}

string Joint::get_name() {
	return name;
}

void Joint::compute_matrices() {
#if DEBUG
	cout << get_name() << "::compute_matrices()" << endl;
#endif

	//// compute the relative
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(relative_position.x, relative_position.y, relative_position.z);
	glRotated(relative_rotation.w, relative_rotation.x, relative_rotation.y,
			relative_rotation.z);
	glScaled(relative_scale.x, relative_scale.y, relative_scale.z);
	glGetDoublev(GL_MODELVIEW_MATRIX, matrix_relative);
	transpose(matrix_relative, matrix_relative, 4);
	glPopMatrix();

	//// compute the absolute = father.abs * relative
	if (father != NULL) {
		//		cout << "case father" << endl;
		multMatrices(father->matrix_absolute, matrix_relative, matrix_absolute,
				4, 4, 4);
		//		multMatrices(matrix_relative, father->matrix_absolute, matrix_absolute,
		//				4, 4, 4);
	} else {
		//		cout << "case no father" << endl;
		// direct copy
		for (int i = 0; i < 16; ++i)
			matrix_absolute[i] = matrix_relative[i];
	}

	//// compute the finale = inverse * absolue
	//	multMatrices(matrix_inverse, matrix_absolute, matrix_finale, 4, 4, 4);
	// we do the contrary because we are in column major order
	multMatrices(matrix_absolute, matrix_inverse, matrix_finale, 4, 4, 4);
#if DEBUG
	cout << "matrix_inverse" << endl;
	display_matrix(matrix_inverse, 4, 4);
	cout << "matrix_absolute" << endl;
	display_matrix(matrix_absolute, 4, 4);
	cout << "matrix_finale" << endl;
	display_matrix(matrix_finale, 4, 4);
#endif // DEBUG

	//	glMatrixMode(GL_MODELVIEW);
	//	glPushMatrix();
	//	glLoadIdentity();
	//	glMultMatrixd(matrix_inverse);
	//	glMultMatrixd(matrix_absolute);
	//	glGetDoublev(GL_MODELVIEW_MATRIX, matrix_finale);
	//	glPopMatrix();

	//	display(true);
	/// get absolute position
	absolute_position.x = matrix_absolute[3];//12
	absolute_position.y = matrix_absolute[7];//13
	absolute_position.z = matrix_absolute[11];//14

	// compute for the sons
	for (vector<Joint*>::iterator it = children.begin(); it < children.end(); ++it) {
		(*it)->compute_matrices();
	}
}

void Joint::compute_inverse_absolute_matrix() {
	cout << get_name() << "::compute_inverse_absolute_matrix()" << endl;

	// compute the inverse of absolute
	bool OK = inverse_matrix4f(matrix_absolute, matrix_inverse);
	cout << "matrix_absolute:" << endl;
	display_matrix(matrix_absolute, 4, 4);
	cout << "matrix_inverse:" << endl;
	display_matrix(matrix_inverse, 4, 4);
	if (!OK)
		cout << endl << "!!! could not invert !!!" << endl;

	// compute for the sons
	for (vector<Joint*>::iterator it = children.begin(); it < children.end(); ++it) {
		(*it)->compute_inverse_absolute_matrix();
	}
}

double * Joint::get_matrix_absolute() const {
	return matrix_absolute;
}

double * Joint::get_matrix_relative() const {
	return matrix_relative;
}

double * Joint::get_matrix_inverse() const {
	return matrix_inverse;
}

double * Joint::get_matrix_finale() const {
	return matrix_finale;
}

Joint * Joint::get_father() const {
	return father;
}

vector<Joint*> Joint::get_children() {
	return children;
}

Point3 Joint::get_relative_position() const {
	return relative_position;
}

Point3 Joint::get_absolute_position() const {
	return absolute_position;
}

Point4 Joint::get_relative_rotation() const {
	return relative_rotation;
}

Point3 Joint::get_relative_scale() const {
	return relative_scale;
}

void Joint::copy_relative_params(Joint* toCopy) {
	relative_position = toCopy->get_relative_position();
	relative_rotation = toCopy->get_relative_rotation();
	relative_scale = toCopy->get_relative_scale();
	//compute_matrices();
}

void Joint::set_father(Joint *father) {
	// remove from a previous old parent
	if (this->father != NULL) {
		vector<Joint*> children = this->father->children;
		for (vector<Joint*>::iterator it = children.begin(); it
				< children.end(); ++it) {
			if (*it == this) {
				cout << "removing old child" << endl;
				children.erase(it);
				--it;
			}
		}
	}

	// set the new father
	this->father = father;
	if (father != NULL)
		father->children.push_back(this);
	//	compute_matrices();
}

void Joint::set_relative_position(Point3 position) {
	this->relative_position = position;
	//	compute_matrices();
}

void Joint::set_relative_rotation(Point4 rotation) {
	this->relative_rotation = rotation;
	//	compute_matrices();
}

void Joint::set_relative_scale(Point3 scale) {
	this->relative_scale = scale;
	//	compute_matrices();
}

void Joint::display(bool display_matrices /*= true*/) {
	for (int space = 0; space < get_depth(); ++space)
		cout << '-';
	cout << "[Joint] \"" << get_name() << "\"";
	cout << ", pos:";
	get_absolute_position().display();
	cout << ", depth:" << get_depth();
	cout << endl;
	if (display_matrices) {
		cout << " * relative:" << endl;
		display_matrix(get_matrix_relative(), 4, 4);
		cout << " * absolute:" << endl;
		display_matrix(get_matrix_absolute(), 4, 4);
	}
}

void Joint::display_with_sons(bool display_matrices /*= true*/) {
	if (get_depth() == 0)
		cout << endl;

	display(display_matrices);
	// compute for the sons
	for (vector<Joint*>::iterator it = children.begin(); it < children.end(); ++it)
		(*it)->display_with_sons(display_matrices);

	if (get_depth() == 0)
		cout << endl;
}

int Joint::get_depth() {
	if (get_father() == NULL)
		return 0;
	else
		return 1 + get_father()->get_depth();
}

Joint* Joint::find(string name) {
	if (this->get_name() == name)
		return this;

	for (vector<Joint*>::iterator it = children.begin(); it < children.end(); ++it) {
		//		cout << "looking'" << (*it)->name << "'" << endl;
		Joint* child_find = (*it)->find(name);
		if (child_find != NULL)
			return child_find;
	}
	return NULL;
}

double Joint::get_selected_animation_speed() {
	return animations.at(selected_animation)->speed;
}

#include <sys/stat.h>
bool FileExists(string strFilename) {
	struct stat stFileInfo;
	bool blnReturn;
	int intStat;

	// Attempt to get the file attributes
	intStat = stat(strFilename.c_str(), &stFileInfo);
	if (intStat == 0) {
		// We were able to get the file attributes
		// so the file obviously exists.
		blnReturn = true;
	} else {
		// We were not able to get the file attributes.
		// This may mean that we don't have permission to
		// access the folder which contains this file. If you
		// need to do that level of checking, lookup the
		// return values of stat which will give you
		// more details on why stat failed.
		blnReturn = false;
	}

	return (blnReturn);
}

double Joint::weight(const double& x, const double& y) {
	//	cout << "Joint::weight(" << x << ", " << y << ")" << endl;
	if (!has_weights_image)
		return 0;
	//	return 0;
	int x_pix = (1.f - x) * weights_image->width();
	int y_pix = (1.f - y) * weights_image->height();
	//	cout << "pix:" << x_pix << ", " << y_pix << endl;
	return weights_image->get(x_pix, y_pix).x;
}

void Joint::load_weights_image() {
	cout << get_name() << "::load_weights_image()" << endl;

	// load the new weight image
	if (weights_image != NULL)
		delete weights_image;

	ostringstream filename_skin;
	//	filename_skin << WEIGHTS_FOLDER << "horse_joint_" << get_name() << ".jpeg";
	filename_skin << get_root()->weight_folder << get_name() << ".gif";//".jpeg";
	cout << "Joint:for mesh '" << get_name() << "':skin file '"
			<< filename_skin.str() << "' ";
	if (FileExists(filename_skin.str())) {
		cout << "exists, loading it" << endl;
		weights_image = new Image(filename_skin.str());
		has_weights_image = true;

		// "touch" the image
		touch_weights_image();

	} else { // image does not exist
		cout << "does not exist, no skin." << endl;
		has_weights_image = false;
	}

	/// load the image for the sons
	for (vector<Joint*>::iterator it = children.begin(); it < children.end(); ++it) {
		(*it)->load_weights_image();
	}

}

void Joint::touch_weights_image() {
	cout << get_name() << "::touch_image() : ";
	cout.flush();
	cout << "width=" << weights_image->width() << endl;
}
