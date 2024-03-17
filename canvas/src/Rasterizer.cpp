#include "Rasterizer.hpp"

#include <cmath>

std::array<std::array<bool, 16>, 4> Rasterizer::pixel_patterns={{
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0 ,0},
}};

Rasterizer::Rasterizer(int width, int height)
: width(width), height(height),
  frame_buffer_l1(width*height, {255, 255, 255}),
  frame_buffer_l2(width*height, {255, 255, 255}),
  z_buffer(width*height, 0)
{ }

void Rasterizer::clearBuffers()
{
    frame_buffer_l1=frame_buffer_l2;
}

void Rasterizer::clearContents()
{
    std::fill(frame_buffer_l1.begin(), frame_buffer_l1.end(), color_t{255, 255, 255});
    std::fill(frame_buffer_l2.begin(), frame_buffer_l2.end(), color_t{255, 255, 255});
    std::fill(z_buffer.begin(), z_buffer.end(), 0);
}

void Rasterizer::save()
{
    frame_buffer_l2=frame_buffer_l1;
}

void Rasterizer::resize(int width, int height)
{
    this->width=width;
    this->height=height;
    frame_buffer_l1.resize(width*height, {255, 255, 255});
    frame_buffer_l2.resize(width*height, {255, 255, 255});
    z_buffer.resize(width*height, 0);
}

void Rasterizer::drawPixel(const point_t& point, const color_t& color, int depth)
{
    // set pixel color
    int x=point.x(), y=point.y();
    if(x>=width || y>=height || x<0 || y<0)
        return;

    int index=y*width+x;
    frame_buffer_l1[index]=color;
    z_buffer[index]=depth;
}

void Rasterizer::drawPoint(const point_t& point)
{
    for(int x=point.x()-1; x<=point.x()+1; x++)
        for(int y=point.y()-1; y<=point.y()+1; y++)
            drawPixel({x, y, 0.0f, 1.0f}, {0, 0, 0});
}

void Rasterizer::drawPixels(const point_t& point, const color_t& color, int pixel_width, int pixel_type, bool is_steep)
{
    if(!pixel_patterns[pixel_type][cnt++%16])
        return;

    if(point.x()>=width || point.y()>=height || point.x()<0 || point.y()<0)
        return;

    // caculate the pixel ranges
    int index, x=point.x(), y=point.y();
    int min_pixel=is_steep ? x-(int)(pixel_width/2) : y-(int)(pixel_width/2);
    int max_pixel=min_pixel+pixel_width;

    if(min_pixel<0)
        min_pixel=0;
    if(!is_steep){
        if(max_pixel>=height)
            max_pixel=height-1;
    }else
        if(max_pixel>=width)
            max_pixel=width-1;


    // set pixel color
    if(!is_steep){
        index=min_pixel*width+x;
        for(int i=min_pixel; i<max_pixel; i++){
            index+=width;
            frame_buffer_l1[index]=color;
        }
    }else{
        index=y*width+min_pixel;
        for(int i=min_pixel; i<max_pixel; i++){
            index++;
            frame_buffer_l1[index]=color;
        }
    }
}

void Rasterizer::drawCirclePixels(int x, int y, int x_axis, int y_axis, const color_t& color, int pixel_width)
{
    // draw pixel in 8 directions
    drawPixels({x_axis+x, y_axis+y, 0.0f, 1.0f}, color, pixel_width, 0, false);
    drawPixels({x_axis-x, y_axis+y, 0.0f, 1.0f}, color, pixel_width, 0, false);
    drawPixels({x_axis+x, y_axis-y, 0.0f, 1.0f}, color, pixel_width, 0, false);
    drawPixels({x_axis-x, y_axis-y, 0.0f, 1.0f}, color, pixel_width, 0, false);
    drawPixels({x_axis+y, y_axis+x, 0.0f, 1.0f}, color, pixel_width, 0, true);
    drawPixels({x_axis+y, y_axis-x, 0.0f, 1.0f}, color, pixel_width, 0, true);
    drawPixels({x_axis-y, y_axis+x, 0.0f, 1.0f}, color, pixel_width, 0, true);
    drawPixels({x_axis-y, y_axis-x, 0.0f, 1.0f}, color, pixel_width, 0, true);
}

