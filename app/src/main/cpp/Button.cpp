//
// Created by USER on 2020-10-12.
//

#include "Button.h"

#include <utility>

Button::Button()
{
    buttonShape = Rect(0,0,0,0);
    buttonText = "";
    textSize = 0;
}

Button::Button(float x, float y, float width, float height, std::string text, int sizeOfText)
{
    buttonShape = Rect(x, y, x+width, y+height);
    buttonText = std::move(text);
    textSize = sizeOfText;
}

Rect Button::GetRect()
{
    return buttonShape;
}

std::string Button::GetText()
{
    return buttonText;
}

int Button::GetTextSize() {
    return textSize;
}

void Button::SetText(std::string text)
{
    buttonText = text;
}

