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
	if (InputValueA.getType() == ValueType::TEXTURE)
	{
		// Bind the source texture and copy
		return handleTexture(InputValueA.asTexture(), InputValueB);
	}

	if (InputValueA.getType() == ValueType::FLOAT3)
	{
		return handleVectors(InputValueA.asFloat3(), InputValueB);
	}

	return Value();
}

Value Add::handleTexture(Texture texture, Value value)
{
	bindFramebuffer(m_texture);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glBlendEquation(GL_FUNC_ADD);

	if (value.getType() == ValueType::FLOAT3)
	{
		Color3 color = value.asColor3();

		glClearColor(color.r, color.g, color.b, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	glBindTexture(GL_TEXTURE_2D, texture.texture);
	glBindVertexArray(Editor::getRenderingVAO());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);

	resetFrameBuffer();
	glDisable(GL_BLEND);
	return Value(Texture(m_texture));
}

Value Add::handleVectors(Float3 float3, Value value)
{

	return Value();
}
