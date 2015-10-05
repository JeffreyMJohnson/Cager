#pragma once
#ifndef GLM_FORCE_PURE
#define GLM_FORCE_PURE
#endif
#include <vector>
#include <fstream>
#include <iostream>
#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"

typedef unsigned int uint;

enum Asset_Type
{
	SHADER
};

struct Shader
{
	uint programHandle = 0;
};

struct Window
{
	GLFWwindow* handle = nullptr;


	glm::vec2 GetHeight()
	{
		int width, height;
		glfwGetWindowSize(handle, &width, &height);
		return glm::vec2(width, height);
	}
};

class AssMan
{
public:
	static bool Init();
	static void Kill();
	//create
	static uint CreateContext(const uint windowWidth, const uint windowHeight, const char* windowTitle);
	static glm::vec2 GetContextSize(uint windowID);
	static void SetCurrentContext(uint windowID);

	//this is temkp for refactor should be rempoved
	static GLFWwindow* GetContextHandle(uint windowID);

	static bool ContextShouldClose(uint windowID);
	static const uint CreateShader(const char * vertexPath, const char * fragmentPath);
	static const uint GetShaderProgram(const uint shader);
	//Destroy

private:
	//static std::vector<void*> mAssets;
	static std::vector<Shader*> shaders;
	static std::vector<Window*> windows;

	/*
	returns 0 if error.
	*/
	static uint LoadSubShader(uint shaderType, const char * path);
};
