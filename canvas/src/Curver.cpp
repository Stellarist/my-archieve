#include "Curver.hpp"

void Curver::drawBezier(const std::vector<cursor_t>& cursors, const color_t& color, bool is_anti)
{
    static auto deCasteliau=[] (double t,double p[],int n) -> double {
        double pp[50][50];
        for(int k=0;k<=n;k++){
            pp[k][0]=p[k];
        }
        for(int r=1;r<=n;r++){
            for(int i=0;i<=n-r;i++){
                pp[i][r]=(1-t)*pp[i][r-1]+t*pp[i+1][r-1];
            }
        }
        return pp[0][n];
    };

    curve=Curve(std::vector<cursor_t>(cursors.begin(), cursors.end()));
    drawSquare(curve, false);

    int n=cursors.size();
    double x, y;
    int rate=500;
    double px[30],py[30];
    for(int k=0;k<=n-1;k++){
        px[k]=cursors[k].x;
        py[k]=cursors[k].y;
    }

    for(double t=0;t<=1;t+=0.01/rate){
        x=deCasteliau(t, px, n-1);
        y=deCasteliau(t, py, n-1);
        if(!is_anti)
            rasterizer.drawPixel({x, y, 0.0f, 1.0f}, color);
        else
            rasterizer.drawPixel({x, y, 0.0f, 1.0f}, {255, 255, 255});
    }
}

void Curver::drawBezier(const curve_t& curve, const color_t& color, bool is_anti)
{
    std::vector<cursor_t> cursors;
    for(auto vertex: curve.vertices)
        cursors.push_back({vertex.x(), vertex.y()});

    static auto deCasteliau=[] (double t,double p[],int n) -> double {
        double pp[50][50];
        for(int k=0;k<=n;k++){
            pp[k][0]=p[k];
        }
        for(int r=1;r<=n;r++){
            for(int i=0;i<=n-r;i++){
                pp[i][r]=(1-t)*pp[i][r-1]+t*pp[i+1][r-1];
            }
        }
        return pp[0][n];
    };

    drawSquare(curve, is_anti);

    int n=cursors.size();
    double x, y;
    int rate=500;
    double px[30],py[30];
    for(int k=0;k<=n-1;k++){
        px[k]=cursors[k].x;
        py[k]=cursors[k].y;
    }

    for(double t=0;t<=1;t+=0.01/rate){
        x=deCasteliau(t, px, n-1);
        y=deCasteliau(t, py, n-1);
        if(!is_anti)
            rasterizer.drawPixel({x, y, 0.0f, 1.0f}, color);
        else
            rasterizer.drawPixel({x, y, 0.0f, 1.0f}, {255, 255, 255});
    }
}

void Curver::drawSquare(const curve_t& curve, bool is_anti)
{
    color_t color;
    color=is_anti ? color_t{255, 255, 255} : color_t{0, 0, 0};

    for(auto vertex: curve.vertices){
        for(int i=vertex.x()-4; i<=vertex.x()+4; i++)
            for(int j=vertex.y()-4; j<=vertex.y()+4; j++)
                rasterizer.drawPixel({i, j, 0.0f, 1.0f}, color);
    }
}

void Curver::saveCurve(int loc)
{
    if(loc==-1)
        curves.push_back(curve);
    else if(loc>=0 && loc<(int)curves.size())
        curves[loc]=curve;
}

std::pair<int, int> Curver::findCurve(const cursor_t& cursor)
{
    int x=cursor.x, y=cursor.y;
    for(int i=0; i<(int)curves.size(); i++)
        for(int j=0; j<(int)curves[i].vertices.size(); j++)
            if((curves[i].vertices[j].x()-x)*(curves[i].vertices[j].x()-x)+(curves[i].vertices[j].y()-y)*(curves[i].vertices[j].y()-y)<=16)
                return {i, j};
    return {-1, -1};
}
