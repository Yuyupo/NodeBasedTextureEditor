#include "ConstantInt.h"
#include "imgui.h"

ConstantInt::ConstantInt()
	: Node("Constant Int", AttributeType::CONST_INT)
	, m_int(0)
{
}

void ConstantInt::createContent()
{
	ImGui::PushItemWidth(50.f);
	ImGui::InputInt("Value", &m_int);
}

Value ConstantInt::createOutput()
{
	return Value(m_int);
}
