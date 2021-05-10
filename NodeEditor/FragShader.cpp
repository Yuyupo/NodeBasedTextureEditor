#include "FragShader.h"
#include "Editor.h"
#include "imgui.h"

static const char* fragmentShaderString = R"(#version 310 es
precision highp float;
in vec2 vTex;
uniform sampler2D inputImage;
out vec4 outColor;
void main() {
    outColor = vec4(texture(inputImage, vTex).rgb, 1.0f);
})";

static const char* vertexShaderString = R"(#version 310 es
    precision highp float;
    in vec2 aPos;
    in vec2 aTex;
    out vec2 vTex;
    void main() {
        gl_Position = vec4(aPos, 0.0f, 1.0f);
        vTex = aTex;
})";

FragShader::FragShader()
    : Node("Custom Fragment Shader")
    , m_colorTexture{ 256.f, 256.f, 0.f }
    , m_texture{ 256.f, 256.f, 0.f }
    , m_texture_program(0)
{
    generateTexture(m_colorTexture);
    generateTexture(m_texture);

    strcpy(m_customFragShader, fragmentShaderString);
    m_texture_program = Editor::createShaderProgram(vertexShaderString, m_customFragShader);

    addInput("");
}

FragShader::~FragShader()
{
    glDeleteProgram(m_texture_program);
}

void FragShader::createContent()
{
    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
    ImGui::InputTextMultiline("##source", m_customFragShader, IM_ARRAYSIZE(m_customFragShader), ImVec2(400.f, ImGui::GetTextLineHeight() * 8), flags);
    if (ImGui::Button("Compile")) {
        glDeleteProgram(m_texture_program);
        m_texture_program = Editor::createShaderProgram(vertexShaderString, m_customFragShader);
    }
    if (m_texture_program == 0)
    {
        ImGui::Text("Compile error");
    }
}

Value FragShader::createOutput()
{
    glUseProgram(m_texture_program);

    Value InValue = getInputValue(0);
    Value retValue = Value();

    if (InValue.getType() == ValueType::FLOAT3)
    {
        bindFramebuffer(m_colorTexture);
        Color3 color = InValue.asColor3();

        glClearColor(color.r, color.g, color.b, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        bindFramebuffer(m_texture);
        drawTexture(m_colorTexture);

        retValue = Value(Texture(m_texture));
    }
    else if (InValue.getType() == ValueType::TEXTURE)
    {
        bindFramebuffer(m_texture);
        Texture InTexture = InValue.asTexture();

        drawTexture(InTexture);

        retValue = Value(Texture(m_texture));
    }
    glUseProgram(Editor::getDefaultShaderProgram());
    resetFrameBuffer();

    return Value(retValue);
}
