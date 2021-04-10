#pragma once
#include "Node.h"
class Add :
    public Node
{
public:
    Add();

    void createContent() override;
    Value createOutput() override;
};

