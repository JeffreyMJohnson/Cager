#include "Cager.h"

const char* Cager::VERTEX_SHADER_PATH = "Vert.glsl";
const char* Cager::FRAGMENT_SHADER_PATH = "Frag.glsl";
//Window Cager::sWindow;
//Shader Cager::sShader;
uint Cager::shader = 0;
uint Cager::mainWindow = 0;

bool Cager::Init(const int width, const int height, const char * title, const vec4 clearColor)
{


	//bool success = AssMan::Init();
	//init glfw
	bool success = glfwInit();
	if (!success)
		return success;

	//init window
	//success = sWindow.Init(title, width, height);
	;
	if (!(mainWindow = AssMan::CreateContext(width, height, title)))
	{
		Destroy();
		return false;
	}

	//set context
	glfwMakeContextCurrent(AssMan::GetContextHandle(mainWindow));

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
	if (AssMan::ContextShouldClose(mainWindow) || Keyboard::IsKeyPressed(Keyboard::KEY_ESCAPE))
		return false;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSwapBuffers(AssMan::GetContextHandle(mainWindow));
	glfwPollEvents();
	return true;
}

void Cager::Destroy()
{
	AssMan::Kill();
}
