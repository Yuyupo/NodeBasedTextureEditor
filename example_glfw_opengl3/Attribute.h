#pragma once
#include <string>

enum class AttributeType
{
    NONE,
    COLOR3
};

class Attribute
{
public:
    Attribute(std::string attributeName, AttributeType type);
    int getID();
    AttributeType getType();
    std::string getName();

private:
    AttributeType m_type;
    static int m_counter;
    int m_id;
    std::string m_name;
};
