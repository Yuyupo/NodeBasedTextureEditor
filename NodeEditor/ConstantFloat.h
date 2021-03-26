#pragma once
#include "Node.h"
class ConstantFloat :
    public Node
{
public:
    ConstantFloat();

    void createContent() override;
    Value createOutput() override;

private:
    float m_float;
};

