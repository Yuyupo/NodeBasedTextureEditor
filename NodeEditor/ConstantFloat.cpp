#include "ConstantFloat.h"
#include "imgui.h"

ConstantFloat::ConstantFloat()
	: Node("Constant Float")
	, m_float(0.f)
{
}

void ConstantFloat::createContent()
{
	ImGui::PushItemWidth(100.f);
	ImGui::InputFloat("Value", &m_float);
}

Value ConstantFloat::createOutput()
{
	return Value(m_float);
}
