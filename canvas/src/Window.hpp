/**
* Window
* glfw windows
*/

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "type.hpp"
#include "Widget.hpp"
#include "Director.hpp"

class Window{
private:
    int width, height;
    unsigned int vao, vbo, window_texture, window_shader;

    GLFWwindow* window;
    Widget* widget;

    static Director director;
    static cursor_t cursor;

    void draw();
    void processInput();
    void initialize();
    void release();

    void createGlShader(const char* vertex_path, const char* fragment_path);
    void checkGlShader(unsigned int shader, std::string type);
    void deleteGlShader();

    static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
    static void mouseButtonCallback(GLFWwindow* window, int button, int state, int mod);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

public:
    Window();
    ~Window();

    int getWidth() {return width;}
    int getHeight() {return height;}

    void run();
};
