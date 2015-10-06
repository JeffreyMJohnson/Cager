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

enum UniformType
{
	MAT4,
	VEC4,
	VEC3,
	FLO1,
	INT1,
	UINT1
};

struct Shader
{
	uint handle = 0;
};

struct Context
{
	GLFWwindow* handle = nullptr;
	glm::vec4 color = glm::vec4(.25f, .25f, .25f, 1);
};

struct RenderObject
{
	uint vao = 0;
	uint vbo = 0;
	uint ibo = 0;
	uint indexCount = 0;
};

struct Vertex
{
	glm::vec4 position;
	glm::vec4 color;
	glm::vec4 normal;
	glm::vec4 tangent;
	glm::vec2 UV;
};

struct Geometry
{
	std::vector<Vertex> vertices;
	std::vector<uint> indices;
};

/*
Asset Manager class.
This abstracts all opengl calls and controls initializing, destruction and access of all assets that utilize the GPU and therefore
opengl such as context creation(window), rendering data (vertice, index), render targets, textures etc.

Init must be called first to initialize the gl environment.
Kill must be called to cleanup the environment.


One thing to note is this uses library ogl_4_4 and has requirement of a window being created and set to current context BEFORE the library can
be initialized. This requires the init for this oibrary to be in the contextcreate function which is not ideal.


*/
class AssMan
{
public:
	static bool Init();
	
	/*
	Call after all draws have been done and want to swap the back buffer of given context.
	*/
	static void Flip(uint context);

	/*
	Called once per frame. used to update timer, poll events etc.
	*/
	static void Update();

	static void Kill();
	//create
	//context window
	static uint CreateContext(const uint windowWidth, const uint windowHeight, const char* windowTitle);
	static void SetContextClearColor(uint context, glm::vec4 color);
	static glm::vec2 GetContextSize(uint windowID);
	static void SetCurrentContext(uint windowID);

	//render object
	static uint CreateRenderObject(Geometry& geometry);
	static void DrawRenderObject(uint objectID);

	//this is temp for refactor should be removed
	static GLFWwindow* GetContextHandle(uint windowID);

	static bool ContextShouldClose(uint windowID);

	/*
	Return unsigned int to be used as unique ID for access.
	NOTE: returns 0 if an error occurs.
	*/
	static const uint CreateShader(const char * vertexPath, const char * fragmentPath);

	/*
	Set the uniform in given shader to variable matching given name with given value.
	*/
	static void SetShaderUniform(uint shaderID, const char* name, const UniformType type, const void* value);

	/*
	returns the shader program handle of given shader id.
	*/
	static const uint GetShaderProgram(const uint shader);
	//Destroy

private:
	//static std::vector<void*> mAssets;
	static std::vector<Shader*> shaders;
	static std::vector<Context*> windows;
	static std::vector<RenderObject*> renderObjects;

	/*
	returns 0 if error.
	*/
	static uint LoadSubShader(uint shaderType, const char * path);
};
