#include "imnodes.h"
#include "imgui.h"

#include "Node.h"
#include "Editor.h"

int Node::m_counter = 0;

Node::Node(std::string nodeName, AttributeType outputType)
    : m_id(++m_counter), m_name(nodeName)
{
    m_output = Editor::createAttribute("", outputType);
    m_output->setParent(this);
}

int Node::getID()
{
    return m_id;
}

std::string Node::getName()
{
    return m_name;
}

std::vector<Attribute*>& Node::getInputs()
{
    return m_inputs;
}

Attribute* Node::getOutput()
{
    return m_output;
}

void Node::addInput(std::string attributeName, AttributeType type)
{
    m_inputs.push_back(Editor::createAttribute(attributeName, type));
    m_inputs.back()->setParent(this);
}

void Node::createContent()
{
}

Value Node::createOutput()
{
    return Value();
}



