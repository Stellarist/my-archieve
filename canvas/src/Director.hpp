/**
* director
* a state machine to control the drawing picture per frame
*/

#pragma once

#include <array>
#include <vector>

#include "Rasterizer.hpp"
#include "Manipulator.hpp"
#include "Transformer.hpp"
#include "Curver.hpp"

enum class State{
    NONE,
    LINE_MIDPOINT,
    LINE_BRESENHAM,
    CIRCLE_MIDPOINT,
    ARC_MIDPOINT,
    Line_M,
    SCAN,
    FILL,
    CUT_CS,
    CUT_MP,
    CUT_PG,
    SELECT,
    UNSELECT,
    Translate,
    Rotate_S,
    Rotate_R,
    Zoom_S,
    Zoom_R,
    Bezier,
    Bezier_Select,
    Bezier_Finish
};

class Director{
private:
    State state;
    Rasterizer rasterizer;
    Manipulator manipulator;
    Transformer transformer;
    Curver curver;

    std::vector<cursor_t> cursors{};
    std::vector<point_t> points{};
    std::set<int> shape_depths{};
    std::map<int, shape_t> shapes{};
    int cnt;

public:
    double delta_x=0, delta_y=0;
    double last_x=0, last_y=0;
    bool first_mouse=true;
    bool clicked=false;
    bool dragging=false;
    std::array<float, 3> color={0.0f ,0.0f, 0.0f};
    int x_ref=0;
    int y_ref=0;
    int pixel_width=1;
    int pixel_type=0;
    float angle=0.0f;
    float scale=1.0f;
    int curver_x, curver_y;

    Director()
        : manipulator(rasterizer), transformer(rasterizer), curver(rasterizer) {}
    ~Director()=default;

    void setState(State state) {this->state=state; cursors.clear(); cnt=0;}

    State getState()
        {return state;}
    void* getData() 
        {return rasterizer.getData();}
    Rasterizer& getRasterizer() 
        {return rasterizer;}
    color_t getColorRGB() 
        {return {(unsigned char)(color[0]*255), (unsigned char)(color[1]*255), (unsigned char)(color[2]*255)};}
    point_t getCursorPoint(std::vector<cursor_t>::iterator it)
        {return {it->x, it->y, 0.0, 1.0f};}
    int getCursorDisSqr(cursor_t a, cursor_t b)
        {return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);}
    constexpr int getWidth() 
        {return rasterizer.getWidth();}
    constexpr int getHeight()
        {return rasterizer.getHeight();}

    void resize(int width, int height) 
        {rasterizer.resize(width, height);}

    void updateFrame();
    void updateCursor(cursor_t cursor, bool flag=false);
    void updateShape(int depth, const shape_t& shape);
    void clearCursors() {cnt=0;}
    void clearBuffers() {rasterizer.clearBuffers();}
    void clearContents() 
        {rasterizer.clearContents(); cursors.clear(); cnt=0; shapes.clear(); shape_depths.clear(); curver.curves.clear(); clicked=false;}
};
