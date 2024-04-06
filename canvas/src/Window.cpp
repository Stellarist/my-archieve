#include "Window.hpp"

#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

cursor_t Window::cursor({0, 0});
Director Window::director;

Window::Window()
{
    glfwInit();
    window=glfwCreateWindow(1920, 1080, "Canvas", nullptr, nullptr);
    if(window==nullptr){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMaximizeWindow(window);
    glfwGetWindowSize(window, &this->width, &this->height);
    glfwMakeContextCurrent(window);
    director.resize(width, height);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
    glViewport(0, 0, width, height);

    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(this->window, frameBufferSizeCallback);
    glfwSetMouseButtonCallback(this->window, mouseButtonCallback);
    glfwSetCursorPosCallback(this->window, cursorPosCallback);

    widget=new Widget(window, director);
    createGlShader();
}

Window::~Window()
{
    delete widget;

    deleteGlShader();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::run()
{
    initialize();

    while(!glfwWindowShouldClose(window)){
        processInput();
        widget->create();
           
        glfwGetCursorPos(window, &cursor.x, &cursor.y);
        director.updateCursor(cursor);
        director.updateFrame();

        glUseProgram(window_shader);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, director.getData());
        glBindVertexArray(this->vao);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
        
        widget->render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    release();
}

void Window::initialize()
{
    float vertices[]={
        -1.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        1.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        1.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        -1.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenTextures(1, &window_texture);
    glBindTexture(GL_TEXTURE_2D, window_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glUseProgram(window_shader);
    glUniform1i(glGetUniformLocation(window_shader, "texture0"), 0);
}

void Window::release()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    glfwTerminate();
}

void Window::processInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void Window::frameBufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::mouseButtonCallback(GLFWwindow* window, int button, int state, int mode)
{
    if(button==GLFW_MOUSE_BUTTON_LEFT && !Widget::touched()){
        if(state==GLFW_PRESS){
            glfwGetCursorPos(window, &cursor.x, &cursor.y);
            director.updateCursor({cursor.x, cursor.y}, true);
            director.clicked=true;
            director.dragging=true;
        }else if(state==GLFW_RELEASE){
            director.dragging=false;
            director.first_mouse=true;
        }
    }else if(button==GLFW_MOUSE_BUTTON_RIGHT && state==GLFW_PRESS && !Widget::touched())
        director.clearCursors();
}

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    if(director.dragging) {
        if(director.first_mouse){
            director.delta_x=0;
            director.delta_y=0;
            director.first_mouse=false;
        }else{
            director.delta_x=xpos-director.last_x;
            director.delta_y=ypos-director.last_y;
        }

        director.last_x=xpos;
        director.last_y=ypos;
    }else{
        director.delta_x=0;
        director.delta_y=0;
    }
}

void Window::createGlShader()
{
    const char* vertex_code=R"(
        #version 450 core
        layout (location=0) in vec3 a_pos;
        layout (location=1) in vec2 a_tex_coords;
        out vec3 pos;
        out vec2 tex_coords;
        void main()
        {
            gl_Position=vec4(a_pos, 1.0);
            pos=a_pos;
            tex_coords=vec2(a_tex_coords.x, 1-a_tex_coords.y);
        }
    )"

    const char* fragment_code=R"(
        #version 450 core
        in vec3 pos;
        in vec2 tex_coords;
        out vec4 frag_color;
        uniform sampler2D texture0;
        void main()
        {
            frag_color=texture(texture0, tex_coords);
        }
    )";

    unsigned int vertex, fragment;
    vertex=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_code, nullptr);    
    glCompileShader(vertex);
    checkGlShader(vertex, "VERTEX");

    fragment=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_code, nullptr);
    glCompileShader(fragment);
    checkGlShader(fragment, "FRAGMENT");

    window_shader=glCreateProgram();
    glAttachShader(window_shader, vertex);
    glAttachShader(window_shader, fragment);
    glLinkProgram(window_shader);
    checkGlShader(window_shader, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Window::checkGlShader(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if(type != "PROGRAM"){
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog 
                << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }else{
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog 
                << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Window::deleteGlShader()
{
    glDeleteProgram(window_shader);
}
