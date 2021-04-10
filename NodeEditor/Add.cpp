#include "Add.h"
#include "Editor.h"

Add::Add()
	: Node("Add")
{
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


	return Value();
}
