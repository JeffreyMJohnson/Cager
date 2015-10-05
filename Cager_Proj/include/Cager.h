#pragma once
#ifndef GLM_FORCE_PURE
#define GLM_FORCE_PURE
#endif // !GLM_FORCE_PURE

#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

//#include "Window.h"
#include "input\Keyboard.h"
//#include "Shader.h"
#include "AssMan.h"

using glm::vec3;
using glm::vec4;



class Cager
{
public:
	/*
	This is the structure that any vertice data must conform to for the framework to be able to
	consume and use it. This is exposed to the user.
	*/
	struct Vertex
	{
		glm::vec4 position;
		glm::vec4 color;
		glm::vec4 normal;
		glm::vec4 tangent;
		glm::vec2 UV;
	};

	static bool Init(const int width, const int height, const char * title, const vec4 clearColor);
	static bool Update();
	static void Destroy();
private:

	static const char* VERTEX_SHADER_PATH;
	static const char* FRAGMENT_SHADER_PATH;
	//static Window sWindow;
	//static Shader sShader;
	static uint shader;
	static uint window;
};