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


	return success;
}

void Cager::Destroy()
{
	if (sWindow.mHandle)
	{
		sWindow.Destroy();
	}
	glfwTerminate();
}
