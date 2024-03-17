/**
* rasterizer
* lab 1 content
* double buffering to record
*/

#pragma once

#include <vector>
#include <array>

#include "type.hpp"

class Rasterizer{
private:
    int width;
    int height;
    int cnt=0;

    std::vector<color_t> frame_buffer_l1;
    std::vector<color_t> frame_buffer_l2;
    std::vector<int> z_buffer;

    static std::array<std::array<bool, 16>, 4> pixel_patterns;

public:
    Rasterizer()=default;
    Rasterizer(int width, int height);
    ~Rasterizer()=default;

    constexpr int getWidth() const {return width;}
    constexpr int getHeight() const {return height;}
    constexpr unsigned int getSize() const {return frame_buffer_l1.size()*sizeof(color_t);}
    constexpr std::vector<color_t>& getFrameBuffer() {return frame_buffer_l1;}
    constexpr std::vector<int>& getZBuffer() {return z_buffer;}
    constexpr void* getData() {return frame_buffer_l1.data();}
    constexpr color_t getPixel(int x, int y) {return frame_buffer_l1.at(y*width+x);}
    constexpr int getDepth(int x, int y) {return z_buffer.at(y*width+x);}
    constexpr bool isInBuffer(int x, int y) {return !(x>=width-1 || y>=height-1 || x<=0 || y<=0);}

    void save();
    void resize(int width, int height);
    void clearBuffers();
    void clearContents();

    void drawPixel(const point_t& point, const color_t& color, int depth=0);
    void drawPoint(const point_t& point);
    void drawPixels(const point_t& point, const color_t& color, int pixel_width, int pixel_type, bool is_steep);
    void drawCirclePixels(int x, int y, int x_axis, int y_axis, const color_t& color, int pixel_width); 
    void drawLineInMidpoint(point_t begin, point_t end, 
        int pixel_width=1, int pixel_type=0, const color_t& color={0, 0, 0});
    void drawLineInBresenham(point_t begin, point_t end, 
        int pixel_width=1, int pixel_type=0, const color_t& color={0, 0, 0});
    void drawCircleInMidpoint(point_t center, point_t perimeter,
        int pixel_width=1, const color_t& color={0, 0, 0});
    void drawArcInMidpoint(point_t center, point_t arc_begin, point_t arc_end,
        int pixel_width=1, const color_t& color={0, 0, 0});
};
