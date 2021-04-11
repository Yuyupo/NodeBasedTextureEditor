#pragma once
#include <string>
#include <vector>
#include "Attribute.h"
#include "Value.h"

class Node
{
public:
    Node(std::string nodeName);
    ~Node();
    int getID();
    void setID(int id);
    std::string getName();
    std::vector<Attribute*>& getInputs();
    Attribute* getOutput();
    void addInput(std::string attributeName);
    Value getInputValue(int idx);

    virtual void createContent();
    virtual Value createOutput();

protected:
    static void generateTexture(Texture& texture);
    static void bindFramebuffer(Texture& texture);
    static void resetFrameBuffer();

private:
    int m_id;
    std::string m_name;
    std::vector<Attribute*> m_inputs;
    Attribute* m_output;
};
