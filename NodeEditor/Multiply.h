#pragma once
#include "Node.h"

class Multiply :
    public Node
{
public:
    Multiply();

    void createContent() override;
    Value createOutput() override;

private:
    Value handleTexture(Texture texture, Value value);
    Value handleVectors(Float3 float3, Value value);

    Texture m_texture;
};

