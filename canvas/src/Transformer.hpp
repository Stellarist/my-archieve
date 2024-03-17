/**
* transformer
* lab 3 content
*/

#pragma once

#include "Rasterizer.hpp"

#include <set>

#include "type.hpp"

class Transformer{
private:
    Rasterizer& rasterizer;
    std::set<int> selected_shapes;

public:
    double delta_a=0, last_a=0;
    double delta_s=0, last_s=0;

    Transformer(Rasterizer& rasterizer)
        : rasterizer(rasterizer) {}

    int getShapeNum(cursor_t cursor);

    void translate(shape_t& shape, const Eigen::Vector2f& route);
    void rotate(shape_t& shape, const Eigen::Vector2f& refer, float angle, bool f_center=true);
    void zoom(shape_t& shape, const Eigen::Vector2f& refer, float scale, bool f_center=true);
};
