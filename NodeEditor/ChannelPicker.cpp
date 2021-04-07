#include "ChannelPicker.h"
#include "Editor.h"
#include "imgui.h"

ChannelPicker::ChannelPicker() 
	: Node("Color Channel Picker", AttributeType::COLOR3)
	, m_color{ 0.f, 0.f, 0.f }
	, m_red(true)
	, m_green(true)
	, m_blue(true)
{
}

void ChannelPicker::createContent()
{
	ImGui::Checkbox("R", &m_red);
	ImGui::Checkbox("G", &m_green);
	ImGui::Checkbox("B", &m_blue);
}

Value ChannelPicker::createOutput()
{
	Node* InputNode = Editor::getInputNode(getInputs()[0]);

	if (InputNode == nullptr) {
		return Value(Color3(0.f, 0.f, 0.f));
	}

	m_color = InputNode->createOutput().asColor3();
	if (!m_red)
	{
		m_color.r = 0.f;
	}
	if (!m_green)
	{
		m_color.g = 0.f;
	}
	if (!m_blue)
	{
		m_color.b = 0.f;
	}
	return Value(Color3(m_color.r, m_color.g, m_color.b));
}
