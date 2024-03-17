#include "Widget.hpp"

Widget::Widget(GLFWwindow* window, Director& director)
: director(director)
{
    // initialize imgui
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450");
}

Widget::~Widget()
{
    // destroy imgui window
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Widget::create()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Tools");

    if(ImGui::Checkbox("Create", &create_window)){
        manipulate_window = false;
        transform_window = false;
        curve_window = false;
    }
    ImGui::SameLine();

    if(ImGui::Checkbox("Manipulate", &manipulate_window)){
        create_window = false;
        transform_window = false;
        curve_window = false;
    }
    ImGui::SameLine();

    if(ImGui::Checkbox("Transform", &transform_window)){
        create_window = false;
        manipulate_window = false;
        curve_window = false;
    }
    ImGui::SameLine();

    if(ImGui::Checkbox("Curve", &curve_window)){
        create_window = false;
        manipulate_window = false;
        transform_window = false;
    }

    if(create_window){
        bool pattern_flag=director.getState()==State::NONE || director.getState()==State::LINE_MIDPOINT || director.getState()==State::LINE_BRESENHAM;
        bool width_flag=!director.pixel_type || !pattern_flag;

        ImGui::ColorEdit3("color", director.color.data());

        if(pattern_flag)
            ImGui::SliderInt("pattern", &director.pixel_type, 0, 3);
        else
            ImGui::SliderInt("pattern", &director.pixel_type, 0, 0);

        if(width_flag)
            ImGui::SliderInt("width", &director.pixel_width, 1, 8);
        else
            ImGui::SliderInt("width", &director.pixel_width, 1, 1);

        if(ImGui::Button("   Cursor  "))
            director.setState(State::NONE);
        if(ImGui::Button("   Clear   "))
            director.clearContents();
        if(ImGui::Button("  Midpoint "))
            director.setState(State::LINE_MIDPOINT);
        if(ImGui::Button(" Bresenham "))
            director.setState(State::LINE_BRESENHAM);
        if(ImGui::Button("   Circle  "))
            director.setState(State::CIRCLE_MIDPOINT);
        if(ImGui::Button("    Arc    "))
            director.setState(State::ARC_MIDPOINT);

        ImGui::Text("%.2ffps", ImGui::GetIO().Framerate);
    }

    if(manipulate_window){
        ImGui::ColorEdit3("color", director.color.data());

        if(ImGui::Button(" Cursor "))
            director.setState(State::NONE);
        if(ImGui::Button("  Clear "))
            director.clearContents();
        if(ImGui::Button(" Line_M "))
            director.setState(State::Line_M);
        if(ImGui::Button("  SCAN  "))
            director.setState(State::SCAN);
        if(ImGui::Button("  FILL  "))
            director.setState(State::FILL);
        if(ImGui::Button(" CUT_CS "))
            director.setState(State::CUT_CS);
        if(ImGui::Button(" CUT_MP "))
            director.setState(State::CUT_MP);
        if(ImGui::Button(" CUT_PG "))
            director.setState(State::CUT_PG);

        ImGui::Text("%.2ffps", ImGui::GetIO().Framerate);
    }

    if(transform_window){
        if(ImGui::Button("  Cursor   "))
            director.setState(State::NONE);
        if(ImGui::Button("   Clear   "))
            director.clearContents();
        if(ImGui::Button("  Select   "))
            director.setState(State::SELECT);
        if(ImGui::Button(" Unselect  "))
            director.setState(State::UNSELECT);
        if(ImGui::Button(" Translate "))
            director.setState(State::Translate);
        if(ImGui::Button(" Rotate_S  "))
            director.setState(State::Rotate_S);
        if(ImGui::Button(" Rotate_R  "))
            director.setState(State::Rotate_R);
        if(ImGui::Button("  Zoom_S   "))
            director.setState(State::Zoom_S);
        if(ImGui::Button("  Zoom_R   "))
            director.setState(State::Zoom_R);
        ImGui::SliderInt("x_ref", &director.x_ref, 0, director.getWidth());
        ImGui::SliderInt("y_ref", &director.y_ref, 0, director.getHeight());
        ImGui::SliderFloat("angle", &director.angle, -2*M_PI, 2*M_PI);
        ImGui::SliderFloat("scale", &director.scale, 0.1f, 10.0f);

        ImGui::Text("%.2ffps", ImGui::GetIO().Framerate);
    }

    if(curve_window){
        ImGui::ColorEdit3("color", director.color.data());
        if(ImGui::Button("  Cursor  "))
            director.setState(State::NONE);
        if(ImGui::Button("  Clear   "))
            director.clearContents();
        if(ImGui::Button("  Bezier  "))
            director.setState(State::Bezier);
        if(ImGui::Button("  Select  "))
            director.setState(State::Bezier_Select);
        if(ImGui::Button("  Finish  "))
            director.setState(State::Bezier_Finish);

        ImGui::Text("%.2ffps", ImGui::GetIO().Framerate);
    }

    ImGui::End();
}

void Widget::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Widget::touched()
{
    if(!ImGui::GetIO().WantCaptureMouse)
        return false;
    else
        return true;
}
