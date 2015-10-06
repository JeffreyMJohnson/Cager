#include "Cager.h"

const char* Cager::VERTEX_SHADER_PATH = "../Cager_Proj/source/shaders/Vert.glsl";
const char* Cager::FRAGMENT_SHADER_PATH = "../Cager_Proj/source/shaders/Frag.glsl";
uint Cager::shader = 0;
uint Cager::mainWindow = 0;
std::vector<GameObject*> Cager::gameObjects{ nullptr };
Camera* Cager::camera = new Camera();

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
	AssMan::SetContextClearColor(mainWindow, clearColor);

	//Init shader
	if (!(shader = AssMan::CreateShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH)))
	{
		Cager::Destroy();
		return false;
	}

	//init keyboard input
	Keyboard::Init();

	//init camera for now
	const float CAMERA_FOV = glm::pi<float>() * .25f;
	const float CAMERA_NEAR = .1f;
	const float CAMERA_FAR = 1000.0f;
	const vec3 CAMERA_FROM = vec3(10, 25, 30);
	const vec3 CAMERA_TO = vec3(10,0,0);
	const vec3 CAMERA_UP = vec3(0, 0, -1);
	camera->StartupPerspective(CAMERA_FOV, (float)width / height, CAMERA_NEAR, CAMERA_FAR);
	camera->SetView(CAMERA_FROM, CAMERA_TO, CAMERA_UP);
	//send uniform to shader now, will need to move to update for flycam.
	AssMan::SetShaderUniform(shader, "ProjectionView", MAT4, glm::value_ptr(camera->GetViewProjection()));
	//sShader->SetUniform("ProjectionView", Shader::MAT4, glm::value_ptr(sCamera->GetViewProjection()));

	return success;
}

bool Cager::Update()
{
	if (AssMan::ContextShouldClose(mainWindow) || Keyboard::IsKeyPressed(Keyboard::KEY_ESCAPE))
		return false;

	AssMan::Update();
	return true;
}

void Cager::DrawFrame()
{
	AssMan::Flip(mainWindow);
}

void Cager::Destroy()
{
	//cleanup gameobjects
	//skip the first index, it's null for error checking
	for (int i = 1; i < gameObjects.size(); i++)
	{
		delete gameObjects[i];
	}
	gameObjects.clear();
	//cleanup assman
	AssMan::Kill();
}

void Cager::DrawGameObject(const uint objectID)
{
	AssMan::DrawRenderObject(gameObjects[objectID]->renderObject);
}

uint Cager::CreateGrid(const int rows, const int cols)
{
	Geometry geometry;
	geometry.vertices.resize(rows*cols);

	for (uint r = 0; r < rows; r++)
	{
		for (uint c = 0; c < cols; c++)
		{
			geometry.vertices[r * cols + c].position = vec4((float)c, 0, (float)r, 1);
			geometry.vertices[r * cols + c].position = vec4((float)c, 0, (float)r, 1);

			vec3 color = vec3(sinf((c / (float)(cols - 1))*(r / (float)(rows - 1))));

			geometry.vertices[r*cols + c].color = vec4(color, 1);
		}
	}

	geometry.indices.resize((rows - 1) * (cols - 1) * 6);

	uint index = 0;
	for (uint r = 0; r < (rows - 1); r++)
	{
		for (uint c = 0; c < (cols - 1); c++)
		{
			//triangle 1
			geometry.indices[index++] = r*cols + c;
			geometry.indices[index++] = (r + 1)*cols + c;
			geometry.indices[index++] = (r + 1)*cols + (c + 1);

			//triangle 2
			geometry.indices[index++] = r*cols + c;
			geometry.indices[index++] = (r + 1)*cols + (c + 1);
			geometry.indices[index++] = r*cols + (c + 1);
		}
	}

	GameObject* gameObj = new GameObject();
	gameObj->renderObject = AssMan::CreateRenderObject(geometry);
	gameObjects.push_back(gameObj);
	return gameObjects.size() - 1;
}
