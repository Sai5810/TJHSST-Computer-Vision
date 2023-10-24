cmake_minimum_required(VERSION 3.21)
project(Circle_Detection_with_OpenCV)
set(CMAKE_CXX_STANDARD 11)
#find_package(OpenCV REQUIRED)

#things to uncomment when submitting
add_subdirectory(opencv)
include_directories(opencv/include)
####################################


add_executable(Circle_Detection_with_OpenCV l071.cpp)
target_link_libraries(Circle_Detection_with_OpenCV opencv_core opencv_highgui opencv_imgproc)
