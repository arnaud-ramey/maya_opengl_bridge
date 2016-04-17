cmake_minimum_required(VERSION 2.8)
project(maya_opengl_bridge)
# Debug, Release, RelWithDebInfo and MinSizeRe
set(CMAKE_BUILD_TYPE RelWithDebInfo)
SET(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra") # add extra warnings

find_package(GLUT REQUIRED)
#~ find_package(JPEG REQUIRED)
#~ find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)
#~ find_package(Threads REQUIRED)
include_directories(libs)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR})
CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/maya_opengl_bridge_path.h.in"
               "${PROJECT_BINARY_DIR}/maya_opengl_bridge_path.h")

add_library(maya_opengl_bridge
  libs/bitmap/Bitmap.cpp
  libs/rtg/RTGlib.c
  Animation.cpp
  Joint.cpp
  Man.cpp
  Player.cpp
  JointShower.cpp
  Image.cpp)
add_executable(test_maya_opengl_bridge test.cpp)
target_link_libraries(test_maya_opengl_bridge maya_opengl_bridge
  ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${OPENGL_LIBRARIES}
  ${JPEG_LIBRARIES} ${GLUT_LIBRARIES})
