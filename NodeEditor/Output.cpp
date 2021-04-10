#include "Output.h"
#include "Editor.h"
#include "imgui.h"

Output::Output()
	: Node("Output")
	, m_color{ 0.f, 0.f, 0.f }
	, m_texture { 50.f, 50.f, 0}
	, m_colorTexture {128.f, 128.f, 0}
	, m_fb (Editor::getRenderingFrameBuffer())
{
	// m_texture comes from outside
	generateTexture(m_colorTexture);

	addInput("Color");
	addInput("Texture");
}

Output::~Output()
{
}

void Output::createContent()
{
	createOutput();

	ImGui::Text("Color");
	ImGui::Image((void*)(intptr_t)m_colorTexture.texture, ImVec2(100.f, 100.f));

	ImGui::Text("\nTexture");
	ImGui::Image((void*)(intptr_t)m_texture.texture, ImVec2(100.f, 100.f));
}

Value Output::createOutput()
{
	Value inputColorValue = getInputValue(0);
	Value inputTextureValue = getInputValue(1);

	if (inputColorValue.getType() == ValueType::FLOAT3) {
		m_color = inputColorValue.asColor3();

		// Init
		glBindFramebuffer(GL_FRAMEBUFFER, m_fb);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture.texture, 0);
		glViewport(0, 0, m_colorTexture.width, m_colorTexture.height);

		// Work
		glClearColor(m_color.r, m_color.g, m_color.b, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Reset 
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	if (inputTextureValue.getType() == ValueType::TEXTURE) {
		m_texture = inputTextureValue.asTexture();
	}

	//TODO zero out the texture when link is broken

	return Value();
}
