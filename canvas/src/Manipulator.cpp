#include "Manipulator.hpp"

#include <map>
#include <stack>

#include "type.hpp"

void Manipulator::scan(const shape_t& shape, const color_t& color, int shape_depth, bool is_anti)
{
    bool flag=false;
    for(auto v: shape.vertices){
        if(rasterizer.isInBuffer(v.x(), v.y()))
            flag=true;
    }
    if(!flag)
        return;
    if(!shape_depth)
        is_anti ? depth=0 : depth++;
    else
        depth=shape_depth;
    color_t new_color=(color==color_t{0, 0, 0}) ? color_t{1, 1, 1} : color;
    std::list<edge_t> active_edges;
    std::multimap<int, edge_t> edge_table;
    int y=std::numeric_limits<int>::max();
    int y2=std::numeric_limits<int>::min();
    for(auto it= shape.vertices.begin(); it!=shape.vertices.end(); it++){
        auto it_n=(it==std::prev(shape.vertices.end())) ?  shape.vertices.begin() : std::next(it);
        int y_max=std::max(it->y(), it_n->y());
        int y_min=std::min(it->y(), it_n->y());
        float x=it->y()<it_n->y() ? it->x() : it_n->x();
        float delta_x=it->y()!=it_n->y() ? (it_n->x()-it->x())/(it_n->y()-it->y()) : 0;
        edge_t edge={y_max, x, delta_x};
        edge_table.insert({y_min, edge});
        y=std::min(y_min, y);
        y2=std::max(y_max, y2)+1;
    }

    std::list<edge_t>::iterator it;
    while (y<y2){
        auto range=edge_table.equal_range(y);
        for(auto it=range.first; it!=range.second; it++)
            active_edges.push_back(it->second);
        edge_table.erase(range.first, range.second);

        active_edges.sort([](const edge_t& a, const edge_t& b) -> bool {
            return abs(b.x-a.x)<1e-6 ? a.delta_x<b.delta_x : a.x<b.x;
        });

        it=active_edges.begin();
        auto it_p=it;
        int cnt=0;
        while(it!=active_edges.end() && cnt<10){
            if(it_p->x<=0){
                it=it_p=active_edges.erase(it_p);
                break;
            }
            cnt++;
            it_p=it;
            it++;
            if(it!=active_edges.end() && it_p!=active_edges.end()){
                for(int x=it_p->x; x<=it->x+1; x++)
                    rasterizer.drawPixel({x, y, 0.0f, 1.0f}, new_color, depth);
            }
            it++;
        }
        y++;
        active_edges.remove_if([&y](edge_t edge){return (int)edge.y_max==y;});
        for(auto& edge: active_edges)
            edge.x+=edge.delta_x;
    }
    
    if(is_anti)
        drawFrame(shape, 1);
    else
        drawFrame(shape);
}

void Manipulator::fill(int x, int y, const color_t& old_color, const color_t& color)
{
    color_t new_color=(color==color_t{0, 0, 0}) ? color_t{1, 1, 1} : color;
    std::stack<std::pair<int, int>> coords;
    coords.push({x,y});

    while(!coords.empty()){
        auto [coord_x, coord_y]=coords.top();
        coords.pop();

        color_t color=rasterizer.getPixel(coord_x, coord_y);
        if(color!=new_color && color!=color_t{0, 0, 0} && rasterizer.isInBuffer(coord_x, coord_y)){
            rasterizer.drawPixel({coord_x, coord_y, 0.0f, 1.0f}, new_color);
            coords.push({coord_x, coord_y+1});
            coords.push({coord_x, coord_y-1});
            coords.push({coord_x+1, coord_y});
            coords.push({coord_x-1, coord_y});
        }
    }
}

