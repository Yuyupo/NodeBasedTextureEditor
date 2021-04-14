#include "Multiply.h"
#include "Editor.h"
#include "imgui.h"

Multiply::Multiply()
		: Node("Multiply")
		, m_texture{ 256.f, 256.f, 0 }
{
	// TODO size differences
	generateTexture(m_texture);

	addInput("A");
	addInput("B");
}

void Multiply::createContent()
{
}

Value Multiply::createOutput()
{
	Value InputValueA = getInputValue(0);
	Value InputValueB = getInputValue(1);

	if (InputValueA.getType() == ValueType::INT &&
		InputValueB.getType() == ValueType::INT)
	{
		return Value(InputValueA.asInt() * InputValueB.asInt());
	}

	return Value();
}

Value Multiply::handleVectors(Float3 float3, Value value)
{
	return Value();
}
