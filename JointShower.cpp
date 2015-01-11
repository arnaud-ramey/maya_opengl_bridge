/*
 * JointShower.cpp
 *
 *  Created on: Apr 22, 2010
 *      Author: arnaud
 */

/*! the position of the camera */
float X = 30.f, Y = 10.f, Z = 30.f;

/*! the direction of the look */
float lx = -1.0f, ly = 0.0f, lz = -1.0f;

/*! the params of the window */
int width, height;

/*! the time */
double t = 0, t_start = 0;
double t_anim = 0, t_anim_shift;
bool anim_running = true;

/*! colors */
float colorRed[] = { 1.0f, 0.0f, 0.0f, 1.0f };
float colorBlue[] = { 0.0f, 0.0f, 1.0f, 1.0f };
float colorGreen[] = { 0.0f, 1.0f, 0.0f, 1.0f };
float colorWhite[] = { 0.0f, 1.0f, 0.0f, 1.0f };

#include "JointShower.h"

int drawing_mode = DRAWING_MODE_CYLINDERS;

/* the objects */
Joint* skeleton = NULL;
double skeleton_theta = 0, skeleton_last_t_anim = 0;

#include "Movement.cpp"
#include "Drawing.cpp"

#if UV_IMAGE
#include "CImg.h"
using namespace cimg_library;
CImg<unsigned char> cimage(256, 256, 1, 3, 0);
#endif // UV_IMAGE
//
// Create light components
GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 0.3f };
GLfloat diffuseLight[] = { 1.f, 1.f, 1., 1.0f };
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat position[] = { 0.f, 15.f, 15.f, 1 };

void set_lights() {
  // draw the light
  glDisable(GL_LIGHTING);
  glPushMatrix();
  reload_view();
  glTranslatef(position[0], position[1], position[2]);
  glColor3f(1, 1, 0);
  glutSolidSphere(1, 10, 10);
  glPopMatrix();

  // Assign created components to GL_LIGHT0
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT0, GL_POSITION, position);

  /* set a light */
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_COLOR_MATERIAL);
}

