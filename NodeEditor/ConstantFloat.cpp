#include "ConstantFloat.h"
#include "imgui.h"

ConstantFloat::ConstantFloat()
	: Node("Constant Float", AttributeType::CONST_INT)
	, m_float(0.f)
{
}

void ConstantFloat::createContent()
{
	ImGui::InputFloat("Value", &m_float);
}

Value ConstantFloat::createOutput()
{
	return Value(m_float);
}
