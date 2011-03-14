project(FaceInvaders CXX)
cmake_minimum_required(VERSION 2.6)

#set(CMAKE_BUILD_TYPE RELEASE)

find_package( OpenCV REQUIRED )

add_executable( faceinvaders faceinvaders.cpp invader.cpp player.cpp shot.cpp )
target_link_libraries( faceinvaders ${OpenCV_LIBS} )