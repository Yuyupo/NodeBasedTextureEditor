#pragma once
class Value
{
public:
    Value();
    Value(float f);
    Value(int i);

    float asFloat();
    int asInt();

private:
    union
    {
        float m_f;
        int m_i;
    };
};

