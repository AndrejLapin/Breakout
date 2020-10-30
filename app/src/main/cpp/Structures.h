//
// Created by USER on 2020-10-08.
//

#ifndef BREAKOUTCPP_STRUCTURES_H
#define BREAKOUTCPP_STRUCTURES_H

enum movement
{
    Stop = 0,
    Left = 1,
    Right = 2
};

enum menuOptions
{
    Restart = 0,
    Exit = 1
};

typedef struct Vector2
{
    float x = 0, y = 0;

    Vector2(float xCord, float yCord)
    {
        x = xCord;
        y = yCord;
    }
};

#endif //BREAKOUTCPP_STRUCTURES_H
