#pragma once
#include "Node.h"
class FragShader :
    public Node
{
public:
    FragShader();
    ~FragShader();

    void createContent() override;
    Value createOutput() override;

private:
    unsigned int m_texture_program;
    char m_customFragShader[1024 * 16];

    Texture m_texture;
    Texture m_colorTexture;
};