void Manipulator::cutInCS(int x_left, int x_right, int y_bottom, int y_top)
{
    drawLine(line.x1, line.y1, line.x2, line.y2, {255, 255, 255});
    static auto encode=[&] (int x, int y) -> int {
        int c=0;
        if(x<x_left)
            c|=Location::LEFT;
        else if(x>x_right)
            c|=Location::RIGHT;
        if(y<y_bottom)
            c|=Location::BOTTOM;
        else if(y>y_top)
            c|=Location::TOP;
        return c;
    };

    auto[x1, y1, x2, y2, color]=line;
    int code, x, y;
    int code1=encode(x1, y1);
    int code2=encode(x2, y2);

    while(code1!=0 || code2!=0){
        if((code1 & code2)!=0)
            return;
        code=code1;
        if(code1==0)
            code=code2;
        if((LEFT&code) !=0){
            x=x_left;
            y=y1+(y2-y1)*(x-x1)/(x2-x1);
        }else if((RIGHT&code) !=0){
            x=x_right;
            y=y1+(y2-y1)*(x-x1)/(x2-x1);
        }else if((BOTTOM&code) !=0){
            y=y_bottom;
            x=x1+(x2-x1)*(y-y1)/(y2-y1);
        }else if((TOP&code) !=0){
            y=y_top;
            x=x1+(x2-x1)*(y-y1)/(y2-y1);
        }
        if(code==code1){
            x1=x;
            y1=y;
            code1=encode(x1, y1);
        }else{
            x2=x;
            y2=y;
            code2=encode(x2, y2);
        }
    }

    line={std::max(x1, x_left), std::max(y1, y_bottom), std::min(x2, x_right), std::min(y2, y_top), color};
    drawLine(line, true);
}

void Manipulator::cutInMP(int x_left, int x_right, int y_bottom, int y_top)
{
    drawLine(line.x1, line.y1, line.x2, line.y2, {255, 255, 255});
    static auto getDis=[] (int x1, int x2, int y1, int y2) -> float {
        return sqrt(pow(x1-x2, 2)+pow(y1-y2, 2));
    };

    static auto encode=[&] (int x, int y) -> int {
        int c=0;
        if(x<x_left)
            c|=Location::LEFT;
        else if(x>x_right)
            c|=Location::RIGHT;
        if(y<y_bottom)
            c|=Location::BOTTOM;
        else if(y>y_top)
            c|=Location::TOP;
        return c;
    };

    if(line.x1>line.x2){
        std::swap(line.x1, line.x2);
        std::swap(line.y1, line.y2);
    }
    auto[x1, y1, x2, y2, color]=line;
    auto[x3, y3, x4, y4, color2]=line;
    int xm1=(x1+x2)/2, ym1=(y1+y2)/2;
    int xm2=(x3+x4)/2, ym2=(y3+y4)/2;

    float k=(float)(y2-y1)/(x2-x1);
    float b=(float)y1-k*x1;
    int cnt=0;

    while(getDis(xm1, x2, ym1, y2)>1e-3){
        if(cnt++>100)
            break;
        xm1=(x1+x2)/2;
        ym1=(y1+y2)/2;
        int code_0=encode(xm1, ym1);
        int code_1=encode(x1, y1);

        if(code_0==0 && code_1==0)
            break;
        if((code_0&code_1)==0){
            x2=xm1;
            y2=ym1;
        }else if(encode(x2, y2)!=0){
            x1=xm1;
            y1=ym1;
        }else 
            break;
    }
    xm1=std::max({(float)x1, (float)x_left, std::min((float)(y_top-b)/k, (float)(y_bottom-b)/k)});
    ym1=k*xm1+b;

    while(getDis(xm2, x3, ym2, y3)>1e-3){
        if(cnt++>100)
            break;
        xm2=(x3+x4)/2;
        ym2=(y3+y4)/2;
        int code_0=encode(xm2, ym2);
        int code_2=encode(x4, y4);

        if(code_0==0 && code_2==0)
            break;
        if((code_0&code_2)==0){
            x3=xm2;
            y3=ym2;
            x4=xm2;
            y4=ym2;
        }else if(encode(x3, y3)!=0){
            x4=xm2;
            y4=ym2;
            x3=xm2;
            y3=ym2;
        }else 
            break;
    }
    xm1=std::max({(float)x1, (float)x_left, std::min((float)(y_top-b)/k, ((float)y_bottom-b)/k)});
    xm2=std::min({(float)x2, (float)x_right, std::max((float)(y_top-b)/k, ((float)y_bottom-b)/k)});
    ym2=k*xm2+b;

    if(xm1<xm2)
        drawLine(xm1, ym1, xm2, ym2, color, true);
}

