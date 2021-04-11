#include "TextureLoader.h"
#include "Value.h"
#include "imgui.h"
#include <Windows.h>
#include <GL/gl3w.h>   

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureLoader::TextureLoader()
    : Node("2D Texture Loader")
    , m_path("C:\\Users\\Yuyupo\\Downloads\\MyImage01.jpg")
    , m_texture{ 128.f, 128.f, 0}
{
    generateTexture(m_texture);
}

void TextureLoader::createContent()
{
    ImGui::PushItemWidth(200.f);

    if (ImGui::InputText("Full path", m_path, IM_ARRAYSIZE(m_path)))
    {
        LoadTextureFromFile(m_path);
    }

    ImGui::Text("size = %d x %d", m_texture.width, m_texture.height);
}

Value TextureLoader::createOutput()
{
    return Value(m_texture);
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool TextureLoader::LoadTextureFromFile(const char* filename)
{
    unsigned char* image_data = stbi_load(filename, &m_texture.width, &m_texture.height, NULL, 4);
    if (image_data == NULL)
        return false;

    glBindTexture(GL_TEXTURE_2D, m_texture.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_texture.width, m_texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    return true;
}
