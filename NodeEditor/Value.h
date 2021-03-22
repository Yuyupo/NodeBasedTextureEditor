#pragma once
struct Color3
{
    Color3(float r, float g, float b);
    float r, g, b;
};

class Value
{
public:
    Value();
    Value(Color3& rgb);
    Value(float f);
    Value(int i);

    float asFloat();
    int asInt();
    Color3& asColor3();

private:
    union
    {
        Color3 m_color3;
        float m_f;
        int m_i;
    };
};

