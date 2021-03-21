#include "Attribute.h"

int Attribute::m_counter = 10;

Attribute::Attribute(std::string attributeName, AttributeType type)
{
    m_id = ++m_counter;
    m_type = type;
    m_name = attributeName;
}

int Attribute::getID()
{
    return m_id;
}

AttributeType Attribute::getType()
{
    return m_type;
}

std::string Attribute::getName()
{
    return m_name;
}



