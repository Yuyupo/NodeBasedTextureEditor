#include "Attribute.h"

Attribute::Attribute(int id, std::string attributeName, AttributeType type)
{
    m_id = id;
    m_parent = nullptr;
    m_type = type;
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

void Attribute::setParent(Node* node)
{
    m_parent = node;
}

Node* Attribute::getParent()
{
    return m_parent;
}



