#pragma once
#include "Node.h"

class TextureLoader :
    public Node
{
public:
    TextureLoader();

    void createContent() override;
    Value createOutput() override;

private:
    bool LoadTextureFromFile(const char* filename);

    char m_path[128];
    Texture m_texture;
};

