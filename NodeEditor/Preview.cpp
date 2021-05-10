#include "Preview.h"
#include "Editor.h"
#include "imgui.h"

Preview::Preview()
	: Node("Preview")
	, m_color{ 0.f, 0.f, 0.f }
	, m_texture{ 256.f, 256.f, 0 }
	, m_colorTexture{ 256.f, 256.f, 0 }
{
	generateTexture(m_colorTexture);
	generateTexture(m_texture);

	addInput("");
}

Preview::~Preview()
{
}

void Preview::createContent()
{
	createOutput();
	if (getInputValue(0).getType() == ValueType::FLOAT3)
	{
		ImGui::Image((void*)(intptr_t)m_colorTexture.texture, ImVec2(100.f, 100.f));
	}
	else if (getInputValue(0).getType() == ValueType::TEXTURE)
	{
		ImGui::Image((void*)(intptr_t)m_texture.texture, ImVec2(100.f, 100.f));
	}
}

Value Preview::createOutput()
{
	Value inputValue = getInputValue(0);

	if (inputValue.getType() == ValueType::FLOAT3) {
		m_color = inputValue.asColor3();
		bindPreviewBuffer(m_colorTexture);

		glClearColor(m_color.r, m_color.g, m_color.b, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		resetFrameBuffer();
	}

	if (inputValue.getType() == ValueType::TEXTURE) {
		m_texture = inputValue.asTexture();
	}

	return Value(inputValue);
}
