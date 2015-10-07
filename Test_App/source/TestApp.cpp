#include "TestApp.h"

typedef Cager glf;

using glm::vec4;

bool TestApp::StartUp()
{
	bool success = glf::Init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, glm::vec4(.75f, .75f, .75f, 1));
	
	grid = glf::CreateGrid(5,5);

	return success;
}

void TestApp::ShutDown()
{
	
}

bool TestApp::Update()
{
	
	return glf::Update();
}

void TestApp::Draw()
{
	glf::DrawGameObject(grid);
	glf::DrawFrame();
}