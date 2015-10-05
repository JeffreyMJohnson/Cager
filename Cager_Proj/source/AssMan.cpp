#include "AssMan.h"
//have null for first index because it's error number.
std::vector<Shader*> AssMan::shaders{ nullptr };
std::vector<Window*> AssMan::windows{ nullptr };


bool AssMan::Init()
{
	return true;
}

void AssMan::Kill()
{
	//kill shaders list
	//skip the first cell, it's null
	for (int i = 1; i < shaders.size(); i++)
	{
		//free the shader program
		glDeleteShader(shaders[i]->programHandle);
		//delete from heap
		delete shaders[i];
	}
	shaders.clear();

	//kill windows list
	//skip the first cell, it's null
	for (int i = 1; i < windows.size(); i++)
	{
		glfwDestroyWindow(windows[i]->handle);
		//delete from heap
		delete windows[i];
	}
	windows.clear();

	glfwTerminate();
}

uint AssMan::CreateContext(const uint windowWidth, const uint windowHeight, const char * windowTitle)
{
	Window* window = new Window();
	window->handle = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr);
	if (window->handle == nullptr)
	{
		std::cout << "Error creating context window\n";
		delete window;
		return 0;
	}
	windows.push_back(window);
	return windows.size() - 1;
}

GLFWwindow * AssMan::GetContextHandle(uint windowID)
{
	return windows[windowID]->handle;
}

bool AssMan::ContextShouldClose(uint windowID)
{
	return glfwWindowShouldClose(windows[windowID]->handle);
}

/*
return of 0 is error.
*/
const uint AssMan::CreateShader(const char * vertexPath, const char * fragmentPath)
{
	using namespace std;

	uint vertex = LoadSubShader(GL_VERTEX_SHADER, vertexPath);
	if (vertex)
	{
		return vertex;
	}

	uint fragment = LoadSubShader(GL_FRAGMENT_SHADER, fragmentPath);
	if (fragment)
	{
		return fragment;
	}


	int success = GL_FALSE;
	uint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertex);
	glAttachShader(shaderProgram, fragment);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetProgramInfoLog(shaderProgram, length, 0, log);
		std::cout << "Error linking shader program.\n" << log << std::endl;
		delete[] log;
		return success;
	}
	Shader* s = new Shader();
	s->programHandle = shaderProgram;
	shaders.push_back(s);

	//think this should be refactored out..
	glUseProgram(shaderProgram);
	return shaders.size() - 1;
}

/*
returns the shader program handle of given shader id.
*/
const uint AssMan::GetShaderProgram(const uint shader)
{
	return shaders[shader]->programHandle;
}



uint AssMan::LoadSubShader(uint shaderType, const char * path)
{
	std::ifstream stream(path);
	std::string contents = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
	char* code = new char[contents.length() + 1];
	strncpy_s(code, contents.length() + 1, contents.c_str(), contents.length());

	uint shader = glCreateShader(shaderType);
	if (!shader)
	{
		std::cout << "Error creating the shader.\n";
		return shader;
	}

	glShaderSource(shader, 1, &code, 0);
	delete[] code;


	GLint success = GL_FALSE;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetShaderInfoLog(shader, length, 0, log);
		std::cout << "Error compiling shader.\n" << log << std::endl;
		delete[] log;
	}

	

	if (!success)
	{
		return 0;
	}
	else
	{
		return shader;
	}

}
