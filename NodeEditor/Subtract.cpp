#include "Subtract.h"

Subtract::Subtract()
		: Node("Subtract")
		, m_texture{ 256.f, 256.f, 0 }
{
	// TODO size differences
	generateTexture(m_texture);

	addInput("A");
	addInput("B");
}

void Subtract::createContent()
{
}

Value Subtract::createOutput()
{
	Value InputValueA = getInputValue(0);
	Value InputValueB = getInputValue(1);

	if (InputValueA.getType() == ValueType::INT &&
		InputValueB.getType() == ValueType::INT)
	{
		return Value(InputValueA.asInt() - InputValueB.asInt());
	}

	if (InputValueA.getType() == ValueType::TEXTURE)
	{
		// Bind the source texture and copy
		return handleTexture(InputValueA.asTexture(), InputValueB);
	}

	return Value();
}

Value Subtract::handleTexture(Texture texture, Value value)
{
	glColorMask(TRUE, TRUE, TRUE, FALSE);
	bindFramebuffer(m_texture);

	if (value.getType() == ValueType::FLOAT3)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_SUBTRACT);

		Color3 color = value.asColor3();

		glClearColor(color.r, color.g, color.b, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if (value.getType() == ValueType::TEXTURE)
	{
		Texture InTexture = value.asTexture();
		drawTexture(InTexture);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_SUBTRACT);
	}

	drawTexture(texture);
	resetFrameBuffer();
	glDisable(GL_BLEND);
	glColorMask(TRUE, TRUE, TRUE, TRUE);
	return Value(Texture(m_texture));
}

Value Subtract::handleVectors(Float3 float3, Value value)
{
	return Value();
}
