/**
* manipulator
* lab 2 content
*/

#pragma once

#include "Rasterizer.hpp"

#include "type.hpp"

class Manipulator{
private:
    Rasterizer& rasterizer;

public:
    int depth=0;
    line_t line={0, 0, 0, 0, {0, 0, 0}};

    Manipulator(Rasterizer& rasterizer)
        :rasterizer(rasterizer) {}

    enum Location {
        LEFT=1,
        RIGHT=2,
        BOTTOM=4,
        TOP=8
    };
    
    void scan(const shape_t& shape, const color_t& new_color, int shape_depth=0, bool is_anti=false);
    void fill(int x, int y, const color_t& old_color, const color_t& new_color);
    void cutInCS(int x_left, int x_right, int y_bottom, int y_top);
    void cutInMP(int x_left, int x_right, int y_bottom, int y_top);
    void cutInPG(const shape_t& shape);
    void drawLine(int x1, int y1, int x2, int y2, const color_t& color, bool is_dashed=false);
    void drawLine(line_t line, bool is_dashed=false);
    void drawRectangle(int x1, int y1, int x2, int y2, const color_t& color);
    void drawFrame(const shape_t& shape, int type=0);
};