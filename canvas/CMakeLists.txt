cmake_minimum_required(VERSION 3.20)

project(canvas)

set(CMAKE_CXX_STANDARD 23)

find_package(glad CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

file(GLOB SOURCES src/*.cpp)
add_executable(canvas ${SOURCES})

target_compile_definitions(canvas PRIVATE
    PROJECT_PATH="${CMAKE_SOURCE_DIR}")

target_link_libraries(canvas
    glad::glad
    imgui::imgui
    glfw
)
