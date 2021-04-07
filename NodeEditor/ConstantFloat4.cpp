#include "ConstantFloat4.h"
#include "imgui.h"

ConstantFloat4::ConstantFloat4()
	: Node("Constant Float4", AttributeType::CONST_FLOAT4)
	, m_xyzw{ 0.f, 0.f, 0.f, 0.f}
{
}

void ConstantFloat4::createContent()
{
	ImGui::PushItemWidth(150.f);
	ImGui::InputFloat4("Float4", m_xyzw);
}

Value ConstantFloat4::createOutput()
{
	return Value(Float4(m_xyzw[0], m_xyzw[1], m_xyzw[2], m_xyzw[3]));
}
