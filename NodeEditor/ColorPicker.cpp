#include "ColorPicker.h"
#include "Value.h"
#include "imgui.h"

ColorPicker::ColorPicker() 
	: Node("Color Picker", AttributeType::COLOR3)
	, m_color{ 0.f, 0.f, 0.f }
{
}

void ColorPicker::createContent()
{
	ImGui::PushItemWidth(100.f);
	ImGui::ColorEdit3("Color", m_color);
}

Value ColorPicker::createOutput()
{
	return Value(Color3(m_color[0], m_color[1], m_color[2]));
}