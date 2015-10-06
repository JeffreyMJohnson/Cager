#include "AssMan.h"
//have null for first index because it's error number.
std::vector<Shader*> AssMan::shaders{ nullptr };
std::vector<Context*> AssMan::windows{ nullptr };
std::vector<RenderObject*> AssMan::renderObjects{ nullptr };


bool AssMan::Init()
{
	//init glfw
	bool success = glfwInit();
	if (!success)
	{
		std::cout << "Error initializing GLFW.\n";
		return success;
	}

	return success;
}

void AssMan::Flip(uint context)
{
	glfwSwapBuffers(windows[context]->handle);
	//clear the buffer after the swap or nothing will paint!
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

void AssMan::Update()
{
	glfwPollEvents();
}

void AssMan::Kill()
{
	//kill shaders list
	//skip the first cell, it's null
	for (int i = 1; i < shaders.size(); i++)
	{
		//free the shader program
		glDeleteShader(shaders[i]->handle);
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

	//kill renderobject list
	//skip the first cell, it's null
	for (int i = 1; i < renderObjects.size(); i++)
	{
		glDeleteBuffers(1, &renderObjects[i]->vbo);
		glDeleteBuffers(1, &renderObjects[i]->ibo);
		glDeleteVertexArrays(1, &renderObjects[i]->vao);
		//delete from heap
		delete renderObjects[i];
	}
	renderObjects.clear();

	glfwTerminate();

}

uint AssMan::CreateContext(const uint windowWidth, const uint windowHeight, const char * windowTitle)
{
	Context* window = new Context();
	window->handle = glfwCreateWindow(windowWidth, windowHeight, windowTitle, nullptr, nullptr);
	if (window->handle == nullptr)
	{
		std::cout << "Error creating context window\n";
		delete window;
		return 0;
	}

	//context must be set before callingf ogl_loadfunctions
	glfwMakeContextCurrent(window->handle);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		std::cout << "Error loading ogl.\n";
		delete window;
		return 0;
	}

	windows.push_back(window);

	//this should be refactored to proerties of render targets
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);

	return windows.size() - 1;
}

void AssMan::SetContextClearColor(uint context, glm::vec4 color)
{
	SetCurrentContext(context);
	glClearColor(color.r, color.g, color.b, color.a);

}

void AssMan::SetCurrentContext(uint windowID)
{
	glfwMakeContextCurrent(windows[windowID]->handle);
}

uint AssMan::CreateRenderObject(Geometry& geometry)
{
	RenderObject* rendObj = new RenderObject();
	//create / bind vao
	glGenVertexArrays(1, &rendObj->vao);
	glBindVertexArray(rendObj->vao);

	rendObj->indexCount = geometry.indices.size();

	//create,  bind and load vbo
	glGenBuffers(1, &rendObj->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, rendObj->vbo);
	glBufferData(GL_ARRAY_BUFFER, geometry.vertices.size() * sizeof(Vertex), geometry.vertices.data(), GL_STATIC_DRAW);

	//create, bind, fill ibo
	glGenBuffers(1, &rendObj->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendObj->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry.indices.size() * sizeof(uint), geometry.indices.data(), GL_STATIC_DRAW);

	//set up attribs
	glEnableVertexAttribArray(0);//position
	glEnableVertexAttribArray(1);//color in shader right now.
	glEnableVertexAttribArray(2);//normal
	glEnableVertexAttribArray(3);//tangent
	glEnableVertexAttribArray(4);//UV coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 4));

	//cleanup
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	renderObjects.push_back(rendObj);

	return renderObjects.size() - 1;
}

void AssMan::DrawRenderObject(uint objectID)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(renderObjects[objectID]->vao);
	glDrawElements(GL_TRIANGLES, renderObjects[objectID]->indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

GLFWwindow * AssMan::GetContextHandle(uint windowID)
{
	return windows[windowID]->handle;
}

bool AssMan::ContextShouldClose(uint windowID)
{
	return glfwWindowShouldClose(windows[windowID]->handle);
}


const uint AssMan::CreateShader(const char * vertexPath, const char * fragmentPath)
{
	using namespace std;

	uint vertex = LoadSubShader(GL_VERTEX_SHADER, vertexPath);
	if (!vertex)
	{
		return vertex;
	}

	uint fragment = LoadSubShader(GL_FRAGMENT_SHADER, fragmentPath);
	if (!fragment)
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
	s->handle = shaderProgram;
	shaders.push_back(s);

	//think this should be refactored out..
	glUseProgram(shaderProgram);
	return shaders.size() - 1;
}

void AssMan::SetShaderUniform(uint shaderID, const char * name, const UniformType type, const void * value)
{

	GLint location = glGetUniformLocation(shaders[shaderID]->handle, name);
	switch (type)
	{
	case MAT4:
		glUniformMatrix4fv(location, 1, false, (const GLfloat*)value);
		break;
	case VEC4:
		glUniform4fv(location, 1, (GLfloat*)value);
		break;
	case VEC3:
		glUniform3fv(location, 1, (GLfloat*)value);
		break;
	case FLO1:
		glUniform1f(location, *(GLfloat*)value);
		break;
	case INT1:
		glUniform1i(location, *(GLint*)value);
		break;
	case UINT1:
		glUniform1ui(location, *(GLuint*)value);
		break;
	}
}


const uint AssMan::GetShaderProgram(const uint shader)
{
	return shaders[shader]->handle;
}



uint AssMan::LoadSubShader(uint shaderType, const char * path)
{
	std::ifstream stream(path);
	std::string contents = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
	if (contents.size() == 0)
	{
		printf("Error loading shader file %s\nThere is nothing there.\n", path);
		return 0;
	}
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
