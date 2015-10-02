#pragma once
#include "GameApp.h"
#include "GLFW/glfw3.h"

class TestApp : public GameApp
{
public:
	const int WINDOW_WIDTH = 1280;
	const int WINDOW_HEIGHT = 720;
	const char* WINDOW_TITLE = "Cager Test App";

	bool StartUp();
	void ShutDown();
	bool Update();
	void Draw();

};