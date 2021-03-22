#include "imnodes.h"
#include "imgui.h"

#include "Node.h"

int Node::m_counter = 10;

Node::Node(std::string nodeName, AttributeType outputType)
    : m_id(++m_counter), m_name(nodeName), m_output("", outputType)
{
}

int Node::getID()
{
    return m_id;
}

std::string Node::getName()
{
    return m_name;
}

std::vector<Attribute>& Node::getInputs()
{
    return m_inputs;
}

Attribute& Node::getOutput()
{
    return m_output;
}

void Node::addInput(std::string attributeName, AttributeType type)
{
    m_inputs.emplace_back(attributeName, type);
}

void Node::createContent()
{
    ImGui::Text("kutyacica");
}

Value Node::createOutput()
{
    return Value();
}



