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

	return success;
}

bool Cager::Update()
{
	if (AssMan::ContextShouldClose(mainWindow) || Keyboard::IsKeyPressed(Keyboard::KEY_ESCAPE))
		return false;
	UpdateFlyCam();
	AssMan::SetShaderUniform(shader, "ProjectionView", MAT4, glm::value_ptr(camera->GetViewProjection()));
	if (Keyboard::IsKeyPressed(Keyboard::KEY_1))
	{
		gameObjects[1]->wireFrame = !gameObjects[1]->wireFrame;
	}
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
	AssMan::DrawRenderObject(gameObjects[objectID]->renderObject, gameObjects[objectID]->wireFrame);
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

uint Cager::CreateGameObjectFromFile(const char * path)
{
	/*
	bool success = true;
	//find extension
	std::string sPath(path);
	std::string ext = sPath.substr(sPath.find_last_of('.'));

	Geometry geometry;

	if (ext == ".obj")
	{
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string err = tinyobj::LoadObj(shapes, materials, path);
		if (err.length() != 0)
		{
			std::cout << "Error loading OBJ file:\n" << err << std::endl;
			success = false;
		}

		//hard coding only using first shape, can change to loop here
		if (success)
		{
			auto shape = shapes[0];
			auto mesh = shape.mesh;

			geometry.vertices.resize(mesh.positions.size());

			uint posIndex = 0;
			uint normalIndex = 0;
			uint UVIndex = 0;
			bool hasNormals = mesh.normals.size() == mesh.positions.size();
			bool hasUVs = mesh.texcoords.size() == mesh.positions.size();
			//obj has vectors of floats, my struct and shaders uses glm vecs so need to build myself
			for (uint vertexCount = 0; posIndex < mesh.positions.size(); vertexCount++)
			{
				float x = mesh.positions[posIndex++];
				float y = mesh.positions[posIndex++];
				float z = mesh.positions[posIndex++];
				geometry.vertices[vertexCount].position = vec4(x, y, z, 1);

				if (hasNormals)
				{
					x = mesh.normals[normalIndex++];
					y = mesh.normals[normalIndex++];
					z = mesh.normals[normalIndex++];
					geometry.vertices[vertexCount].normal = vec4(x, y, z, 1);
				}

				if (hasUVs)
				{
					x = mesh.texcoords[UVIndex++];
					y = mesh.texcoords[UVIndex++];
					geometry.vertices[vertexCount].UV = vec2(x, y);
				}
			}

			geometry.indices = mesh.indices;
		}
	}
	else if (ext == ".fbx")
	{


		FBXFile file;
		success = file.load(path, FBXFile::UNITS_METER, false, false, false);
		if (!success)
		{
			std::cout << "Error loading FBX file:\n";
		}
		else
		{
			//hardcoding to use single mesh, can loop here if needed.
			FBXMeshNode* mesh = file.getMeshByIndex(0);
			geometry.vertices.resize(mesh->m_vertices.size());

			for (int i = 0; i < mesh->m_vertices.size(); i++)
			{
				auto xVert = mesh->m_vertices[i];
				geometry.vertices[i].position = xVert.position;
				geometry.vertices[i].color = xVert.colour;
				geometry.vertices[i].normal = xVert.normal;
				geometry.vertices[i].UV = xVert.texCoord1;
			}

			geometry.indices = mesh->m_indices;

			file.unload();
		}

	}
	else
	{
		std::cout << "Unsupported format. Only support .obj or .fbx files.\n";
		success = false;
	}
	if (!success)
	{
		return false;
	}

	LoadModel(geometry);

	return true;
	*/


	return uint();
}

void Cager::UpdateFlyCam()
{
	if (Keyboard::IsKeyPressed(Keyboard::KEY_W) || Keyboard::IsKeyRepeat(Keyboard::KEY_W))
	{
		camera->Move(1);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_X) || Keyboard::IsKeyRepeat(Keyboard::KEY_X))
	{
		camera->Move(-1);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_A) || Keyboard::IsKeyRepeat(Keyboard::KEY_A))
	{
		camera->Slide(-1, 0);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_D) || Keyboard::IsKeyRepeat(Keyboard::KEY_D))
	{
		camera->Slide(1, 0);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_E) || Keyboard::IsKeyRepeat(Keyboard::KEY_E))
	{
		camera->Slide(0, 1);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_C) || Keyboard::IsKeyRepeat(Keyboard::KEY_C))
	{
		camera->Slide(0, -1);
	}
}
