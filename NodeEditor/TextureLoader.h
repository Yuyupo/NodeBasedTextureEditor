#pragma once
#include "Node.h"

class TextureLoader :
    public Node
{
public:
    TextureLoader();

    bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

    void createContent() override;
    Value createOutput() override;

private:
    char m_path[128];
    int m_width;
    int m_height;
    GLuint m_texture;
};

