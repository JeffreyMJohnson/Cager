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



struct GameObject
{
	uint renderObject;
};


class Cager
{
public:
	/*
	This is the structure that any vertice data must conform to for the framework to be able to
	consume and use it. This is exposed to the user.
	*/
	

	static bool Init(const int width, const int height, const char * title, const vec4 clearColor);
	static bool Update();
	/*
	Call after drawing all objects in frame and want to render the frame.
	*/
	static void DrawFrame();
	static void Destroy();

	static void DrawGameObject(const uint objectID);

	/*
	Creates a 2D grid of given rows and cols, with center positioned at origin.
	returns unique ID to gameobject for reference
	*/
	static uint CreateGrid(const int rows, const int cols);


private:



	static const char* VERTEX_SHADER_PATH;
	static const char* FRAGMENT_SHADER_PATH;
	static uint shader;
	static uint mainWindow;
	static std::vector<GameObject*> gameObjects;
};