void Rasterizer::drawLineInMidpoint(point_t begin, point_t end, int pixel_width, int pixel_type, const color_t& color)
{
    cnt=0;
    if(pixel_type)
        pixel_width=2;

    bool is_steep=false;
    if(abs(end.x()-begin.x())<abs(end.y()-begin.y())){
        std::swap(begin.x(), begin.y());
        std::swap(end.x(), end.y());
        is_steep=true; 
    }
    if(end.x()<begin.x())
        std::swap(begin, end);

    int x=begin.x(), y=begin.y(), xe=end.x();
    int a=-abs(begin.y()-end.y()), b=end.x()-begin.x();
    int d=2*a+b, d1=2*a, d2=2*(a+b), d0=end.y()-begin.y()>0 ? 1 : -1;

    is_steep ? drawPixels({y, x-1, 0.0f,1.0f}, color, pixel_width/2, pixel_type, is_steep) : drawPixels({x-1, y, 0.0f, 1.0f}, color, pixel_width/2, pixel_type, is_steep);
    is_steep ? drawPixels({y, x, 0.0f,1.0f}, color, pixel_width, pixel_type, is_steep) : drawPixels({x, y, 0.0f, 1.0f}, color, pixel_width, pixel_type, is_steep);
    while(x<xe){
        if(d<0){
            x++;
            y+=d0;
            d+=d2;
        }else{
            x++;
            d+=d1;
        } 
        is_steep ? drawPixels({y, x, 0.0f,1.0f}, color, pixel_width, pixel_type, is_steep) : drawPixels({x, y, 0.0f, 1.0f}, color, pixel_width, pixel_type, is_steep);
    }
    is_steep ? drawPixels({y, x+1, 0.0f,1.0f}, color, pixel_width/2, pixel_type, is_steep) : drawPixels({x+1, y, 0.0f, 1.0f}, color, pixel_width/2, pixel_type, is_steep);
}

void Rasterizer::drawLineInBresenham(point_t begin, point_t end, int pixel_width, int pixel_type, const color_t& color)
{
    cnt=0;
    if(pixel_type)
        pixel_width=2;

    bool is_steep=false;
    if(abs(end.x()-begin.x())<abs(end.y()-begin.y())){
        std::swap(begin.x(), begin.y());
        std::swap(end.x(), end.y());
        is_steep=true; 
    }
    if(end.x()<begin.x())
        std::swap(begin, end);

    int x=begin.x(), y=begin.y();
    int dx=end.x()-begin.x(), dy=abs(end.y()-begin.y());
    int e=-dx, d=end.y()>begin.y() ? 1 : -1;
    
    is_steep ? drawPixels({y, x-1, 0.0f,1.0f}, color, pixel_width/2, pixel_type, is_steep) : drawPixels({x-1, y, 0.0f, 1.0f}, color, pixel_width/2, pixel_type, is_steep);
    for(int i=1; i<dx; i++){
        is_steep ? drawPixels({y, x, 0.0f,1.0f}, color, pixel_width, pixel_type, is_steep) : drawPixels({x, y, 0.0f, 1.0f}, color, pixel_width, pixel_type, is_steep);
        x++;
        e+=2*dy;
        if(e>0){
            y+=d;
            e-=2*dx;
        }
    }
    is_steep ? drawPixels({y, x, 0.0f,1.0f}, color, pixel_width/2, pixel_type, is_steep) : drawPixels({x, y, 0.0f, 1.0f}, color, pixel_width/2, pixel_type, is_steep);
}

void Rasterizer::drawCircleInMidpoint(point_t center, point_t perimeter, int pixel_width, const color_t& color)
{
    int r=abs(sqrt((perimeter.x()-center.x())*(perimeter.x()-center.x())
        +(perimeter.y()-center.y())*(perimeter.y()-center.y())));
    int x=0, y=r, e=1-r, x_axis=center.x(), y_axis=center.y();
    drawCirclePixels(x, y, x_axis, y_axis, color, pixel_width);
    while(x<=y){
        if(e<0)
            e+=2*x+3;
        else{
            e+=2*(x-y)+5;
            y--;
        }
        x++;
        drawCirclePixels(x, y, x_axis, y_axis, color, pixel_width);
    }
}

void Rasterizer::drawArcInMidpoint(point_t center, point_t arc_begin, point_t arc_end, int pixel_width, const color_t& color)
{
    int r=abs(sqrt((arc_begin.x()-center.x())*(arc_begin.x()-center.x())
        +(arc_begin.y()-center.y())*(arc_begin.y()-center.y())));
    int x_axis=center.x(), y_axis=center.y();
    float x=arc_begin.x()-center.x(), y=arc_begin.y()-center.y();
    float xe=arc_end.x()-center.x(), ye=arc_end.y()-center.y();
    float begin = atan2(y, x);
    float end = atan2(ye, xe);
    if(y<0)
        begin=2*M_PI+begin;
    if(ye<0)
        end=2*M_PI+end;
    if(begin>end)
        end+=2*M_PI;

    float delta=0.001f, cos_d=cos(delta), sin_d=sin(delta), temp_x, temp_y;
    for(float theta=begin; theta<=end; theta+=delta){
        temp_x=x;
        temp_y=y;
        x=cos_d*temp_x-sin_d*temp_y;
        y=sin_d*temp_x+cos_d*temp_y;
        drawPixels({x_axis+x, y_axis+y, 0.0f, 1.0f}, color, pixel_width, 0, abs(y)<abs((x)));
    }
}
