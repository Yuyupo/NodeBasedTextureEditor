#include "ConstantFloat3.h"
#include "imgui.h"

ConstantFloat3::ConstantFloat3()
	: Node("Constant Float3", AttributeType::CONST_FLOAT3)
	, m_xyz{0.f, 0.f, 0.f}
{
}

void ConstantFloat3::createContent()
{
	ImGui::PushItemWidth(100.f);
	ImGui::InputFloat3("Float3", m_xyz);
}

Value ConstantFloat3::createOutput()
{
	return Value(Float3(m_xyz[0], m_xyz[1], m_xyz[2]));
}
