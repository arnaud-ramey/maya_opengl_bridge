/*
 * Joint.h
 *
 *  Created on: Apr 22, 2010
 *      Author: arnaud
 */

#ifndef JOINT_H_
#define JOINT_H_

#include "Includes.h"
#include "Vertex.h"
#include "Image.h"

// forward declaration
class Animation;

class Joint {
public:
	Joint(Joint* father, string name);
	virtual ~Joint();
	static Joint* clone(Joint* toCopy, string name);

	// define the default orientation for the Joint
	virtual void move_to_reference_position();
	void set_reference_position();

	Joint *get_father() const;
	void set_father(Joint *parent);
	Joint* get_root() {
		return (get_father() == NULL ? this : get_father()->get_root());
	}
	int get_depth();
	vector<Joint*> get_children();
	Joint* first_child() {
		return get_children().front();
	}
	Point3 get_relative_position() const;
	Point3 get_absolute_position() const;
	Point4 get_relative_rotation() const;
	Point3 get_relative_scale() const;
	void set_relative_position(Point3 position);
	void set_relative_rotation(Point4 rotation);
	void set_relative_scale(Point3 scale);
	void copy_relative_params(Joint* toCopy);

	double *get_matrix_absolute() const;
	double *get_matrix_relative() const;
	double *get_matrix_inverse() const;
	double *get_matrix_finale() const;
	void compute_matrices();
	void compute_inverse_absolute_matrix();
	void display(bool display_matrices = true);
	void display_with_sons(bool display_matrices = true);

	void set_name(string name);
	string get_name();
	Joint* find(string name);
	vector<Animation*> animations;
	int selected_animation;
	void add_new_animation(Animation* animation) {
		animations.push_back(animation);
		selected_animation = animations.size() - 1;
	}
	void set_no_animation() {
		selected_animation = -1;
		animations.clear();
	}
	void set_next_animation() {
		selected_animation = (selected_animation + 1) % animations.size();
	}
	double get_selected_animation_speed();

private:
	static Joint* clone(Joint* toCopy, Joint* newFather);
	string name;
	Joint* father;
	vector<Joint*> children;

	Point3 relative_position;
	Point4 relative_rotation;
	Point3 relative_scale;
	Point3 absolute_position;

	double* matrix_absolute;
	double* matrix_relative;
	double* matrix_inverse;
	double* matrix_finale;

public:
	// stuff with the RTG files
	int type_of_drawin;
	string rtg_filename;
	//	Point3 rtg_position;
	//	double rtg_scale;
	void set_rtg_file(string filename) {//, Point3 pos, double scale) {
		type_of_drawin = DRAWING_MODE_RTG;
		rtg_filename = filename;
		//		rtg_position = pos;
		//		rtg_scale = scale;
	}
	void set_drawn_by_cylinder() {
		type_of_drawin = DRAWING_MODE_CYLINDERS;
	}
	void set_not_drawn() {
		type_of_drawin = DRAWING_MODE_NO_DRAWN;
	}

