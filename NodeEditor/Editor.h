#pragma once
#include <GLFW/glfw3.h>

class Node;

class Editor
{
public:
	Editor() = delete;
	static void drawNodes();
	static void addNode(Node* node);
	static void addLink(int attribute1, int attribute2);
	static Attribute* createAttribute(std::string name, AttributeType type);
	static void handleEvents();
	static void init(const char* glsl_version, GLFWwindow* window);
	static void cleanUp();

private:
	static std::vector<Attribute*> m_attributes;
	static std::vector<Node*> m_nodes;
	static std::vector<std::pair<int, int>> m_links;
};

