#pragma once
#include "Node.h"
class ChannelPicker :
    public Node
{
public:
    ChannelPicker();

    void createContent() override;
    Value createOutput() override;

private:
    Color3 m_color;
    bool m_red, m_green, m_blue;
};

