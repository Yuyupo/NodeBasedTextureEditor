#include "Value.h"
#include <GL/gl3w.h>   

Value::Value()
{
    //NOTE: 0 out the biggest member
    m_float4 = { 0.f, 0.f, 0.f , 0.f};
}

Value::Value(Color3& rgb)
{
    m_color3 = rgb;
}

Value::Value(Texture& texture)
{
    m_texture = texture;
}

Value::Value(float f)
{
    m_f = f;
}

Value::Value(int i)
{
    m_i = i;
}

Value::Value(Float3& float3)
{
    m_float3 = float3;
}

Value::Value(Float4& float4)
{
    m_float4 = float4;
}

float Value::asFloat()
{
    return m_f;
}

int Value::asInt()
{
    return m_i;
}

Color3& Value::asColor3()
{
    return m_color3;
}

Texture& Value::asTexture()
{
    return m_texture;
}

Float3& Value::asFloat3()
{
    return m_float3;
}

Float4& Value::asFloat4()
{
    return m_float4;
}

Color3::Color3(float r, float g, float b) 
    : r(r), g(g), b(b)
{
}

Texture::Texture(int width, int height, GLuint texture)
    : width(width), height(height), texture(texture)
{
}

Float3::Float3(float x, float y, float z)
    : x(x), y(y), z(z)
{
}

Float4::Float4(float x, float y, float z, float w)
    : x(x), y(y), z(z), w(w)
{
}
