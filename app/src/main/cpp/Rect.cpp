//
// Created by USER on 2020-10-08.
//

#include "Rect.h"

Rect::Rect()
{
    bottom = 0;
    left = 0;
    right = 0;
    top = 0;
}
Rect::Rect(float leftX, float bottomY, float rightX, float topY)
{
    bottom = bottomY;
    left = leftX;
    right = rightX;
    top = topY;
}
