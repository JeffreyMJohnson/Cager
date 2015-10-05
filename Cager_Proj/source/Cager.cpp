#include "Cager.h"

const char* Cager::VERTEX_SHADER_PATH = "Vert.glsl";
const char* Cager::FRAGMENT_SHADER_PATH = "Frag.glsl";
uint Cager::shader = 0;
uint Cager::mainWindow = 0;

bool Cager::Init(const int width, const int height, const char * title, const vec4 clearColor)
{

	//init assman
	bool success = AssMan::Init();
	
	//create main window
	if (!(mainWindow = AssMan::CreateContext(width, height, title)))
	{
		Cager::Destroy();
		return false;
	}

	//Init shader
	if (!(shader = AssMan::CreateShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH)))
	{
		Cager::Destroy();
		return false;
	}

	//init keyboard input
	Keyboard::Init();

	return success;
}

bool Cager::Update()
{
	if (AssMan::ContextShouldClose(mainWindow) || Keyboard::IsKeyPressed(Keyboard::KEY_ESCAPE))
		return false;
	//set clear color
	//glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSwapBuffers(AssMan::GetContextHandle(mainWindow));
	glfwPollEvents();
	return true;
}

void Cager::Destroy()
{
	AssMan::Kill();
}
