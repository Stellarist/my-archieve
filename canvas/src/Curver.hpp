/**
* curver
* lab 4 content
*/

#pragma once

#include "Rasterizer.hpp"

class Curver{
private:
    Rasterizer& rasterizer;

public:
    curve_t curve;
    std::vector<curve_t> curves;

    Curver(Rasterizer& rasterizer)
            :rasterizer(rasterizer) {}
    
    void drawBezier(const std::vector<cursor_t>& cursors, const color_t& color, bool is_anti=false);
    void drawBezier(const curve_t& curve, const color_t& color, bool is_anti=false);
    void drawSquare(const curve_t& curve, bool is_anti=false);
    void saveCurve(int loc=-1);
    std::pair<int, int> findCurve(const cursor_t& cursor);
};
