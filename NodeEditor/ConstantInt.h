#pragma once
#include "Node.h"
class ConstantInt :
    public Node
{
public:
    ConstantInt();

    void createContent() override;
    Value createOutput() override;

private:
    int m_int;
};
