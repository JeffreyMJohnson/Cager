#pragma once
#ifndef GLM_FORCE_PURE
#define GLM_FORCE_PURE
#endif // !GLM_FORCE_PURE

#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

#include "Window.h"
#include "input\Keyboard.h"

using glm::vec3;
using glm::vec4;



class Cager
{
public:
	static bool Init(const int width, const int height, const char * title, const vec4 clearColor);
	static bool Update();
	static void Destroy();
private:
	static Window sWindow;
};