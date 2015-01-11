/*
 * Image.cpp
 *
 *  Created on: Apr 25, 2010
 *      Author: arnaud
 */

#include "Image.h"

Image::Image(string filename) {
	//	cout << "Image::Image(" << filename << ")" << endl;
	cimg = new CImg<unsigned char> (filename.c_str());
	//	cout << width() << ", " << height() << endl;
}

Image::~Image() {
	delete cimg;
}
