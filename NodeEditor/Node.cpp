#include "imnodes.h"
#include "imgui.h"

#include "Node.h"
#include "Editor.h"

Node::Node(std::string nodeName)
    : m_id(0), m_name(nodeName)
{
    m_output = Editor::createAttribute("");
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

void Node::addInput(std::string attributeName)
{
    m_inputs.push_back(Editor::createAttribute(attributeName));
    m_inputs.back()->setParent(this);
}

Value Node::getInputValue(int idx)
{
    Node* InputNode = Editor::getInputNode(getInputs()[idx]);

    if (InputNode == nullptr) {
        return Value();
    }

    return InputNode->createOutput();
}

void Node::createContent()
{
}

Value Node::createOutput()
{
    return Value();
}

void Node::generateTexture(Texture& texture)
{

    glGenTextures(1, &texture.texture);
    glBindTexture(GL_TEXTURE_2D, texture.texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // set alpha to 1.f
    std::vector<GLubyte> emptyData(texture.width * texture.height * 4, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, emptyData.data());
}



