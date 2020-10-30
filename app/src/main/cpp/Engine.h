//
// Created by USER on 2020-10-20.
//

#ifndef BREAKOUTCPP_ENGINE_H
#define BREAKOUTCPP_ENGINE_H

#include <string>
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "Button.h"

class Engine
{
private:
    Paddle *playerPaddle;
    Ball *ball;
    bool offsetSet;
    bool paddleIsTouched;
    float xOffset;
    float xOffsetStart;
    int screenWidth;
    int screenHeight;
    int ballCount;
    int numBricks;
    int numBricksToDestroy; // amount of bricks left to destroy to advance to the next level
    int lives;
    int score;
    bool brickBreaking;
    bool shouldAppRun;
    Brick *bricks[200];
    Button *btnRestart;
    Button *btnQuit;
public:
    Engine();
    Engine(int screenX, int screenY);
    void SetupLevel(int screenX, int screenY);
    void Update(long fps);
    void ProcessMinimalStep(long fps, float velocity);
    void CheckCollisionsAddDistance(long fps, float velocity);
    void GameOver();
    bool CircleRectIntersect(Circle circle, Rect rect);
    bool CirclesIntersect(Circle circle1, Circle circle2);
    void AddPositionToBall(long fps, float velocity);
    void SetOffsetSet(bool value);
    void TouchListener(float touchPointX, float touchPointY);
    void ButtonTouchListener(float touchPointX, float touchPointY);
    void RestartGame();
    void SetPaddleIsTouched(bool value);
    void Destroy();
    int GetLives();
    int GetScore();
    bool BrickBreaking();
    bool ShouldAppBeRunning();
    Vector2 GetScreenParameters();

    // Player specific methods
    void MovePaddle(float touchPointX, float touchPointY);
    float GetPlayerTop();
    float GetPlayerLeft();
    float GetPlayerRight();
    float GetPlayerBottom();
    float GetPlayerRadius();
    void LaunchBall(long holdTime);

    // Ball specific methods
    float GetBallXPos();
    float GetBallYPos();
    float GetBallRadius();

    // Brick specific methods
    int GetNumberOfBricks();
    bool GetIsAlive(int index);
    float GetBrickTop(int index);
    float GetBrickLeft(int index);
    float GetBrickRight(int index);
    float GetBrickBottom(int index);

    // Button methods
    float GetButtonRestartTop();
    float GetButtonRestartLeft();
    float GetButtonRestartRight();
    float GetButtonRestartBottom();
    std::string GetButtonRestartText();
    int GetButtonRestartTextSize();

    float GetButtonQuitTop();
    float GetButtonQuitLeft();
    float GetButtonQuitRight();
    float GetButtonQuitBottom();
    std::string GetButtonQuitText();
    int GetButtonQuitTextSize();


protected:

};


#endif //BREAKOUTCPP_ENGINE_H
