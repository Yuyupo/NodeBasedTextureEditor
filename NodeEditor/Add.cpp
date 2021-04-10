#include "Add.h"
#include "Editor.h"

Add::Add()
	: Node("Add")
	, m_texture {256.f, 256.f, 0}
{
	// TODO size differences
	generateTexture(m_texture);

	addInput("A");
	addInput("B");
}

void Add::createContent()
{
}

Value Add::createOutput()
{
	Value InputValueA = getInputValue(0);
	Value InputValueB = getInputValue(1);

	if (InputValueA.getType() == ValueType::INT &&
		InputValueB.getType() == ValueType::INT)
	{
		return Value(InputValueA.asInt() + InputValueB.asInt());
	}

	// TODO add color
	if (InputValueA.getType() == ValueType::TEXTURE &&
		InputValueB.getType() == ValueType::FLOAT3)
	{
		Texture InTexture = InputValueA.asTexture();
		Color3 InColor = InputValueB.asColor3();

		// Init
		glBindFramebuffer(GL_FRAMEBUFFER, Editor::getRenderingFrameBuffer());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, InTexture.texture, 0);
		glViewport(0, 0, InTexture.width, InTexture.height);

		// Bind the source texture and copy
		glBindTexture(GL_TEXTURE_2D, m_texture.texture);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_texture.width, m_texture.height);
		GLenum err = glGetError();

		// Reset 
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return Value(Texture(m_texture));
	}

	return Value();
}
