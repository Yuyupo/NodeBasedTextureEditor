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
	bindFramebuffer(texture);
	glBindTexture(GL_TEXTURE_2D, m_texture.texture);
	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, m_texture.width, m_texture.height);

	if (value.getType() == ValueType::FLOAT3)
	{
		GLenum err;
		bindFramebuffer(m_texture);


		//glClear(GL_COLOR_BUFFER_BIT);
		//glDisable(GL_BLEND);
		resetFrameBuffer();

		return Value(Texture(m_texture));
	}

	resetFrameBuffer();

	return Value();
}

Value Add::handleVectors(Float3 float3, Value value)
{

	return Value();
}
