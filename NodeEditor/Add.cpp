#include "Add.h"
#include "Editor.h"
#include "imgui.h"

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

	if (InputValueA.getType() == ValueType::TEXTURE)
	{
		// Bind the source texture and copy
		return handleTexture(InputValueA.asTexture(), InputValueB);
	}

	if (InputValueB.getType() == ValueType::TEXTURE)
	{
		// Bind the source texture and copy
		return handleTexture(InputValueB.asTexture(), InputValueA);
	}

	if (InputValueA.getType() == ValueType::FLOAT3)
	{
		return handleVectors(InputValueA.asFloat3(), InputValueB);
	}

	if (InputValueB.getType() == ValueType::FLOAT3)
	{
		return handleVectors(InputValueB.asFloat3(), InputValueA);
	}

	return Value();
}

Value Add::handleTexture(Texture texture, Value value)
{
	bindFramebuffer(m_texture);

	if (value.getType() == ValueType::FLOAT3)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_ADD);

		Color3 color = value.asColor3();

		glClearColor(color.r, color.g, color.b, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
	}	

	if (value.getType() == ValueType::TEXTURE)
	{
		Texture InTexture = value.asTexture();
		drawTexture(InTexture);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_ADD);
	}

	drawTexture(texture);
	resetFrameBuffer();
	glDisable(GL_BLEND);
	return Value(Texture(m_texture));
}

Value Add::handleVectors(Float3 float3, Value value)
{
	Color3 color = { 0.f, 0.f, 0.f };
	if (value.getType() == ValueType::FLOAT3)
	{
		Color3 valueColor = value.asColor3();
		color.r = float3.x + valueColor.r;
		color.g = float3.y + valueColor.g;
		color.b = float3.z + valueColor.b;
	}

	if (value.getType() == ValueType::FLOAT)
	{
		float valueFloat = value.asFloat();
		color.r = float3.x + valueFloat;
		color.g = float3.y + valueFloat;
		color.b = float3.z + valueFloat;
	}

	return Value(Color3(color.r, color.g, color.b));
}
