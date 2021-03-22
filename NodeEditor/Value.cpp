#include "Value.h"

Value::Value()
{
    //NOTE: 0 out the biggest member
    m_color3 = { 0.f, 0.f, 0.f };
}

Value::Value(Color3& rgb)
{
    m_color3 = rgb;
}

Value::Value(float f)
{
    m_f = f;
}

Value::Value(int i)
{
    m_i = i;
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

Color3::Color3(float r, float g, float b) 
    : r(r), g(g), b(b)
{
}
