#include "Cager.h"

Window Cager::sWindow;

bool Cager::Init(const int width, const int height, const char * title, const vec4 clearColor)
{
	bool success = true;
	//init glfw
	success = glfwInit();
	if (!success)
		return success;

	//init window
	success = sWindow.Init(title, width, height);
	if (!success)
	{
		Destroy();
		return success;
	}

	//set context
	glfwMakeContextCurrent(sWindow.mHandle);

	//load extensions
	success = ogl_LoadFunctions() != ogl_LOAD_FAILED;
	if (!success)
	{
		Destroy();
		return success;
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
