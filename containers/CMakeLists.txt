cmake_minimum_required(VERSION 3.20)

project(containers)

file(GLOB SOURCES src/*.cpp)

add_library(containers STATIC ${SOURCES})
