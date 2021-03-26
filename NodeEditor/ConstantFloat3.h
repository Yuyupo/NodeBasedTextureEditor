#pragma once
#include "Node.h"

class ConstantFloat3 :
    public Node
{
public:
    ConstantFloat3();

    void createContent() override;
    Value createOutput() override;

private:
    float m_xyz[3];
};
