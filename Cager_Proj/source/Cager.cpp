#include "Cager.h"

const char* Cager::VERTEX_SHADER_PATH = "Vert.glsl";
const char* Cager::FRAGMENT_SHADER_PATH = "Frag.glsl";
//Window Cager::sWindow;
//Shader Cager::sShader;
uint Cager::shader = 0;
uint Cager::window = 0;

bool Cager::Init(const int width, const int height, const char * title, const vec4 clearColor)
{
	bool success = true;
	//init glfw
	success = glfwInit();
	if (!success)
		return success;

	//init window
	//success = sWindow.Init(title, width, height);
	
	if (!success)
	{
		Destroy();
		return success;
	}

	//set context
	//glfwMakeContextCurrent(sWindow.mHandle);

	//load extensions
	success = ogl_LoadFunctions() != ogl_LOAD_FAILED;
	if (!success)
	{
		Destroy();
		return success;
	}

	//Init shader
	//sShader.LoadShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
	if (!(shader = AssMan::CreateShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH)))
	{
		return false;
	}

	//set clear color
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);


	//init keyboard input
	Keyboard::Init();

	return success;
}

bool Cager::Update()
{
	if (glfwWindowShouldClose(sWindow.mHandle) || Keyboard::IsKeyPressed(Keyboard::KEY_ESCAPE))
		return false;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glfwSwapBuffers(sWindow.mHandle);
	glfwPollEvents();
	return true;
}

void Cager::Destroy()
{
	if (sWindow.mHandle)
	{
		sWindow.Destroy();
	}
	glfwTerminate();
}