	//	 stuff with the weights
	string weight_folder;
	bool has_weights_image;
	Image* weights_image;
	double weight(const double& x, const double& y);
	void load_weights_image();
	void touch_weights_image();
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
template<class T>
static inline void transpose(T* A, T* rep, int size) {
	T buffer[size * size];
	for (int i = 0; i < size; ++i)
		for (int j = 0; j < size; ++j)
			buffer[size * i + j] = A[size * j + i];
	// copy the results
	for (int var = 0; var < size * size; ++var)
		rep[var] = buffer[var];
}

template<class T>
static inline float Determinant4f(const T m[16]) {
	return m[12] * m[9] * m[6] * m[3] - m[8] * m[13] * m[6] * m[3] - m[12]
			* m[5] * m[10] * m[3] + m[4] * m[13] * m[10] * m[3] + m[8] * m[5]
			* m[14] * m[3] - m[4] * m[9] * m[14] * m[3] - m[12] * m[9] * m[2]
			* m[7] + m[8] * m[13] * m[2] * m[7] + m[12] * m[1] * m[10] * m[7]
			- m[0] * m[13] * m[10] * m[7] - m[8] * m[1] * m[14] * m[7] + m[0]
			* m[9] * m[14] * m[7] + m[12] * m[5] * m[2] * m[11] - m[4] * m[13]
			* m[2] * m[11] - m[12] * m[1] * m[6] * m[11] + m[0] * m[13] * m[6]
			* m[11] + m[4] * m[1] * m[14] * m[11] - m[0] * m[5] * m[14] * m[11]
			- m[8] * m[5] * m[2] * m[15] + m[4] * m[9] * m[2] * m[15] + m[8]
			* m[1] * m[6] * m[15] - m[0] * m[9] * m[6] * m[15] - m[4] * m[1]
			* m[10] * m[15] + m[0] * m[5] * m[10] * m[15];
}

template<class T>
static inline bool inverse_matrix4f(const T in[16], T out[16]) {
	float det = Determinant4f(in);
	if (det == 0) {
		cout << "inverse_matrix4f() : input matrix is not inversible !" << endl;
		return false;
	}

	out[0] = (-in[13] * in[10] * in[7] + in[9] * in[14] * in[7] + in[13]
			* in[6] * in[11] - in[5] * in[14] * in[11] - in[9] * in[6] * in[15]
			+ in[5] * in[10] * in[15]) / det;
	out[4] = (in[12] * in[10] * in[7] - in[8] * in[14] * in[7] - in[12] * in[6]
			* in[11] + in[4] * in[14] * in[11] + in[8] * in[6] * in[15] - in[4]
			* in[10] * in[15]) / det;
	out[8] = (-in[12] * in[9] * in[7] + in[8] * in[13] * in[7] + in[12] * in[5]
			* in[11] - in[4] * in[13] * in[11] - in[8] * in[5] * in[15] + in[4]
			* in[9] * in[15]) / det;
	out[12] = (in[12] * in[9] * in[6] - in[8] * in[13] * in[6] - in[12] * in[5]
			* in[10] + in[4] * in[13] * in[10] + in[8] * in[5] * in[14] - in[4]
			* in[9] * in[14]) / det;
	out[1] = (in[13] * in[10] * in[3] - in[9] * in[14] * in[3] - in[13] * in[2]
			* in[11] + in[1] * in[14] * in[11] + in[9] * in[2] * in[15] - in[1]
			* in[10] * in[15]) / det;
	out[5] = (-in[12] * in[10] * in[3] + in[8] * in[14] * in[3] + in[12]
			* in[2] * in[11] - in[0] * in[14] * in[11] - in[8] * in[2] * in[15]
			+ in[0] * in[10] * in[15]) / det;
	out[9] = (in[12] * in[9] * in[3] - in[8] * in[13] * in[3] - in[12] * in[1]
			* in[11] + in[0] * in[13] * in[11] + in[8] * in[1] * in[15] - in[0]
			* in[9] * in[15]) / det;
	out[13] = (-in[12] * in[9] * in[2] + in[8] * in[13] * in[2] + in[12]
			* in[1] * in[10] - in[0] * in[13] * in[10] - in[8] * in[1] * in[14]
			+ in[0] * in[9] * in[14]) / det;
	out[2] = (-in[13] * in[6] * in[3] + in[5] * in[14] * in[3] + in[13] * in[2]
			* in[7] - in[1] * in[14] * in[7] - in[5] * in[2] * in[15] + in[1]
			* in[6] * in[15]) / det;
	out[6] = (in[12] * in[6] * in[3] - in[4] * in[14] * in[3] - in[12] * in[2]
			* in[7] + in[0] * in[14] * in[7] + in[4] * in[2] * in[15] - in[0]
			* in[6] * in[15]) / det;
	out[10] = (-in[12] * in[5] * in[3] + in[4] * in[13] * in[3] + in[12]
			* in[1] * in[7] - in[0] * in[13] * in[7] - in[4] * in[1] * in[15]
			+ in[0] * in[5] * in[15]) / det;
	out[14] = (in[12] * in[5] * in[2] - in[4] * in[13] * in[2] - in[12] * in[1]
			* in[6] + in[0] * in[13] * in[6] + in[4] * in[1] * in[14] - in[0]
			* in[5] * in[14]) / det;
	out[3] = (in[9] * in[6] * in[3] - in[5] * in[10] * in[3] - in[9] * in[2]
			* in[7] + in[1] * in[10] * in[7] + in[5] * in[2] * in[11] - in[1]
			* in[6] * in[11]) / det;
	out[7] = (-in[8] * in[6] * in[3] + in[4] * in[10] * in[3] + in[8] * in[2]
			* in[7] - in[0] * in[10] * in[7] - in[4] * in[2] * in[11] + in[0]
			* in[6] * in[11]) / det;
	out[11] = (in[8] * in[5] * in[3] - in[4] * in[9] * in[3] - in[8] * in[1]
			* in[7] + in[0] * in[9] * in[7] + in[4] * in[1] * in[11] - in[0]
			* in[5] * in[11]) / det;
	out[15] = (-in[8] * in[5] * in[2] + in[4] * in[9] * in[2] + in[8] * in[1]
			* in[6] - in[0] * in[9] * in[6] - in[4] * in[1] * in[10] + in[0]
			* in[5] * in[10]) / det;

	//	transpose(out, out, 4);
	return true;
}

/**
 * @param A
 * @param B
 * @param res = A * B - for matrices where the data are put column after column
 * @param Arows
 * @param Acols
 * @param Bcols
 */
template<class T>
static inline void multMatrices(T* A, T* B, T* res, int Arows, int Acols, int Bcols) {
	//	cout << "A" << endl;
	//	display_matrix(A, Acols, Arows);
	//	cout << "B" << endl;
	//	display_matrix(B, Bcols, Acols);

	/// empty res
	for (int var = 0; var < Arows * Bcols; ++var)
		res[var] = 0;

	for (int x = 0; x < Bcols; x++) {
		for (int y = 0; y < Arows; y++) {
			for (int k = 0; k < Acols; k++) {
				// res(x, y) += A(k, y) * B(x, k)
				res[x + y * Bcols] += A[k + y * Acols] * B[x + k * Bcols];
			}
		}
	}

	//	glMatrixMode(GL_MODELVIEW);
	//	glPushMatrix();
	//	glLoadIdentity();
	//	glMultMatrixd(A);
	//	glMultMatrixd(B);
	//	glGetDoublev(GL_MODELVIEW_MATRIX, res);
	//	glPopMatrix();
}

/*!
 * line after line matrices
 * @param mat
 * @param nCols
 * @param nRows
 */
template<class T>
static inline void display_matrix(T* mat, int nCols, int nRows) {
	const T* mat_ptr = mat;
	cout << setfill(' ');
	for (int j = 0; j < nRows; ++j) {
		for (int i = 0; i < nCols; ++i) {
			double value = mat_ptr[i + j * nCols];
			double value_f = (abs(value) < 1e-6 ? 0 : value);
			cout << setw(7) << setprecision(4) << value_f << "  ";
		} // end loop i
		cout << endl;
	} // end loop j
}

//const double IDENTITY4x4[16] = {//
//		1, 0, 0, 0, //
//				0, 1, 0, 0, //
//				0, 0, 1, 0, //
//				0, 0, 0, 1 };

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif /* JOINT_H_ */
