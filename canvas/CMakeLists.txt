cmake_minimum_required(VERSION 3.20)

project(canvas)

set(CMAKE_CXX_STANDARD 23)

include_directories(dependencies)

aux_source_directory(src SRC_LIST)
add_executable(canvas ${SRC_LIST})

target_compile_definitions(canvas PRIVATE
    PROJECT_PATH="${CMAKE_SOURCE_DIR}")

target_link_libraries(canvas
    ${CMAKE_SOURCE_DIR}/dependencies/glad/glad.lib
    ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui.lib
    glfw3
)
