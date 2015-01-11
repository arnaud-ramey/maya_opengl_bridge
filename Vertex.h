/*
 * Vertex.h
 *
 *  Created on: Apr 23, 2010
 *      Author: arnaud
 */

#ifndef VERTEX_H_
#define VERTEX_H_

class Point2 {
public:
	double x, y;
	Point2() {
	}
	Point2(double x, double y) {
		this->x = x;
		this->y = y;
	}
	inline string toString() {
		ostringstream ans;
		ans << "(" << x << ", " << y << ")";
		return ans.str();
	}
	inline Point2 clone() {
		return Point2(x, y);
	}
};

class Point3 {
public:
	double x, y, z;
	Point3() {
	}
	Point3(double x, double y, double z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	inline string toString() {
		ostringstream ans;
		ans << "(" << x << ", " << y << ", " << z << ")";
		return ans.str();
	}
	inline void display() {
		cout << "(" << x << ", " << y << ", " << z << ")";
	}
	inline Point3 clone() {
		return Point3(x, y, z);
	}
	inline double dist(const Point3 p) {
		return sqrt( pow(x - p.x, 2) + pow(y - p.y, 2) + pow(z - p.z, 2));
	}
};

class Point4 {
public:
	double w, x, y, z;
	Point4() {
	}
	Point4(double w, double x, double y, double z) {
		this->w = w;
		this->x = x;
		this->y = y;
		this->z = z;
	}
	inline string toString() {
		ostringstream ans;
		ans << "(" << w << ", " << x << ", " << y << ", " << z << ")";
		return ans.str();
	}

	inline Point4 clone() {
		return Point4(w, x, y, z);
	}
};

#endif /* VERTEX_H_ */
