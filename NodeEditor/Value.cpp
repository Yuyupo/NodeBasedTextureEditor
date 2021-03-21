#include "Value.h"

Value::Value()
{
    //NOTE: 0 out the biggest member
    m_f = 0.f;
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
