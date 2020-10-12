
#ifndef BREAKOUTCPP_BUTTON_H
#define BREAKOUTCPP_BUTTON_H

#include <string>
#include "Rect.h"


class Button
{
private:
    Rect buttonShape;
    std::string buttonText;
    int textSize;
public:
    Button();
    Button(float x, float y, float width, float height, std::string text, int sizeOfText);
    Rect GetRect();
    std::string GetText();
    int GetTextSize();
    void SetText(std::string text);

protected:

};


#endif //BREAKOUTCPP_BUTTON_H
