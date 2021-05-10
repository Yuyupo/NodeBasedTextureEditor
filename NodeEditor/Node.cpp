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

    std::vector<GLubyte> emptyData(texture.width * texture.height * 4, 0);
    for (int count = 1; count < emptyData.size(); count ++)
    {
        if (count % 4 == 0) {
            emptyData[count -1] = 0xFF;
        }
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, emptyData.data());
}

void Node::drawTexture(Texture& texture)
{
    glBindTexture(GL_TEXTURE_2D, texture.texture);
    glBindVertexArray(Editor::getRenderingVAO());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Node::bindFramebuffer(Texture& texture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, Editor::getRenderingFrameBuffer());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.texture, 0);
    glViewport(0, 0, texture.width, texture.height);
}

void Node::resetFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Node::bindPreviewBuffer(Texture& texture)
{
    glBindFramebuffer(GL_FRAMEBUFFER, Editor::getPreviewFrameBuffer());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.texture, 0);
    glViewport(0, 0, texture.width, texture.height);
}