void Manipulator::cutInPG(const shape_t& shape)
{
    drawLine(line.x1, line.y1, line.x2, line.y2, {255, 255, 255});
    std::vector<vertex_t> vertices;
    auto[x1, y1, x2, y2, color]=line;
    if(x1>x2){
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    float k=(float)(y2-y1)/(x2-x1);
    float b=(float)y1-k*x1;
    bool visible=false;
    for(auto it= shape.vertices.begin(); it!=shape.vertices.end(); it++){
        auto it_n=(it==std::prev(shape.vertices.end())) ?  shape.vertices.begin() : std::next(it);
        float k2=(it_n->y()-it->y())/(it_n->x()-it->x());
        float b2=it->y()-k2*it->x(); 
        float x=(b2-b)/(k-k2);
        float y=k*x+b;
        if(x>=std::min(it->x(), it_n->x()) && x<=std::max(it->x(), it_n->x())){
            vertices.push_back({x, y});
            if(x<x1)
                visible=!visible;
        }
    };

    if(visible)
        vertices.push_back({x1, y1});
    vertices.push_back({x2, y2});
    std::sort(vertices.begin(), vertices.end(), [](const vertex_t& a, const vertex_t& b) -> bool {
        return a.x()<b.x();
    });
    
    for(auto it=vertices.begin(); it!=vertices.end()&&(it+1)!=vertices.end(); it++){
        drawLine(it->x(), it->y(), (it+1)->x(), (it+1)->y(), color, true);
        it++;
    }

    line={0, 0, 0, 0, {0, 0, 0}};
    drawFrame(shape, 1);
}

void Manipulator::drawLine(int x1, int y1, int x2, int y2, const color_t& color, bool is_dashed) {
    if(is_dashed)
        line={x1, y1, x2, y2, color};
    int dx=abs(x2 - x1);
    int dy=abs(y2 - y1);
    int sx=(x1 < x2) ? 1 : -1;
    int sy=(y1 < y2) ? 1 : -1;
    int err=dx - dy;

    while (x1 != x2 || y1 != y2) {
        rasterizer.drawPixel({x1, y1, 0.0f, 1.0f}, color);
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }

    rasterizer.drawPixel({x2, y2, 0.0f, 1.0f}, color);
}

void Manipulator::drawLine(line_t line, bool is_dashed)
{
    drawLine(line.x1, line.y1, line.x2, line.y2, line.color, is_dashed);
}

void Manipulator::drawRectangle(int x1, int y1, int x2, int y2, const color_t& color)
{
    rasterizer.drawLineInBresenham({x1, y1, 0.0f, 1.0f}, {x2, y1, 0.0f, 1.0f}, 1, 1, {0, 0, 0});
    rasterizer.drawLineInBresenham({x1, y1, 0.0f, 1.0f}, {x1, y2, 0.0f, 1.0f}, 1, 1, {0, 0, 0});
    rasterizer.drawLineInBresenham({x2, y2, 0.0f, 1.0f}, {x2, y1, 0.0f, 1.0f}, 1, 1, {0, 0, 0});
    rasterizer.drawLineInBresenham({x2, y2, 0.0f, 1.0f}, {x1, y2, 0.0f, 1.0f}, 1, 1, {0, 0, 0});
}

void Manipulator::drawFrame(const shape_t& shape, int type)
{
    for(auto it= shape.vertices.begin(); it!=shape.vertices.end(); it++){
        auto it_n=(it==std::prev(shape.vertices.end())) ?  shape.vertices.begin() : std::next(it);
        if(type==0){
            rasterizer.drawLineInBresenham({it->x(), it->y(), 0.0f, 1.0f}, {it_n->x(), it_n->y(), 0.0f, 1.0f}, 4, 0, {0, 0, 0});
            rasterizer.drawPixel({it->x(), it->y(), 0.0f, 1.0f}, {0, 0, 0});
        }
        else if(type==1){
            rasterizer.drawLineInBresenham({it->x(), it->y(), 0.0f, 1.0f}, {it_n->x(), it_n->y(), 0.0f, 1.0f}, 4, 0, {255, 255, 255});
            rasterizer.drawPixel({it->x(), it->y(), 0.0f, 1.0f}, {255, 255, 255});
        }
    }
}
