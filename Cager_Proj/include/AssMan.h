#pragma once
#include <vector>
#include <fstream>
#include <iostream>
#include "gl_core_4_4\gl_core_4_4.h"
#include "GLFW\glfw3.h"

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
};

class AssMan
{
public:
	static void Init();
	static void Kill();
	//create
	static uint CreateContext(const uint windowWidth, const uint windowHeight, const char* windowTitle);
	static const uint CreateShader(const char * vertexPath, const char * fragmentPath);
	static const uint GetShaderProgram(const uint shader);
	//Destroy

private:
	//static std::vector<void*> mAssets;
	static std::vector<Shader*> shaders;
	static Window* window;

	/*
	returns 0 if error.
	*/
	static uint LoadSubShader(uint shaderType, const char * path);
};