void set_skeleton(Joint* b) {
  skeleton = b;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/*!
 * @param pos_in the absolute position of the point in the skeleton
 * @param pos_out the output position of the point
 * @param x_uv
 * @param y_uv
 * @param skeleton
 * @param sum_weights
 * @param verbose
 */
inline static void compute_pos_one_node(Point3* pos_in, Point3* pos_out,
                                        Joint* skeleton, double weight, double& sum_weights, bool verbose) {
  // incr the sum of the weights
  sum_weights += weight;
  if (verbose)
    cout << skeleton->get_name() << " : weight:" << weight << endl;

  if (weight != 0) {
    // make the matricial product
    double* m = skeleton->get_matrix_finale();
    //	pos_out.x += (pos_in.x * m[0] + pos_in.y * m[4] + pos_in.z * m[8] + m[12])
    //			* weight;
    //	pos_out.y += (pos_in.x * m[1] + pos_in.y * m[5] + pos_in.z * m[9] + m[13])
    //			* weight;
    //	pos_out.z += (pos_in.x * m[2] + pos_in.y * m[6] + pos_in.z * m[10] + m[14])
    //			* weight;
    double pos_in_arr[4] = { pos_in->x, pos_in->y, pos_in->z, 1 };
    double *pos_in_arr_ptr = pos_in_arr;
    double evol[4];

    //		cout << skeleton->get_name() << " : pos_in_arr:" << endl;
    //		display_matrix(pos_in_arr_ptr, 4, 1);
    //		cout << skeleton->get_name() << " : m (finale):" << endl;
    //		display_matrix(m, 4, 4);

    /// compute m * pos_in
    multMatrices(m, pos_in_arr_ptr, evol, 4, 4, 1);

    //		if (verbose) {
    //			cout << skeleton->get_name() << " : pos_in:" << pos_in->toString()
    //					<< endl;
    //			cout << skeleton->get_name() << " : pos_in_arr:" << endl;
    //			display_matrix(pos_in_arr_ptr, 4, 1);
    //			cout << skeleton->get_name() << " : m (finale):" << endl;
    //			display_matrix(m, 4, 4);
    //			cout << skeleton->get_name() << " : evol:" << endl;
    //			display_matrix(evol, 4, 1);
    //		}

    pos_out->x += weight * evol[0] / evol[3];
    pos_out->y += weight * evol[1] / evol[3];
    pos_out->z += weight * evol[2] / evol[3];
  } // end if weight != 0
}

inline static void compute_pos_rec(Point3* pos_in, Point3* pos_out,
                                   double x_uv, double y_uv, Joint* skeleton, double& sum_weights,
                                   bool verbose) {
  //	cout << "current:" << skeleton->get_name() << endl;

  // get the weight
  double weight = skeleton->weight(x_uv, y_uv);
  compute_pos_one_node(pos_in, pos_out, skeleton, weight, sum_weights,
                       verbose);

  // compute the position for the children of the skeleton
  vector<Joint*> children = skeleton->get_children();
  for (vector<Joint*>::iterator it = children.begin(); it < children.end(); ++it) {
    compute_pos_rec(pos_in, pos_out, x_uv, y_uv, *it, sum_weights, verbose);
  }
}

inline static void compute_pos(Point3* pos_in, Point3* pos_out, double x_uv,
                               double y_uv, Joint* skeleton, bool verbose) {
  if (verbose) {
    cout << "pos_in=" << pos_in->toString() << ", ";
    cout << "uv=(" << x_uv << ", " << y_uv << ")" << endl;
  }

  //	Point3 pos = skeleton->get_absolute_position();
  //	glTranslated(pos.x, pos.y, pos.z);

  double sum_weights = 0;
  pos_out->x = 0;
  pos_out->y = 0;
  pos_out->z = 0;
  compute_pos_rec(pos_in, pos_out, x_uv, y_uv, skeleton, sum_weights, verbose);

  //
#if UV_IMAGE // draw the weight on the image
  int x_pix = (1.f - x_uv) * cimage.width();
  int y_pix = (1.f - y_uv) * cimage.height();
  const unsigned char red[] = {255, 0, 0};
  const unsigned char green[] = {0, 255, 0};
  const unsigned char blue[] = {0, 0, 255};
  const unsigned char white[] = {255, 255, 255};
  //cimage.draw_point(x_pix, y_pix, color);
  const unsigned char* color = (sum_weights == 0 ? white
                                                 : (sum_weights <= 255 ? red : (sum_weights <= 512 ? green : blue)));
  cimage.draw_circle(x_pix, y_pix, 3, color);
#endif // UV_IMAGE
  //


  if (sum_weights != 0) {
    pos_out->x = 1. * pos_out->x / sum_weights;
    pos_out->y = 1. * pos_out->y / sum_weights;
    pos_out->z = 1. * pos_out->z / sum_weights;
  } else {
    //		cout << "Weird ! Point " << pos_in->toString() << " has no weights..."
    //				<< endl;
    // give a weight of 1 to the root
    compute_pos_one_node(pos_in, pos_out, skeleton, 1, sum_weights, verbose);
  }

  if (verbose) {
    cout << "pos_in=" << pos_in->toString() << endl;
    cout << "pos_out=" << pos_out->toString() << endl << endl;
  }
}

void drawObject(RTGFile *file, RTGObject *obj, bool useTextures) {
  unsigned int i, j;
  RTGVertex *v;
  RTGVec3 *normal;
  RTGVec3 *point;
  RTGVec2 *tex_coords;

  glColor3f(1, 1, 1);
  if (useTextures)
    setMaterial(file->mat[obj->mat]);

  glPolygonMode(GL_FRONT, GL_FILL);

  /// locate in 0,0,0
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  reload_view();
  //	glLoadIdentity();

  for (i = 0; i < obj->npoly; i++) {
    glBegin(GL_POLYGON);
    for (j = 0; j < obj->poly[i]->nvert; j++) {
      v = &obj->poly[i]->v[j];

      // apply the noram
      normal = &obj->norm[v->n];
      glNormal3f(normal->x, normal->y, normal->z);

      // apply the tex coords
      tex_coords = &obj->tex[v->t];
      glTexCoord2f(tex_coords->x, tex_coords->y);
      //			cout << tex_coords->x << ", " << tex_coords->y << endl;

      // find the vertex coords and transform them to match the skeleton
      point = &obj->pos[v->p];

      // normal mode
      //			glVertex3f(point->x, point->y, point->z);

      // skeleton
      Point3 pos_in(point->x, point->y, point->z), pos_out;
      compute_pos(&pos_in, &pos_out, tex_coords->x, tex_coords->y,
                  skeleton, DEBUG);
      glVertex3f(pos_out.x, pos_out.y, pos_out.z);

    } // end loop vertices
    glEnd(); // polygons
  } // end loop polygons

#if UV_IMAGE
  cimage.save("out.gif");
#endif // UV_IMAGE
  glPopMatrix();

  unload_texture_unit(TEX_UNIT_1);
}

void drawNode(RTGFile *file, RTGNode *node, bool useTextures) {
  unsigned int i;

  /* Om noden har geometri, rita den */
  if (node->obj) {
    drawObject(file, node->obj, useTextures);
  }
  /* Om noden har barn, rita dem */
  for (i = 0; i < node->nchildren; i++) {
    drawNode(file, node->children[i], useTextures);
  }

}

/*!
 *
 * @param file
 * @param useTextures
 * @param x
 * @param y
 * @param z
 * @param scale
 * @param angle
 */
void drawRTG(RTGFile *file, bool useTextures, Point3 pos, double scale,
             double angle_rad) {

  if (DEBUG)
    cout << endl << "drawRTG()" << endl;
  unsigned int i;

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  reload_view();
  /* Applicera transformation här! */
  glTranslated(pos.x, pos.y, pos.z);
  glScaled(scale, scale, scale);
  //	glRotated(angle, 0, 1, 0);

  // rotate the good angle
  //	glRotatef(90, 0, 1, 0);
  //	glRotatef(angle_rad * 180.f / PI - 90.f, 0, 0, 1);

  /* Draw all the top level nodes */
  for (i = 0; i < file->nnodes; i++) {
    drawNode(file, file->nodes[i], useTextures);
  }

  glPopMatrix();
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void draw_node_at_current_pos(Joint* node) {
  /*
   * determine what kind of drawing we need
   */
  int this_drawing_mode = DRAWING_MODE_NO_DRAWN;
  if (drawing_mode == DRAWING_MODE_NO_DRAWN) {
    this_drawing_mode = DRAWING_MODE_NO_DRAWN;
  } else if (drawing_mode == DRAWING_MODE_AXES) {
    this_drawing_mode = DRAWING_MODE_AXES;
  } else if (drawing_mode == DRAWING_MODE_CYLINDERS) {
    if (node->type_of_drawin == DRAWING_MODE_NO_DRAWN)
      this_drawing_mode = DRAWING_MODE_NO_DRAWN;
    else
      this_drawing_mode = DRAWING_MODE_CYLINDERS;
  } else if (drawing_mode == DRAWING_MODE_RTG) {
    if (node->type_of_drawin == DRAWING_MODE_RTG)
      this_drawing_mode = DRAWING_MODE_RTG;
    else
      this_drawing_mode = DRAWING_MODE_NO_DRAWN;
  }

  /*
   * draw really
   */
  glMatrixMode(GL_MODELVIEW);
  if (this_drawing_mode == DRAWING_MODE_NO_DRAWN) {
    // do nothing
  } //
  else if (this_drawing_mode == DRAWING_MODE_AXES)
    draw_axes_here();

  else if (this_drawing_mode == DRAWING_MODE_CYLINDERS) {
    // draw a cylinder
    //		glutSolidTeapot(5.f);
    Point3 begin = node->get_absolute_position();
    vector<Joint*> children = node->get_children();
    for (vector<Joint*>::iterator it = children.begin(); it
         < children.end(); ++it) {
      Point3 end = (*it)->get_absolute_position();
      double dist = begin.dist(end);
      glPushMatrix();
      reload_view();
      //			glTranslated(skeleton_position.x, skeleton_position.y,
      //					skeleton_position.z);
      //			glRotated(skeleton_rotation.w, skeleton_rotation.x,
      //					skeleton_rotation.y, skeleton_rotation.z);
      glColor3f(1, 1, 1);
      renderCylinder_convenient(begin, end, dist / 6.f, 10);
      glPopMatrix();
    }
  } //
  else if (this_drawing_mode == DRAWING_MODE_RTG) {
    LoadedRTG* h = get_rtg_file_by_name(node->rtg_filename);
    //		Point3 pos = node->rtg_position;
    //		drawRTG(h->rtg_file, true, pos, node->rtg_scale, 0);
    drawRTG(h->rtg_file, true, Point3(0, 0, 0), 1, 0);
  }
}

void draw_node_and_sons(Joint* node) {
  //	node->display();

  //// locate to the good position
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  reload_view();

  //	glTranslated(skeleton_position.x, skeleton_position.y, skeleton_position.z);
  //	glRotated(skeleton_rotation.w, skeleton_rotation.x, skeleton_rotation.y,
  //			skeleton_rotation.z);

  //glMultMatrixd(node->get_matrix_absolute());
  double absolute_trans[16], *absolute_trans_ptr = absolute_trans;
  transpose(node->get_matrix_absolute(), absolute_trans_ptr, 4);
  glMultMatrixd(absolute_trans);
  draw_node_at_current_pos(node);
  glPopMatrix();

  //// draw the children
  vector<Joint*> children = node->get_children();
  for (vector<Joint*>::iterator ptr = children.begin(); ptr < children.end(); ++ptr) {
    draw_node_and_sons(*ptr);
  }
}

void draw_body() {
  /*
   * draw the skeleton
   */
  // load the animation
  if (skeleton != NULL) {
    int anim_id = skeleton->selected_animation;
    if (anim_id != -1)
      skeleton->animations.at(anim_id)->anim(t_anim);
    draw_node_and_sons(skeleton);
  }
}

void draw_symbols() {
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  reload_view();
  // draw the cube for anim on / off
  if (anim_running == false)
    glColor3f(1, 0, 0);
  else
    glColor3f(0, 1, 0);
  glTranslatef(4, 1, -4);
  glutSolidCube(2);

  // draw the sphere for the speed
  reload_view();
  glColor3f(0, 1, 1);
  if (skeleton->selected_animation == 1)
    glColor3f(1, 0, 1);
  else if (skeleton->selected_animation == 2)
    glColor3f(1, 1, 0);
  glTranslatef(-4, 1, 4);
  glutSolidSphere(1, 12, 12);
  glPopMatrix();
}

void draw_scene() {
  //	cout << "draw_scene()" << endl;
  cout << '^';

  draw_sky();
  set_lights();
  draw_background();
  draw_axes();
  draw_symbols();
  draw_body();

  //	if (SAVE_IMAGES)
  //		screenShot();
}

void update_time() {
  struct timeval now;
  gettimeofday(&now, NULL);
  t = now.tv_sec + now.tv_usec / 1E6 - t_start;
  if (t_start == 0)
    t_start = t;

  // update the animation time
  if ((skeleton != NULL && skeleton->selected_animation == 0) || anim_running
      == false) {
    t_anim_shift = -1;
  } else {
    if (t_anim_shift == -1)
      t_anim_shift = t - t_anim;
    t_anim = t - t_anim_shift;
  }
}

void display() {
  //	cout << "display()" << endl;
  // incr time
  //t += .1;
  glEnable(GL_DEPTH_TEST);

  update_time();
  draw_scene();

  glutSwapBuffers();

  //	double time = ((double) cvGetTickCount() - timer) / freq;
  //	cout << "Time for OpenGL rendering :" << time << endl;
}

void my_exit() {
  cout << endl << "End of the program" << endl;
  delete skeleton;
}

void init_jointshower(int w, int h) {
  //	glutInit(&argc, argv);
  printf("Keys:\n");
  printf("' ' : change animation\n");
  printf("'s' : toggle animation\n");
  printf("'r' : change rendering\n");
  printf("'8' : camera go forward\n");
  printf("'2' : camera go backward\n");
  printf("'1' : camera strafe left\n");
  printf("'3' : camera strafe right\n");
  printf("'4' : camera rotate left\n");
  printf("'6' : camera rotate right\n");
  printf("'+' : camera go higher\n");
  printf("'-' : camera go lower\n");
  printf("'q' : quit\n");
  printf("\n");

  int i = 0;
  glutInit(&i, NULL);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
  width = w;
  height = h;
  glutInitWindowSize(width, height);
  glutCreateWindow("openGL_effects");

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(display);

  createGLUTMenus();

  atexit(*my_exit);
}

void start_jointshower() {
  glutMainLoop();
}

