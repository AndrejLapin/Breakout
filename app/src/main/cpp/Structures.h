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

typedef struct
{
    float x = 0, y = 0;
}Vector2;

#endif //BREAKOUTCPP_STRUCTURES_H
