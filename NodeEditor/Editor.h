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
	static Attribute* createAttribute(std::string name);
	static void deleteAttribute(int id);
	static void handleEvents();
	static void init(const char* glsl_version, GLFWwindow* window);
	static void cleanUp();
	static Node* getInputNode(Attribute* attr);
	static void deleteNode(int id);
	static GLuint getRenderingFrameBuffer();
	static GLuint getPreviewFrameBuffer();
	static GLuint getRenderingVAO();
	static unsigned int createShaderProgram(const char* vertex_src, const char* fragment_src);
	static unsigned int getDefaultShaderProgram();

	template <typename T>
	static Node* createNode();

private:
	static GLuint m_renderingFrameBuffer;
	static GLuint m_previewFrameBuffer;
	static GLuint m_renderingVertexArrayObject;
	static GLuint m_renderingVertexBuffer;
	static unsigned int m_defaultShaderProgram;

	static std::vector<Attribute*> m_attributes;
	static std::vector<Node*> m_nodes;
	static std::vector<std::pair<int, int>> m_links;
	static bool active_menu;
};

