//
// Created by USER on 2020-10-12.
//

#ifndef BREAKOUTCPP_BRICK_H
#define BREAKOUTCPP_BRICK_H


#include "Rect.h"

class Brick
{
private:

    Rect myShape;
    bool isAlive;
    int lives;

public:

Brick();
Brick(int row, int column, int width, int height, int startingLives);
Rect GetRect();
bool GetIsAlive();
bool SubtractLife(); // returns true if the brick was just destroyed

protected:

};


#endif //BREAKOUTCPP_BRICK_H
