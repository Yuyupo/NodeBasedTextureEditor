#include <iostream>
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imnodes.h"

#include "ConstantFloat4.h"
#include "ConstantFloat3.h"
#include "ConstantInt.h"
#include "ConstantFloat.h"
#include "ColorPicker.h"
#include "TextureLoader.h"
#include "Editor.h"
#include "Node.h"

std::vector<Attribute*> Editor::m_attributes;
std::vector<Node*> Editor::m_nodes;
std::vector<std::pair<int, int>> Editor::m_links;
bool Editor::active_menu = true;

void Editor::drawNodes()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Material editor", &active_menu, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Add node"))
        {
            if (ImGui::MenuItem("Color picker..", ""))
            { 
                addNode(new ColorPicker());
            }
            if (ImGui::MenuItem("Texture loader", ""))
            { 
                addNode(new TextureLoader());
            }
            if (ImGui::MenuItem("Const Int", ""))
            { 
                addNode(new ConstantInt());
            }
            if (ImGui::MenuItem("Const Float", ""))
            {
                addNode(new ConstantFloat());
            }
            if (ImGui::MenuItem("Const Float3", ""))
            {
                addNode(new ConstantFloat3());
            }
            if (ImGui::MenuItem("Const Float4", ""))
            {
                addNode(new ConstantFloat4());
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    imnodes::BeginNodeEditor();

    for (Node* node : m_nodes)
    {
        imnodes::BeginNode(node->getID());
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(node->getName().c_str());
        imnodes::EndNodeTitleBar();

        for (Attribute* inputAttr : node->getInputs())
        {
            imnodes::BeginInputAttribute(inputAttr->getID());
            ImGui::Text(inputAttr->getName().c_str());
            imnodes::EndInputAttribute();
        }

        if (node->getOutput()->getType() != AttributeType::NONE)
        {
            imnodes::BeginOutputAttribute(node->getOutput()->getID());
            imnodes::EndOutputAttribute();
        }

        node->createContent();

        imnodes::EndNode();
    }

    for (int i = 0; i < m_links.size(); ++i)
    {
        const std::pair<int, int> p = m_links[i];
        // in this case, we just use the array index of the link
        // as the unique identifier
        imnodes::Link(i, p.first, p.second);
    }

    imnodes::EndNodeEditor();
    ImGui::End();
    // Rendering
    ImGui::Render();
}

void Editor::addNode(Node* node)
{
    m_nodes.push_back(node);
}

void Editor::addLink(int attribute1, int attribute2)
{
    m_links.push_back(std::make_pair(attribute1, attribute2));
}

Attribute* Editor::createAttribute(std::string name, AttributeType type)
{
    m_attributes.push_back(new Attribute(m_attributes.size(), name, type));
    return m_attributes.back();
}

void Editor::handleEvents()
{
    int start_attr, end_attr;
    if (imnodes::IsLinkCreated(&start_attr, &end_attr))
    {
        std::cout << "New link: start:" << start_attr << ", end:" << end_attr << std::endl;
        addLink(start_attr, end_attr);

        if (end_attr == 1)
        {
            Color3 col1 = m_attributes[start_attr]->getParent()->createOutput().asColor3();
            std::cout << m_attributes[start_attr]->getParent()->getName();
            std::cout << "\tColor chosen R:" << (int)(col1.r * 255.f) << " G:" << (int)(col1.g * 255.f) << " B:" << (int)(col1.b * 255.f) << std::endl;
        }

        //if (start_attr == 2 && end_attr == 1) {
            //std::cout << "\tColor chosen R:" << (int)(col1[0] * 255.f) << " G:" << (int)(col1[1] * 255.f) << " B:" << (int)(col1[2] * 255.f) << std::endl;
        //}
    }

    int link_id;
    if (imnodes::IsLinkHovered(&link_id) && (GetKeyState('D') & 0x8000))
    {
        std::cout << "Deleted link: " << m_links.at(link_id).first << " " << m_links.at(link_id).second << std::endl;
        m_links.erase(m_links.begin() + link_id);
        std::cout << "Remaining links: " << m_links.size() << std::endl;
    }

    int node_id;
    if (imnodes::IsNodeHovered(&node_id) && (GetKeyState('D') & 0x8000))
    {
        std::cout << "Deleted node named: " << m_nodes[node_id-1]->getName() << std::endl;

        for (int i = 0; i < m_links.size(); ++i)
        {
            int firstID = m_links[i].first;
            int secondID = m_links[i].second;
            if ((m_attributes[firstID]->getParent()->getID() == node_id)
                || (m_attributes[secondID]->getParent()->getID() == node_id))
            {
                m_links.erase(m_links.begin() + i);
            }
        }
      // m_nodes.erase(m_nodes.begin() + (node_id - 1));
    }
}

void Editor::init(const char* glsl_version, GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    imnodes::Initialize();

    Node* n = new Node("Output", AttributeType::NONE);
    n->addInput("Color", AttributeType::COLOR3);
    n->addInput("Texture", AttributeType::TEXTURELOADER);
    addNode(n);
}

void Editor::cleanUp()
{
    for (Node* node : m_nodes)
    {
        delete node;
    }
}
