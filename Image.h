/*
 * Image.h
 *
 *  Created on: Apr 25, 2010
 *      Author: arnaud
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include "Includes.h"
#include "Vertex.h"
#include "CImg.h"
using namespace cimg_library;

class Image {
public:
	Image(string filename);
	virtual ~Image();

	CImg<unsigned char>* cimg;
	inline Point3 get(int x, int y) {
		Point3 ans;
		ans.x = (int) *cimg->data(x, y, 0);
		ans.y = (int) *cimg->data(x, y, (cimg->depth() == 3 ? 1 : 0));
		ans.z = (int) *cimg->data(x, y, (cimg->depth() == 3 ? 2 : 0));
		return ans;
	}
	inline int width() {
		return cimg->width();
	}
	inline int height() {
		return cimg->height();
	}
};

#endif /* IMAGE_H_ */
