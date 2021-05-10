#pragma once
#include "Node.h"
class Preview :
    public Node
{
public:
    Preview();
    ~Preview();

    void createContent() override;
    Value createOutput() override;

private:
    GLuint m_previewFrameBuffer;

    Color3 m_color;
    Texture m_texture;
    Texture m_colorTexture;
};

