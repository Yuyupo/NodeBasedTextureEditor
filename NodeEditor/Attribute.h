#pragma once
#include <string>

class Node;

class Attribute
{
public:
    Attribute(int id, std::string attributeName);
    int getID();
    std::string getName();
    void setParent(Node* node);
    Node* getParent();

private:
    Node* m_parent;
    int m_id;
    std::string m_name;
};
