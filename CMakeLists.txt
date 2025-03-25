cmake_minimum_required(VERSION 3.30)
project(DA2425_PRJ1_G75)

set(CMAKE_CXX_STANDARD 23)

add_executable(DA2425_PRJ1_G75 src/Main/main.cpp
        src/Main/data_structures/createGraphs.cpp
        src/Main/data_structures/createGraphs.h
        src/Main/Modes/driving.h
)
