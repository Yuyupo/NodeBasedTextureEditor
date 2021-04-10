#include "Attribute.h"

Attribute::Attribute(int id, std::string attributeName)
{
    m_id = id;
    m_parent = nullptr;
    m_name = attributeName;
}

int Attribute::getID()
{
    return m_id;
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



