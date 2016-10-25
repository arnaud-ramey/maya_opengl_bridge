                  +----------------------+
                  |  maya_opengl_bridge  |
                  +----------------------+

[![Build Status](https://travis-ci.org/arnaud-ramey/maya_opengl_bridge.svg)](https://travis-ci.org/arnaud-ramey/maya_opengl_bridge)

A realistic way to animate a horse in an OpenGL environment.

License :                  see the LICENSE file.
Authors :                  see the AUTHORS file.
How to build the program:  see the INSTALL file.

There are several reasons to use a 3D modeler instead of the OpenGL
primitives (cubes, cylinders, etc.).
* The 3D modeler is made for designing complex objects in 3D, with creative
and powerful tools ;
* There are plenty of public 3D objects available on the Internet, made by
skillful artists ;
* The OpenGL primitives don't give a convincing visual aspect, because of
there limited possibilities.

However, it is interesting to be able to use the objects designed in the 3D
modeler outside of it, typically in a programming environment (here C++ with
OpenGL). This enables some user inputs and experience, instead of a static
animation (for instance, selection of the point of view).

Here the horse is structured as a skeleton, where each bone is drawn by
itself, drawing a RTG object at its position. The RTG objects are exported
directly from Maya. The menus are made with the GLUT routines. The whole
program was made in an Unix environment.

________________________________________________________________________________

How to use the program
________________________________________________________________________________
To display the help, just launch in a terminal .
It will display the help of the program.

Keys:
  ' ' : change animation
  's' : toggle animation
  'r' : change rendering
  '8' : camera go forward
  '2' : camera go backward
  '1' : camera strafe left
  '3' : camera strafe right
  '4' : camera rotate left
  '6' : camera rotate right
  '+' : camera go higher
  '-' : camera go lower
  'q' : quit
