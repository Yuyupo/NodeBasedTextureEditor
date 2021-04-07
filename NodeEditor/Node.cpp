#include "imnodes.h"
#include "imgui.h"

#include "Node.h"
#include "Editor.h"

Node::Node(std::string nodeName, AttributeType outputType)
    : m_id(0), m_name(nodeName)
{
    m_output = Editor::createAttribute("", outputType);
    m_output->setParent(this);
}

Node::~Node()
{
    for (Attribute* inputAttr : getInputs())
    {
        //Editor::deleteAttribute(inputAttr->getID());
    }

    //Editor::deleteAttribute(getOutput()->getID());
}

int Node::getID()
{
    return m_id;
}

void Node::setID(int id)
{
    m_id = id;
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



