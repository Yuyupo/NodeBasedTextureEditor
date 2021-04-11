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
    // textura -> float3 -> int,float
    Value handleTexture(Texture texture, Value value);
    Value handleVectors(Float3 float3, Value value);

    Texture m_texture;
};

