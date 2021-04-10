#include "Value.h"
#include <GL/gl3w.h>   

Value::Value()
{
    m_type = ValueType::NONE;
}

Value::Value(Color3& rgb)
{
    m_color3 = rgb;
    m_type = ValueType::FLOAT3;
}

Value::Value(Texture& texture)
{
    m_texture = texture;
    m_type = ValueType::TEXTURE;
}

Value::Value(float f)
{
    m_f = f;
    m_type = ValueType::FLOAT;
}

Value::Value(int i)
{
    m_i = i;
    m_type = ValueType::INT;
}

Value::Value(Float3& float3)
{
    m_float3 = float3;
    m_type = ValueType::FLOAT3;
}

Value::Value(Float4& float4)
{
    m_float4 = float4;
    m_type = ValueType::FLOAT4;
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

ValueType Value::getType()
{
    return m_type;
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
