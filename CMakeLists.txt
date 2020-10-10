cmake_minimum_required(VERSION 3.15)
project(ImageProcess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(OpenCV_DIR /home/wq/C++/opencv/installation/OpenCV_4.4/lib/cmake/opencv4)

find_package(OpenCV REQUIRED)

include_directories(SYSTEM ${OpenCV_INCLUDE_DIRS})

add_executable(ImageProcess HistogramEqualization.cpp)

# add libs you need
set(OpenCV_LIBS opencv_cudaimgproc opencv_core opencv_imgproc opencv_highgui opencv_imgcodecs opencv_video opencv_ximgproc opencv_xfeatures2d)

# linking
target_link_libraries(ImageProcess ${OpenCV_LIBS})