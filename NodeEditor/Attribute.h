#pragma once
#include <string>

class Node;

enum class AttributeType
{
    NONE,
    COLOR3
};

class Attribute
{
public:
    Attribute(int id, std::string attributeName, AttributeType type);
    int getID();
    AttributeType getType();
    std::string getName();
    void setParent(Node* node);
    Node* getParent();

private:
    Node* m_parent;
    AttributeType m_type;
    int m_id;
    std::string m_name;
};
