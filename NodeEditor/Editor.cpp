#include <iostream>
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imnodes.h"

#include "ColorPicker.h"
#include "Node.h"
#include "Editor.h"

Editor::Editor(const char* glsl_version, GLFWwindow* window)
{
    init(glsl_version, window);

    Node* n = new Node("Output", AttributeType::NONE);
    n->addInput("Color", AttributeType::COLOR3);
    addNode(n);
}

Editor::~Editor()
{
    for (Node* node : m_nodes)
    {
        delete node;
    }
}

void Editor::drawNodes()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Material editor");
    imnodes::BeginNodeEditor();

    for (Node* node : m_nodes)
    {
        imnodes::BeginNode(node->getID());
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted(node->getName().c_str());
        imnodes::EndNodeTitleBar();

        for (Attribute& inputAttr : node->getInputs())
        {
            imnodes::BeginInputAttribute(inputAttr.getID());
            ImGui::Text(inputAttr.getName().c_str());
            imnodes::EndInputAttribute();
        }

        if (node->getOutput().getType() != AttributeType::NONE)
        {
            imnodes::BeginOutputAttribute(node->getOutput().getID());
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

void Editor::handleEvents()
{
    //TODO : optimize this (Y)
    int start_attr, end_attr;
    if (imnodes::IsLinkCreated(&start_attr, &end_attr))
    {
        std::cout << "New link: start:" << start_attr << ", end:" << end_attr << std::endl;
        addLink(start_attr, end_attr);

        //if (start_attr == 2 && end_attr == 1) {
            //std::cout << "\tColor chosen R:" << (int)(col1[0] * 255.f) << " G:" << (int)(col1[1] * 255.f) << " B:" << (int)(col1[2] * 255.f) << std::endl;
        //}
    }

    int link_id;
    if (imnodes::IsLinkHovered(&link_id) && (GetKeyState('A') & 0x8000))
    {
        std::cout << "Deleted link: " << m_links.at(link_id).first << " " << m_links.at(link_id).second << std::endl;
        m_links.erase(m_links.begin() + link_id);
        std::cout << "Remaining links: " << m_links.size() << std::endl;
    }

    // TODO: KeyReleased
    if ((GetKeyState('C') & 0x8000))
    {
        addNode(new Node("kutya", AttributeType::NONE));
        addNode(new ColorPicker());
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
}
