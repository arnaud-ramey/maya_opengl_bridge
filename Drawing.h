/*
 * Drawing.h
 *
 *  Created on: Apr 26, 2010
 *      Author: arnaud
 */

#ifndef DRAWING_H_
#define DRAWING_H_

#define  TEX_UNIT_1				GL_TEXTURE0_ARB
#define  TEX_UNIT_2				GL_TEXTURE1_ARB

class LoadedRTG {
public:
	string filename;
	RTGFile *rtg_file;

	LoadedRTG(string filename) {
		this->filename = filename;
		rtg_file = RTG_Parse(filename.c_str(), 1); // verbose
	}
};

LoadedRTG* get_rtg_file_by_name(string filename);
void setMaterial(RTGMaterial *material);
void load_texture(int idTex, int idTexUnit);
void unload_texture_unit(int idTexUnit);

#endif /* DRAWING_H_ */
