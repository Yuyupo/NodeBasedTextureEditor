#include <iostream>
#include <Windows.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imnodes.h"

#include "Output.h"
#include "ConstantFloat4.h"
#include "ConstantFloat3.h"
#include "ConstantInt.h"
#include "ConstantFloat.h"
#include "ColorPicker.h"
#include "ChannelPicker.h"
#include "TextureLoader.h"

#include "Add.h"
#include "Multiply.h"
#include "Subtract.h"

#include "Editor.h"
#include "Node.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

GLuint Editor::m_renderingFrameBuffer;
GLuint Editor::m_renderingVertexArrayObject;
GLuint Editor::m_renderingVertexBuffer;
std::vector<Attribute*> Editor::m_attributes;
std::vector<Node*> Editor::m_nodes;
std::vector<std::pair<int, int>> Editor::m_links;
bool Editor::active_menu = true;

static unsigned int createShaderProgram(const char* vertex_src, const char* fragment_src) {
    // X.1. Create the vertex shader.
    unsigned int vertex_shader;
    {
        // X.1.1. Create a vertex shader object.
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);

        // X.1.2. Specify the shader source.
        glShaderSource(vertex_shader, 1, &vertex_src, NULL);

        // X.1.3. Compile the shader.
        glCompileShader(vertex_shader);

        // X.1.4. Check if there is any error during compilation.
        int success;
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info[512];
            glGetShaderInfoLog(vertex_shader, 512, NULL, info);
            printf("Vertex shader error:\n%s\n", info);
            exit(-3);
        }
    }

    // X.2. Create the fragment shader.
    unsigned int fragment_shader;
    {
        // X.2.1. Create a fragment shader object.
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

        // X.2.2. Specify the shader source.
        glShaderSource(fragment_shader, 1, &fragment_src, NULL);

        // X.2.3. Compile the shader.
        glCompileShader(fragment_shader);

        // X.2.4. Check if there is any error during compilation.
        int success;
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char info[512];
            glGetShaderInfoLog(fragment_shader, 512, NULL, info);
            printf("Fragment shader error:\n%s\n", info);
            exit(-3);
        }
    }

    // X.3. Create a shader program and attach the vertex/fragment shaders.
    unsigned int shader_program;
    {
        shader_program = glCreateProgram();
        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);
        glLinkProgram(shader_program);

        int success;
        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
        if (!success) {
            char info[512];
            glGetProgramInfoLog(shader_program, 512, NULL, info);
            printf("Program error:\n%s\n", info);
            exit(-3);
        }
    }

    // X.4. The vertex and fragment shaders can be removed after linking.
    {
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
    }

    return shader_program;
}

