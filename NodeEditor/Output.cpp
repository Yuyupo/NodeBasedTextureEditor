#include "Output.h"
#include "Editor.h"
#include "imgui.h"

Output::Output()
	: Node("Output")
	, m_color{ 0.f, 0.f, 0.f }
	, m_texture { 50.f, 50.f, 0}
	, m_colorTexture {256.f, 256.f, 0}
	, m_fb (Editor::getRenderingFrameBuffer())
{
	// m_texture comes from outside
	generateTexture(m_colorTexture);

	addInput("Texture");
}

Output::~Output()
{
}

void Output::createContent()
{
	createOutput();
	if (getInputValue(0).getType() == ValueType::FLOAT3)
	{
		ImGui::Text("\Preview");
		ImGui::Image((void*)(intptr_t)m_colorTexture.texture, ImVec2(100.f, 100.f));
	}
	else if (getInputValue(0).getType() == ValueType::TEXTURE)
	{
		ImGui::Text("\Preview");
		ImGui::Image((void*)(intptr_t)m_texture.texture, ImVec2(100.f, 100.f));
	}
}

Value Output::createOutput()
{
	Value inputTextureValue = getInputValue(0);

	if (inputTextureValue.getType() == ValueType::FLOAT3) {
		m_color = inputTextureValue.asColor3();

		bindFramebuffer(m_colorTexture);

		glClearColor(m_color.r, m_color.g, m_color.b, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		resetFrameBuffer();
	}

	if (inputTextureValue.getType() == ValueType::TEXTURE) {
		m_texture = inputTextureValue.asTexture();
	}

	return Value();
}
