#include "Window.h"

bool Window::Init(const char * title, const uint width, const uint height)
{
	mTitle = title;
	mWidth = width;
	mHeight = height;

	mHandle = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (!mHandle)
		return false;

	return true;
}

void Window::Destroy()
{
	if(mHandle)
		glfwDestroyWindow(mHandle);
}