void Editor::drawNodes()
{
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Material editor", &active_menu, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("New node"))
        {
            if (ImGui::MenuItem("Color picker", ""))
            { 
                createNode<ColorPicker>();
            }
            if (ImGui::MenuItem("Color channel picker", ""))
            {
                Node* n = createNode<ChannelPicker>();
            }
            if (ImGui::MenuItem("Texture loader", ""))
            { 
                createNode<TextureLoader>();
            }
            if (ImGui::BeginMenu("Constants", ""))
            {
                if (ImGui::MenuItem("Const Int", ""))
                {
                    createNode<ConstantInt>();
                }
                if (ImGui::MenuItem("Const Float", ""))
                {
                    createNode<ConstantFloat>();
                }
                if (ImGui::MenuItem("Const Float3", ""))
                {
                    createNode<ConstantFloat3>();
                }
                if (ImGui::MenuItem("Const Float4", ""))
                {
                    createNode<ConstantFloat4>();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Operations", ""))
            {
                if (ImGui::MenuItem("Add", ""))
                {
                    createNode<Add>();
                }
                if (ImGui::MenuItem("Subtract", ""))
                {
                    createNode<Subtract>();
                }
                if (ImGui::MenuItem("Multiply", ""))
                {
                    createNode<Multiply>();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        static char m_path[128] = ".\\OutputTextures\\";
        ImGui::InputText("\tFilename", m_path, IM_ARRAYSIZE(m_path));
        if (ImGui::Button("Save output texture"))
        {
            int width = 256;
            int height = 256;
            static GLubyte* data = new GLubyte[4 * width * height];
            memset(data, 0, 4 * width * height);
            // Blit the multisample framebuffer into the regular framebuffer
            glBindFramebuffer(GL_READ_FRAMEBUFFER, getRenderingFrameBuffer());
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, getRenderingFrameBuffer());
            glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

            // Read from the regular framebuffer into the data array
            glBindFramebuffer(GL_FRAMEBUFFER, getRenderingFrameBuffer());
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
            std::cout << "Saved as: " << m_path << std::endl;
            stbi_write_png(strcat(m_path, ".png"), width, height, 4, data, width * 4);
        }
        ImGui::EndMenuBar();
    }
    imnodes::BeginNodeEditor();

    for (Node* node : m_nodes)
    {
        if (node == nullptr) continue;
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

        if (node->getName() != "Output")
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

Attribute* Editor::createAttribute(std::string name)
{
    for (size_t i = 0; i < m_attributes.size(); i++)
    {
        if (m_attributes[i] == nullptr)
        {
            m_attributes[i] = new Attribute(i, name);
            return m_attributes[i];
        }
    }

    m_attributes.push_back(new Attribute(m_attributes.size(), name));
    return m_attributes.back();
}

void Editor::deleteAttribute(int id)
{
    delete m_attributes[id];
    m_attributes[id] = nullptr;
}

Node* Editor::getInputNode(Attribute* attr)
{
    for (size_t i = 0; i < m_links.size(); i++)
    {
        if (m_links[i].second == attr->getID())
        {
            return m_attributes[m_links[i].first]->getParent();
        }
    }
    return nullptr;
}

void Editor::deleteNode(int id)
{
    delete m_nodes[id];
    m_nodes[id] = nullptr;
}

GLuint Editor::getRenderingFrameBuffer()
{
    return m_renderingFrameBuffer;
}

GLuint Editor::getRenderingVAO()
{
    return m_renderingVertexArrayObject;
}

template <typename NodeType>
Node* Editor::createNode()
{
    Node* node = new NodeType();
    for (size_t i = 0; i < m_nodes.size(); i++)
    {
        if (m_nodes[i] == nullptr)
        {
            node->setID(i);
            m_nodes[i] = node;
            return m_nodes[i];
        }
    }

    node->setID(m_nodes.size());
    m_nodes.push_back(node);
    return m_nodes.back();
}

void Editor::handleEvents()
{
    int start_attr, end_attr;
    if (imnodes::IsLinkCreated(&start_attr, &end_attr))
    {
        std::cout << "New link: start:" << start_attr << ", end:" << end_attr << std::endl;
        addLink(start_attr, end_attr);

        // Output Color
        if (end_attr == 1)
        {
            Color3 col1 = m_attributes[start_attr]->getParent()->createOutput().asColor3();
            std::cout << "Link sent from: " << m_attributes[start_attr]->getParent()->getName() << "\tTo: " << m_attributes[end_attr]->getParent()->getName() << std::endl;
            std::cout << "\tColor chosen R:" << (int)(col1.r * 255.f) << " G:" << (int)(col1.g * 255.f) << " B:" << (int)(col1.b * 255.f) << std::endl;
        }

        // Output Texture
        if (end_attr == 2)
        {
            Texture tex = m_attributes[start_attr]->getParent()->createOutput().asTexture();
            std::cout << "Link sent from: " << m_attributes[start_attr]->getParent()->getName() << "\tTo: " << m_attributes[end_attr]->getParent()->getName() << std::endl;
            std::cout << "\t Texture size: Height: " << tex.height << ", Width: " << tex.width << std::endl;
        }

        if (m_attributes[end_attr]->getParent()->getName() == "Color Channel Picker")
        {
            Color3 col1 = m_attributes[start_attr]->getParent()->createOutput().asColor3();
            std::cout << "Link sent from: " << m_attributes[start_attr]->getParent()->getName() << "\tTo: " << m_attributes[end_attr]->getParent()->getName() << std::endl; 
            std::cout << "\tColor chosen R:" << (int)(col1.r * 255.f) << " G:" << (int)(col1.g * 255.f) << " B:" << (int)(col1.b * 255.f) << std::endl;
        }

        if (m_attributes[end_attr]->getParent()->getName() == "Add")
        {
            Color3 col1 = m_attributes[start_attr]->getParent()->createOutput().asColor3();
            std::cout << "Link sent from: " << m_attributes[start_attr]->getParent()->getName() << "\tTo: " << m_attributes[end_attr]->getParent()->getName() << std::endl;
            std::cout << "\tColor chosen R:" << (int)(col1.r * 255.f) << " G:" << (int)(col1.g * 255.f) << " B:" << (int)(col1.b * 255.f) << std::endl;
        }
    }

    int link_id;
    if (imnodes::IsLinkHovered(&link_id) && (GetKeyState('D') & 0x8000))
    {
        std::cout << "Deleted link: " << m_links.at(link_id).first << " " << m_links.at(link_id).second << std::endl;
        m_links.erase(m_links.begin() + link_id);
        std::cout << "Remaining links: " << m_links.size() << std::endl;
    }

    int node_id;
    if (imnodes::IsNodeHovered(&node_id) && (GetKeyState('D') & 0x8000) && m_nodes[node_id]->getName() != "Output")
    {
        std::cout << "Deleted node named: " << m_nodes[node_id]->getName() << std::endl;

        for (int i = 0; i < m_links.size(); ++i)
        {
            int firstID = m_links[i].first;
            int secondID = m_links[i].second;
            if ((m_attributes[firstID]->getParent()->getID() == node_id)
                || (m_attributes[secondID]->getParent()->getID() == node_id))
            {
                std::cout << "\tDeleted node links: " << firstID << " " << secondID << std::endl;
                m_links.erase(m_links.begin() + i);
                i--;
            }
        }

        deleteNode(node_id);
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

    const char* texture_display_vertex_src = R"(#version 310 es
    precision highp float;
    in vec2 aPos;
    in vec2 aTex;
    out vec2 vTex;
    void main() {
        gl_Position = vec4(aPos, 0.0f, 1.0f);
        vTex = aTex;
    })";

    const char* texture_display_fragment_src = R"(#version 310 es
    precision highp float;
    in vec2 vTex;
    uniform sampler2D inputImage;
    out vec4 outColor;
    void main() {
        outColor = vec4(texture(inputImage, vTex).rgb, 1.0f);
    })";

    unsigned int texture_program = createShaderProgram(texture_display_vertex_src, texture_display_fragment_src);
    glUseProgram(texture_program);

    glGenFramebuffers(1, &m_renderingFrameBuffer);

    glGenBuffers(1, &m_renderingVertexBuffer);
    glGenVertexArrays(1, &m_renderingVertexArrayObject);
    glBindVertexArray(m_renderingVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_renderingVertexBuffer);

    int aPosLoc = glGetAttribLocation(texture_program, "aPos");
    int aTexLoc = glGetAttribLocation(texture_program, "aTex");

    // Vertex coords
    glVertexAttribPointer(aPosLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(aPosLoc);

    // UV coords
    glVertexAttribPointer(aTexLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (const GLvoid*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(aTexLoc);
    glBindVertexArray(0);

    static const GLfloat vertices[] = {
        -1.0,  1.0,  0.0, 1.0,
        -1.0, -1.0,  0.0, 0.0,
         1.0,  1.0,  1.0, 1.0,
         1.0, -1.0,  1.0, 0.0,
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    createNode<Output>();
}

void Editor::cleanUp()
{
    glDeleteFramebuffers(1, &m_renderingFrameBuffer);
    glDeleteVertexArrays( 1, &m_renderingVertexArrayObject);
    glDeleteBuffers(1, &m_renderingVertexBuffer);

    for (Node* node : m_nodes)
    {
        delete node;
    }
}

