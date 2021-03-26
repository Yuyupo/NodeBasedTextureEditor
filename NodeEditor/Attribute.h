#pragma once
#include <string>

class Node;

// Input/output tipusokat picit fliki
// float2/3 err -> text boxban error?
enum class AttributeType
{
    NONE,
    COLOR3,
    TEXTURELOADER,
    CONST_INT,
    CONST_FLOAT,
    CONST_FLOAT3,
    CONST_FLOAT4
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
