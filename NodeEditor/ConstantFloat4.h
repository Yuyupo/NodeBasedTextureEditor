#pragma once
#include "Node.h"
class ConstantFloat4 :
    public Node
{
public:
    ConstantFloat4();

    void createContent() override;
    Value createOutput() override;

private:
    float m_xyzw[4];
};

