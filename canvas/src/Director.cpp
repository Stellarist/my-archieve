#include "Director.hpp"

#include "type.hpp"

constexpr int getDisSquare(int x1, int x2, int y1, int y2);

void Director::updateFrame()
{
    rasterizer.clearBuffers();

    switch(state){
        using enum State;
        case NONE:
            break;

        case LINE_MIDPOINT:
            if(cursors.size()<2) break;
            rasterizer.drawLineInMidpoint({cursors[0].x, cursors[0].y, 0.0f, 1.0f}, {cursors[1].x, cursors[1].y, 0.0f, 1.0f}, pixel_width, pixel_type, getColorRGB());
            if(cnt==2) {rasterizer.save(); cnt=0;}
            break;

        case LINE_BRESENHAM:
            if(cursors.size()<2) break;
            rasterizer.drawLineInBresenham({cursors[0].x, cursors[0].y, 0.0f, 1.0f}, {cursors[1].x, cursors[1].y, 0.0f, 1.0f}, pixel_width, pixel_type, getColorRGB());
            if(cnt==2) {rasterizer.save(); cnt=0;}
            break;

        case CIRCLE_MIDPOINT:
            if(cursors.size()<2) break;
            rasterizer.drawCircleInMidpoint({cursors[0].x, cursors[0].y, 0.0f, 1.0f}, {cursors[1].x, cursors[1].y, 0.0f, 1.0f}, pixel_width, getColorRGB());
            if(cnt==2) {rasterizer.save(); cnt=0;}
            break;

        case ARC_MIDPOINT:
            if(cursors.size()<2)
                break;
            else if(cursors.size()==2)
                rasterizer.drawLineInBresenham({cursors[0].x, cursors[0].y, 0.0f, 1.0f}, {cursors[1].x, cursors[1].y, 0.0f, 1.0f}, pixel_width, 0, getColorRGB());
            else if(cursors.size()==3)
                rasterizer.drawArcInMidpoint({cursors[0].x, cursors[0].y, 0.0f, 1.0f}, {cursors[1].x, cursors[1].y, 0.0f, 1.0f}, {cursors[2].x, cursors[2].y, 0.0f, 1.0f}, pixel_width, getColorRGB());
            if(cnt==3) {rasterizer.save(); cnt=0;}
            break;

        case Line_M:
            if(cursors.size()<2) break;
            manipulator.drawLine(cursors[0].x, cursors[0].y, cursors[1].x, cursors[1].y, getColorRGB(), true);
            if(cnt==2) {rasterizer.save(); cnt=0;}
            break;

        case SCAN:
            if(cursors.size()<2)
                break;
            if(getCursorDisSqr(cursors[0], *(cursors.end()-2))<625 && cursors.size()>2){
                cursors.erase(cursors.end()-2);
                shape_t shape{cursors};
                manipulator.scan(shape, getColorRGB());
                shape.color=getColorRGB();
                updateShape(manipulator.depth, shape);
                rasterizer.save();
                cnt=0;
            }else if(getCursorDisSqr(cursors[0], *(cursors.end()-1))<625)
                rasterizer.drawLineInBresenham(getCursorPoint(cursors.end()-2), getCursorPoint(cursors.begin()), 1, 0, {0, 0, 0});
            else
                rasterizer.drawLineInBresenham(getCursorPoint(cursors.end()-2), getCursorPoint(cursors.end()-1), 1, 0, {0, 0, 0});
            if(clicked){
                rasterizer.save();
                clicked=false;
            }
            break;

        case FILL:
            if(clicked){
                auto [x, y]=cursors[0];
                manipulator.fill(x, y, rasterizer.getPixel(x, y), getColorRGB());
                rasterizer.save();
                clicked=false;
                cnt=0;
            }
            break;

        case CUT_CS:
            if(cursors.size()<2)
                break;
            else if(cursors.size()==2)
                manipulator.drawRectangle(cursors[0].x, cursors[0].y, cursors[1].x, cursors[1].y, {0, 0, 0});
            else if(cursors.size()==3){
                manipulator.cutInCS(cursors[0].x, cursors[1].x, cursors[0].y, cursors[1].y);
                rasterizer.save();
                cnt=0;
                setState(State::NONE);
            }
            break;

        case CUT_MP:
            if(cursors.size()<2)
                break;
            else if(cursors.size()==2)
                manipulator.drawRectangle(cursors[0].x, cursors[0].y, cursors[1].x, cursors[1].y, {0, 0, 0});
            else if(cursors.size()==3){
                manipulator.cutInMP(cursors[0].x, cursors[1].x, cursors[0].y, cursors[1].y);
                rasterizer.save();
                cnt=0;
                setState(State::NONE);
            }
            break;

        case CUT_PG:
            if(cursors.size()<2)
                break;
            if(getCursorDisSqr(cursors[0], *(cursors.end()-2))<625 && cursors.size()>2){
                cursors.erase(cursors.end()-2);
                manipulator.cutInPG({cursors});
                rasterizer.save();
                cnt=0;
                setState(State::NONE);
            }else if(getCursorDisSqr(cursors[0], *(cursors.end()-1))<625)
                rasterizer.drawLineInBresenham(getCursorPoint(cursors.end()-2), getCursorPoint(cursors.begin()), 1, 1, {0, 0, 0});
            else
                rasterizer.drawLineInBresenham(getCursorPoint(cursors.end()-2), getCursorPoint(cursors.end()-1), 1, 1, {0, 0, 0});
            if(clicked){
                rasterizer.save();
                clicked=false;
            }
            break;
            
        case SELECT:
            if(clicked){
                auto cursor=cursors.back();
                int t=transformer.getShapeNum(cursor);
                if(t)
                    shape_depths.insert(transformer.getShapeNum(cursor));
                transformer.last_a=0;
                transformer.last_s=1;
                angle=0;
                scale=1;
                clicked=false;
            }
            break;

        case UNSELECT:
            shape_depths.clear();
            break;

        case Translate:
            for(auto i: shape_depths){
                if(i!=0){
                    if(dragging){
                        manipulator.scan(shapes[i], {255, 255, 255}, 0, true);
                        transformer.translate(shapes[i], {delta_x, delta_y});
                        manipulator.scan(shapes[i], shapes[i].color, i);
                        rasterizer.save();
                    }
                }
            }
            break;

        case Rotate_S:
            transformer.delta_a=angle-transformer.last_a;
            transformer.last_a=angle;
            for(auto i: shape_depths){
                if(i!=0){
                    if(transformer.delta_a){
                        manipulator.scan(shapes[i], {255, 255, 255}, 0, true);
                        transformer.rotate(shapes[i], {x_ref, y_ref}, transformer.delta_a, true);
                        manipulator.scan(shapes[i], shapes[i].color, i);
                        rasterizer.save();
                    }
                }
            }
            break;

        case Rotate_R:
            transformer.delta_a=angle-transformer.last_a;
            transformer.last_a=angle;
            for(auto i: shape_depths){
                if(i!=0){
                    if(transformer.delta_a){
                        manipulator.scan(shapes[i], {255, 255, 255}, 0, true);
                        transformer.rotate(shapes[i], {x_ref, y_ref}, transformer.delta_a, false);
                        manipulator.scan(shapes[i], shapes[i].color, i);
                        rasterizer.save();
                    }
                }
            }
            break;

        case Zoom_S:
            transformer.delta_s=scale/transformer.last_s;
            transformer.last_s=scale;
            for(auto i: shape_depths){
                if(i!=0){
                    if(transformer.delta_s){
                        manipulator.scan(shapes[i], {255, 255, 255}, 0, true);
                        transformer.zoom(shapes[i], {x_ref, y_ref}, transformer.delta_s, true);
                        manipulator.scan(shapes[i], shapes[i].color, i);
                        rasterizer.save();
                    }
                }
            }
            break;

        case Zoom_R:
            transformer.delta_s=scale/transformer.last_s;
            transformer.last_s=scale;
            for(auto i: shape_depths){
                if(i!=0){
                    if(transformer.delta_s){
                        manipulator.scan(shapes[i], {255, 255, 255}, 0, true);
                        transformer.zoom(shapes[i], {x_ref, y_ref}, transformer.delta_s, false);
                        manipulator.scan(shapes[i], shapes[i].color, i);
                        rasterizer.save();
                    }
                }
            }
            break;

        case Bezier:
            if(cursors.size()<2)
                break;
            else
                curver.drawBezier(cursors, getColorRGB());
            break;

        case Bezier_Select:
            if(dragging){
                auto cursor=cursors.back();
                if(first_mouse){
                    auto p=curver.findCurve(cursor);
                    curver_x=p.first;
                    curver_y=p.second;
                    first_mouse=false;
                }
                if(curver_x<0 || curver_y<0 || curver_x>=(int)curver.curves.size())
                    break;
                else{
                    curver.drawBezier(curver.curves[curver_x], getColorRGB(), true);
                    curver.curves[curver_x].vertices[curver_y].x()=cursor.x;
                    curver.curves[curver_x].vertices[curver_y].y()=cursor.y;
                    curver.drawBezier(curver.curves[curver_x], getColorRGB());
                    rasterizer.save();
                }
            }
            break;

        case Bezier_Finish:
            curver.drawBezier(curver.curve, getColorRGB());
            curver.saveCurve();
            rasterizer.save();
            break;
    }

    cursors.resize(cnt);
}


void Director::updateCursor(cursor_t cursor, bool flag)
{
    if(state==State::NONE)
        return;
    if(flag){
        cursors[cnt]=cursor;
        cnt++;
    }
    else
        cursors.push_back(cursor);
}

void Director::updateShape(int depth, const shape_t& shape)
{
    shapes.insert({depth, shape});
}
