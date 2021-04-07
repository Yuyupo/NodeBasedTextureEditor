#include "TextureLoader.h"
#include "Value.h"
#include "imgui.h"
#include <Windows.h>
#include <GL/gl3w.h>   

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureLoader::TextureLoader()
    : Node("2D Texture Loader", AttributeType::TEXTURELOADER)
    , m_path("")
    , m_width(0)
    , m_height(0)
    , m_texture(0)
{
}

void TextureLoader::createContent()
{
    ImGui::PushItemWidth(200.f);

    if (ImGui::InputText("Full path", m_path, IM_ARRAYSIZE(m_path)))
    {
        LoadTextureFromFile(m_path, &m_texture, &m_width, &m_height);
    }
    ImGui::Image((void*)(intptr_t)m_texture, ImVec2(m_width, m_height));

    ImGui::Text("pointer = %p", m_texture);
    ImGui::Text("size = %d x %d", m_width, m_height);
}

Value TextureLoader::createOutput()
{
    return Value(Texture(m_width, m_height, m_texture));
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool TextureLoader::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}
