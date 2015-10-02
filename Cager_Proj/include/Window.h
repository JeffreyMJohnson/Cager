#pragma once
#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"

typedef unsigned int uint;

class Window
{
public:
	GLFWwindow* mHandle = 0;
	uint mWidth = 0;
	uint mHeight = 0;
	const char* mTitle;
	glm::vec4 mClearColor;

	bool Init(const char* title, const uint width, const uint height);
	void Destroy();
};