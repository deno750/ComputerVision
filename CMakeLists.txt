cmake_minimum_required(VERSION 2.8)

project(ComputerVision)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(
	lab1_rgb
	src/Lab1/lab1_rgb.cpp
)

add_executable(
    lab1_hsv
    src/Lab1/lab1_hsv.cpp
)



target_link_libraries(lab1_rgb ${OpenCV_LIBS})
target_link_libraries(lab1_hsv ${OpenCV_LIBS})
