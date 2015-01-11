/*
 * Drawing.cpp
 *
 *  Created on: Jan 24, 2010
 *      Author: arnaud
 *
 * drawing functions
 */

#include "Includes.h"
#include "Drawing.h"
#include "Textures.cpp"

static void draw_cylinder(Point3 origin, double rotation_radians,
                          double diameter, double length) {
    glPushMatrix();
    // go to the origin
    glTranslatef(origin.x, origin.y, origin.z);
    // rotate the good angle
    glRotatef(90, 0, 1, 0);
    glRotatef(-rotation_radians * 180.f / PI, 1, 0, 0);
    // draw the cylinder
    GLUquadric *myQuad = gluNewQuadric();
    gluCylinder(myQuad, diameter, diameter, length, 10, 10);
    gluDeleteQuadric(myQuad);
    glPopMatrix();
}

void renderCylinder(float x1, float y1, float z1, float x2, float y2, float z2,
                    float radius, int subdivisions, GLUquadricObj *quadric) {
    float vx = x2 - x1;
    float vy = y2 - y1;
    float vz = z2 - z1;

    //handle the degenerate case of z1 == z2 with an approximation
    if (vz == 0)
        vz = .0001;

    float v = sqrt(vx * vx + vy * vy + vz * vz);
    float ax = 57.2957795 * acos(vz / v);
    if (vz < 0.0)
        ax = -ax;
    float rx = -vy * vz;
    float ry = vx * vz;
    glPushMatrix();

    //draw the cylinder body
    glTranslatef(x1, y1, z1);
    glRotatef(ax, rx, ry, 0.0);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluCylinder(quadric, radius, radius, v, subdivisions, 1);

    //draw the first cap
    gluQuadricOrientation(quadric, GLU_INSIDE);
    gluSphere(quadric, radius, subdivisions, subdivisions);
    //	gluDisk(quadric, 0.0, radius, subdivisions, 1);

    //draw the second cap
    glTranslatef(0, 0, v);
    gluQuadricOrientation(quadric, GLU_OUTSIDE);
    gluDisk(quadric, 0.0, radius, subdivisions, 1);
    glPopMatrix();
}

void renderCylinder_convenient(Point3 p1, Point3 p2, float radius,
                               int subdivisions) {
    //the same quadric can be re-used for drawing many cylinders
    GLUquadricObj *quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    renderCylinder(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, radius, subdivisions,
                   quadric);
    gluDeleteQuadric(quadric);
}

static void draw_axes_here() {
    glDisable(GL_LIGHTING);
    glLineWidth(2.0);
    int size = 1.f;
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); // X axis is red.
    glVertex3f(0, 0, 0);
    glVertex3f(size, 0, 0);
    glColor3f(0, 1, 0); // Y axis is green.
    glVertex3f(0, 0, 0);
    glVertex3f(0, size, 0);
    glColor3f(0, 0, 1); // z axis is blue.
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, size);
    glEnd();
    glEnable(GL_LIGHTING);
}

/*!
 * draw the axis arrow in three colors
 */
static void draw_axes(void) {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    reload_view();
    //glTranslatef(0, 0, 0);
    draw_axes_here();
    glPopMatrix();
}

class HillLocation {
public:
    int x;
    int z;
    int deepness;
    int radius;
    float color;
};

class TreeLocation {
public:
    Point3 origin;
    int height;
    int green_size;
    float color;
};

vector<HillLocation> hills;
vector<TreeLocation> trees;

static inline void draw_hill(HillLocation* h) {
    glPushMatrix();
    glTranslated(h->x, -h->deepness, h->z);
    glColor3f(0, h->color, 0); // green
    glutSolidSphere(h->radius, 20, 20);
    glPopMatrix();
}

static inline void draw_tree(TreeLocation* h) {
    glPushMatrix();
    glColor3f(.2, 0, 0); // red
    draw_cylinder(h->origin, PI / 2, h->height / 10, h->height);
    glTranslatef(h->origin.x, h->origin.y + h->height, h->origin.z);
    glColor3f(0, h->color, 0); // green
    glutSolidSphere(h->green_size, 20, 20);
    glPopMatrix();
}

static void draw_background() {
    // draw the ground -> grass
    glColor3f(0, 1, 0); // green
    int diam = 30;
    int y_floor = 0;
    load_texture("grass.bmp", TEX_UNIT_1);
    glPushMatrix();
    reload_view();
    glBegin(GL_QUADS);
    glTexCoord2d(0, 0);
    glVertex3f(-diam, y_floor, -diam);
    glTexCoord2d(0, 1);
    glVertex3f(-diam, y_floor, +diam);
    glTexCoord2d(1, 1);
    glVertex3f(+diam, y_floor, +diam);
    glTexCoord2d(1, 0);
    glVertex3f(+diam, y_floor, -diam);
    glNormal3f(0, 1, 0);
    glEnd();
    glPopMatrix();
    unload_texture_unit(TEX_UNIT_1);
}

static void draw_sky() {
    /* draw the sky */
    glClearColor(0.5f, 0.5f, 1.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void setMaterial(RTGMaterial *material) {
    /* Sätt material här */
    if (material->texFile != NULL) {
        load_texture(material->texFile, TEX_UNIT_1);
    }
    //	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->amb);
    //	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->dif);
    //	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->spc);
    //	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shn);
    //	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material->em);
    //glColor3fv(material->amb);
}

/*!
 * where we store the heroes
 */
vector<LoadedRTG> loaded_rtgs;

/*!
 * find the index of the hero in the vector
 * @return -1 if not found
 */
static int find_hero_index(string filename) {
    int rep = 0;
    for (vector<LoadedRTG>::iterator it = loaded_rtgs.begin(); it
         < loaded_rtgs.end(); ++it) {
        if (it->filename.find(filename) != string::npos)
            return rep;
        ++rep;
    }
    return -1;
}

/*!
 * draw the hero
 */
LoadedRTG* get_rtg_file_by_name(string filename) {
    int index = find_hero_index(filename);
    // load if needed
    if (index == -1) {
        // create filename
        ostringstream filename_full;
        filename_full << RTG_FOLDER << filename;

        // display
        cout << "RTG file not loaded ! '" << filename_full.str()
             << "'. Loading it !" << endl;

        LoadedRTG h(filename_full.str());
        loaded_rtgs.push_back(h);
        index = loaded_rtgs.size() - 1;
    }
    return &loaded_rtgs.at(index);
}

