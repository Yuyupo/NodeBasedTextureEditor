#pragma once
#include "Node.h"

class ColorPicker :
    public Node
{
public:
    ColorPicker();

    void createContent() override;
    Value createOutput() override;

private:
    float m_color[3];
};

