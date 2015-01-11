/*
 * test.cpp
 *
 *  Created on: Jan 12, 2010
 *      Author: arnaud
 */
#include "Animation.h"
#include "JointShower.h"
#include "Drawing.h"

void test_image() {
  Image* b = new Image(//
                       "textures/skin/front_left_elbow.bmp"
                       //				"textures/grass.bmp" //
                       );
  cout << b->get(15, 15).toString() << endl;
}

void test_man() {
  init_jointshower(800, 600);
  Man* man = new Man();
  man->add_new_animation(new Still(man));
  man->add_new_animation(new ShakeArms(man));
  set_skeleton(man);

  start_jointshower();
}

void test_player() {
  init_jointshower(800, 600);
  Player* player = new Player();
  player->add_new_animation(new Still(player));
  player->add_new_animation(new ShakeArms(player));
  player->add_new_animation(new ShakeArms2(player));
  set_skeleton(player);

  start_jointshower();
}

void test_matrices() {
  double M1[16] = { 1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 3, 0, 0, 0, 0, 4 },
      *M1_ptr = M1;
  double M2[16] = { 1, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
      *M2_ptr = M2;
  double M3[16] = { 1, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1 },
      *M3_ptr = M3;
  double vec[4] = { 1, 2, 3, 4 }, *vec_ptr = vec;
  double vec2[4] = { 0, 0, 0, 0 }, *vec2_ptr = vec2;
  double res[16];

  // M1 * M2
  multMatrices(M1_ptr, M2_ptr, res, 4, 4, 4);
  cout << "mult - res:";
  display_matrix(res, 4, 4);
  cout << endl << "M1:";
  display_matrix(M1, 4, 4);
  cout << endl << "M2:";
  display_matrix(M2, 4, 4);
  cout << endl;

  // transpose
  transpose(M2, res, 4);
  cout << "transpose - res:";
  display_matrix(res, 4, 4);
  cout << endl << "M2:";
  display_matrix(M2, 4, 4);
  cout << endl;

  // M3^-1
  inverse_matrix4f(M3, res);
  cout << "inv - res:";
  display_matrix(res, 4, 4);
  cout << endl << "M3:";
  display_matrix(M3, 4, 4);
  cout << endl;

  // vec * M3
  multMatrices(M3_ptr, vec_ptr, vec2_ptr, 4, 4, 1);
  cout << "vec mult - vec2:";
  display_matrix(vec2, 4, 1);
  cout << endl << "vec:";
  display_matrix(vec, 4, 1);
  cout << endl << "M3:";
  display_matrix(M3, 4, 4);
  cout << endl;
}

int main(int argc, char **argv) {
  cout << "main" << endl;

  //	test_horse();
  //	test_image();
  //	test_matrices();

  //test_man();
  test_player();

  cout << "end of main" << endl;
}
