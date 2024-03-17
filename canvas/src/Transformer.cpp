#include "Transformer.hpp"

void Transformer::translate(shape_t& shape, const Eigen::Vector2f& route)
{
    for(auto& vertex: shape.vertices)
        vertex=vertex+route;
}

void Transformer::rotate(shape_t& shape, const Eigen::Vector2f& refer, float angle, bool f_center)
{
    static auto barycenter=[] (int& x, int& y, const shape_t& shape) {
        for(auto vertex: shape.vertices){
            x+=vertex.x();
            y+=vertex.y();
        }
        if(shape.vertices.size()){
            x/=shape.vertices.size();
            y/=shape.vertices.size();
        }
    };

    int x=0, y=0;
    if(f_center)
        barycenter(x, y, shape);
    if(!f_center)
        translate(shape, {-refer.x(), -refer.y()});
    else
        translate(shape, {-x, -y});
    Eigen::Matrix2f rotation;
    rotation<<cos(angle), -sin(angle),
                    sin(angle), cos(angle);
    for(auto& vertex: shape.vertices)
        vertex=rotation*vertex;
    if(!f_center)
        translate(shape, {refer.x(), refer.y()});
    else
        translate(shape, {x, y});
}

void Transformer::zoom(shape_t& shape, const Eigen::Vector2f& refer, float scale, bool f_center)
{
    static auto barycenter=[] (int& x, int& y, const shape_t& shape) {
        for(auto vertex: shape.vertices){
            x+=vertex.x();
            y+=vertex.y();
        }
        if(shape.vertices.size()){
            x/=shape.vertices.size();
            y/=shape.vertices.size();
        }
    };

    Eigen::Vector2f ref;
    int x=0, y=0;
    if(f_center){
        barycenter(x, y, shape);
        ref={x, y};
    }else
        ref=refer;    
    
    for(auto& vertex: shape.vertices){
        vertex-=ref;
        vertex*=scale;
        vertex+=ref;
    }
}

int Transformer::getShapeNum(cursor_t cursor)
{
    return rasterizer.getDepth((int)cursor.x, (int)cursor.y);
}
