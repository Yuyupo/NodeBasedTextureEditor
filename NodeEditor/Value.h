#pragma once
#include <GL/gl3w.h>

enum class ValueType
{
    NONE,
    COLOR3,
    TEXTURE,
    INT,
    FLOAT,
    FLOAT3,
    FLOAT4
};

struct Color3
{
    Color3(float r, float g, float b);
    float r, g, b;
};

struct Texture
{
    Texture(int width, int height, GLuint texture);
    int width, height;
    GLuint texture;
};

struct Float3
{
    Float3(float x, float y, float z);
    float x, y, z;
};

struct Float4
{
    Float4(float x, float y, float z, float w);
    float x, y, z, w;
};

class Value
{
public:
    Value();
    Value(Color3& rgb);
    Value(Texture& texture);
    Value(float f);
    Value(int i);
    Value(Float3& float3);
    Value(Float4& float4);

    float asFloat();
    int asInt();
    Color3& asColor3();
    Texture& asTexture();
    Float3& asFloat3();
    Float4& asFloat4();
    ValueType getType();

private:
    union
    {
        Color3 m_color3;
        Texture m_texture;
        Float3 m_float3;
        Float4 m_float4;
        float m_f;
        int m_i;
    };

    ValueType m_type;
};

