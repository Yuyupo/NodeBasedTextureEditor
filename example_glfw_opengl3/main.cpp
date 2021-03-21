#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imnodes.h"
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <GL/gl3w.h>           
#include <GLFW/glfw3.h>

#include "Node.h"
#include "Attribute.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

// Simple helper function to load an image into a OpenGL texture with common settings
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Node based shader editor", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (gl3wInit() != 0)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

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

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    std::vector<Node> nodes;
    std::vector<std::pair<int, int>> links;
    static float col1[3] = { 0.f, 0.f, 0.f };
    int my_image_width = 0;
    int my_image_height = 0;
    GLuint my_image_texture = 0;
    GLuint my_color_texture = 0;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Material editor");
        imnodes::BeginNodeEditor();

        // First node
        imnodes::BeginNode(1);
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Output");
        imnodes::EndNodeTitleBar();

        // Color attribute
        imnodes::BeginInputAttribute(1);
        ImGui::Text("Color");
        //ehhez hozzáadni a texturecolorbuffer id-t
        ImGui::Image((void*)(intptr_t)my_color_texture, ImVec2(50, 50), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        imnodes::EndInputAttribute();

        //Texture attribute
        imnodes::BeginInputAttribute(4);
        ImGui::Text("Texture");
        imnodes::EndInputAttribute();
        imnodes::EndNode();
        //First node end

        //Second node
        imnodes::BeginNode(2);
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("Color picker");
        imnodes::EndNodeTitleBar();

        imnodes::BeginOutputAttribute(2);
        ImGui::ColorEdit3("Color", col1);
        imnodes::EndOutputAttribute();
        imnodes::EndNode();
        //Second node end

        //Third node
        imnodes::BeginNode(3);
        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("2D Texture loader");
        imnodes::EndNodeTitleBar();

        imnodes::BeginOutputAttribute(3);
        imnodes::EndOutputAttribute();
        static char str0[128] = "";
        // C:\\Users\\Yuyupo\\Downloads\\MyImage01.jpg

        if (ImGui::InputText("Full path", str0, IM_ARRAYSIZE(str0))) {
            std::cout << "Load texture called with path: " << str0 << std::endl;
            LoadTextureFromFile(str0, &my_image_texture, &my_image_width, &my_image_height);
        }
        ImGui::Image((void*)(intptr_t)my_image_texture, ImVec2(my_image_width, my_image_height));

        ImGui::Text("pointer = %p", my_image_texture);
        ImGui::Text("size = %d x %d", my_image_width, my_image_height);
        imnodes::EndNode();
        //Third node end

        for (int i = 0; i < links.size(); ++i)
        {
            const std::pair<int, int> p = links[i];
            // in this case, we just use the array index of the link
            // as the unique identifier
            imnodes::Link(i, p.first, p.second);
        }

        for (Node& node : nodes)
        {
            imnodes::BeginNode(node.getID());
            imnodes::BeginNodeTitleBar();
            ImGui::TextUnformatted(node.getName().c_str());
            imnodes::EndNodeTitleBar();

            for (Attribute& inputAttr : node.getInputs())
            {
                imnodes::BeginInputAttribute(inputAttr.getID());
                ImGui::Text(inputAttr.getName().c_str());
                imnodes::EndInputAttribute();
            }

            if (node.getOutput().getType() != AttributeType::NONE)
            {
                imnodes::BeginOutputAttribute(node.getOutput().getID());
                imnodes::EndOutputAttribute();
            }

            node.createContent();

            imnodes::EndNode();
        }

        imnodes::EndNodeEditor();

        int start_attr, end_attr;
        if (imnodes::IsLinkCreated(&start_attr, &end_attr))
        {
            std::cout << "New link: start:" << start_attr << ", end:" << end_attr << std::endl;
            links.push_back(std::make_pair(start_attr, end_attr));
            if (start_attr == 2 && end_attr == 1) {
                std::cout << "\tColor chosen R:" << (int)(col1[0]*255.f) << " G:" << (int)(col1[1] * 255.f) << " B:" << (int)(col1[2] * 255.f) << std::endl;
            }
            if (start_attr == 3 && end_attr == 1) {
               // std::cout << "\tColor chosen R:" << (int)(col2[0] * 255.f) << " G:" << (int)(col2[1] * 255.f) << " B:" << (int)(col2[2] * 255.f) << std::endl;
            }
        }

        int link_id;
        if (imnodes::IsLinkHovered(&link_id) && (GetKeyState('A') & 0x8000))
        {
            std::cout << "Deleted link: " << links.at(link_id).first << " "<< links.at(link_id).second << std::endl;
            links.erase(links.begin() + link_id);
            std::cout << "Remaining links: " << links.size() << std::endl;
        }

        // released
        if ((GetKeyState('N') & 0x8000))
        {
            nodes.emplace_back("kutya", AttributeType::NONE);
            nodes.emplace_back("macska", AttributeType::COLOR3);
            nodes.back().getInputs().emplace_back("hulululu", AttributeType::COLOR3);
            std::cout << "\tColor R:" << (int)(col1[0] * 255.f) << " G:" << (int)(col1[1] * 255.f) << " B:" << (int)(col1[2] * 255.f) << std::endl;
        }

        ImGui::End();
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    imnodes::Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
