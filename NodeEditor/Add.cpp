#include "Add.h"

Add::Add()
	: Node("Add Float", AttributeType::CONST_FLOAT)
{
}

void Add::createContent()
{
}

Value Add::createOutput()
{
	return Value();
}
