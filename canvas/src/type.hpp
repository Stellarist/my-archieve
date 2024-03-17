#pragma once

#include <vector>

#include <eigen3/Eigen/Eigen>

struct Curve;
struct Square;
struct Shape;

using point_t=Eigen::Vector4f;

using vertex_t=Eigen::Vector2f;

using cursor_t=struct{
    double x;
    double y;
};

using color_t=struct{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

using edge_t=struct{
    int y_max;
    float x;
    float delta_x;
};

using line_t=struct{
    int x1, y1 ,x2, y2;
    color_t color;
};

using curve_t=Curve;

using square_t=Square;

using shape_t=Shape;

struct Curve{
    std::vector<vertex_t> vertices;

    Curve()=default;

    Curve(const std::vector<cursor_t> cursors)
    {
        for(auto it=cursors.begin(); it!=std::prev(cursors.end()); it++)
            vertices.push_back({it->x, it->y});
    }
};

struct Shape{
    std::vector<vertex_t> vertices;
    color_t color;

    Shape()=default;

    Shape(const std::vector<cursor_t>& cursors)
    {
        for(auto it=cursors.begin(); it!=std::prev(cursors.end()); it++)
            vertices.push_back({it->x, it->y});
    }
};

inline bool operator==(const color_t& a, const color_t& b)
{
    return a.r==b.r && a.g==b.g && a.b==b.b;
}

inline bool operator!=(const color_t& a, const color_t& b)
{
    return !(a==b);
}
