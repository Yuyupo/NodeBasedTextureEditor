#pragma once
#include "Node.h"
class Subtract :
    public Node
{
public:
    Subtract();

    void createContent() override;
    Value createOutput() override;

private:
    Value handleTexture(Texture texture, Value value);
    Value handleVectors(Float3 float3, Value value);

    Texture m_texture;
};

