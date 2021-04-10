#pragma once
#include "Node.h"
class Add :
    public Node
{
public:
    Add();

    void createContent() override;
    Value createOutput() override;

private:
    Texture m_texture;
};

