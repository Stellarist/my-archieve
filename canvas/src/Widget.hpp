/**
* Widget
* imgui widgets
*/

#pragma once

#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Director.hpp"

class Widget{
private:
    Director& director;
    bool main_window=true;
    bool create_window=false;
    bool manipulate_window=false;
    bool transform_window=false;
    bool curve_window=false;

public:
    Widget(GLFWwindow* window, Director& director);
    ~Widget();

    void create();
    void render();
    
    static bool touched();
};