cmake_minimum_required(VERSION 3.12)
project(ImageProcess)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(OpenCV_DIR /Users/zhaoyue/opencv/installation/OpenCV-master/lib/cmake/opencv4)

find_package(OpenCV REQUIRED)

include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})

#add_executable(ImageProcess ErodingAndDilating.cpp)
add_executable(ImageProcess MORPH_TOPHAT.cpp)

# add libs you need
set(OpenCV_LIBS opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs)

# linking
target_link_libraries(ImageProcess ${OpenCV_LIBS})