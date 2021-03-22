#pragma once

class Node;

class Editor
{
public:
	Editor(const char* glsl_version, GLFWwindow* window);
	~Editor();

	void drawNodes();
	void addNode(Node* node);
	void addLink(int attribute1, int attribute2);
	void handleEvents();
	void init(const char* glsl_version, GLFWwindow* window);

private:
	std::vector<Node*> m_nodes;
	std::vector<std::pair<int, int>> m_links;
};

