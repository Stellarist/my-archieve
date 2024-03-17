cmake_minimum_required(VERSION 3.15)

project(canvas)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 
    ${CMAKE_SOURCE_DIR}/bin)

include_directories(dependencies)

aux_source_directory(src SRC_LIST)
add_executable(canvas ${SRC_LIST})

add_compile_options(-Wall -Werror)
target_compile_definitions(canvas PRIVATE
    PROJECT_PATH="${CMAKE_SOURCE_DIR}")

target_link_libraries(canvas
    ${CMAKE_SOURCE_DIR}/dependencies/glad/glad.lib
    ${CMAKE_SOURCE_DIR}/dependencies/imgui/imgui.lib
    glfw3
)
