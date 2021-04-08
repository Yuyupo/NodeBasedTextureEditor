#include "Output.h"
#include "Editor.h"
#include "imgui.h"

Output::Output()
	: Node("Output", AttributeType::NONE)
	, m_color{ 0.f, 0.f, 0.f }
	, m_texture { 50.f, 50.f, 0}
	, m_colorTexture {128.f, 128.f, 0}
{
	glGenTextures(1, &m_colorTexture.texture);
	glBindTexture(GL_TEXTURE_2D, m_colorTexture.texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	std::vector<GLubyte> emptyData(m_colorTexture.width * m_colorTexture.height * 4, 0);
	// set alpha to 1.f
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_colorTexture.width, m_colorTexture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, emptyData.data());

	glGenFramebuffers(1, &m_fb);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fb);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture.texture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	addInput("Color", AttributeType::COLOR3);
	addInput("Texture", AttributeType::TEXTURELOADER);
}

Output::~Output()
{
	// cleanup
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &m_fb);
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
	Node* InputColorNode = Editor::getInputNode(getInputs()[0]);
	Node* InputTextureNode = Editor::getInputNode(getInputs()[1]);

	if (InputColorNode != nullptr) {
		m_color = InputColorNode->createOutput().asColor3();

		// render to FBO
		glBindFramebuffer(GL_FRAMEBUFFER, m_fb);
		glViewport(0, 0, m_colorTexture.width, m_colorTexture.height);
		glClearColor(m_color.r, m_color.g, m_color.b, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	if (InputTextureNode != nullptr) {
		m_texture = InputTextureNode->createOutput().asTexture();
	}

	return Value();
}
