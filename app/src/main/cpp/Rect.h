//
// Created by USER on 2020-10-08.
//

#ifndef BREAKOUTCPP_RECT_H
#define BREAKOUTCPP_RECT_H


class Rect
{
private:

public:
    Rect();
    Rect(float leftX, float bottomY, float rightX, float topY);
    float GetWidth();
    float GetHeight();
    float bottom;
    float left;
    float right;
    float top;
    //~Rect();

protected:

};


#endif //BREAKOUTCPP_RECT_H
