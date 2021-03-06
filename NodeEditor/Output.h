#pragma once
#include "Node.h"
class Output :
    public Node
{
public:
    Output();
    ~Output();

    void createContent() override;
    Value createOutput() override;

private:
    Color3 m_color;
    Texture m_texture;
    Texture m_colorTexture;
};

