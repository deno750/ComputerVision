cmake_minimum_required(VERSION 2.8)

project(ComputerVision)
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(
	Lab1_rgb
	src/Lab1/lab1_rgb.cpp
)

add_executable(
    Lab1_hsv
    src/Lab1/lab1_hsv.cpp
)

add_executable(
    Lab2
    src/Lab2/lab2.cpp
)

add_executable(
    Lab3
    src/Lab3/lab3.cpp
)

add_library(Filter
    src/Lab3/filter.cpp
    src/Lab3/filter.h
)

add_executable(
    Lab4
    src/Lab4/lab4.cpp
)

add_executable(
    Lab5
    src/Lab5/lab5.cpp
    src/Lab5/panoramic_utils.h
    src/Lab5/panoramic_image.cpp
    src/Lab5/panoramic_image.h
)


add_executable(
    Lab6
    src/Lab6/lab6.cpp
)

target_link_libraries(Lab1_rgb ${OpenCV_LIBS})
target_link_libraries(Lab1_hsv ${OpenCV_LIBS})
target_link_libraries(Lab2 ${OpenCV_LIBS})
target_link_libraries(Lab3 ${OpenCV_LIBS})
target_link_libraries(Lab3 Filter)
target_link_libraries(Lab4 ${OpenCV_LIBS})
target_link_libraries(Lab5 ${OpenCV_LIBS})
target_link_libraries(Lab6 ${OpenCV_LIBS})
