#pragma once
#include <string>
#include <vector>
#include "Attribute.h"
#include "Value.h"

class Node
{
public:
    Node(std::string nodeName, AttributeType outputType);
    int getID();
    std::string getName();
    std::vector<Attribute*>& getInputs();
    Attribute* getOutput();
    void addInput(std::string attributeName, AttributeType type);

    virtual void createContent();
    virtual Value createOutput();

private:
    static int m_counter;
    int m_id;
    std::string m_name;
    std::vector<Attribute*> m_inputs;
    Attribute* m_output;
};
