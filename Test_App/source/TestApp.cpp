#include "TestApp.h"

typedef Cager glf;

bool TestApp::StartUp()
{
	bool success = glf::Init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, glm::vec4(.75f, .75f, 0, 1));
	return true;
}

void TestApp::ShutDown()
{

}

bool TestApp::Update()
{
	return false;
}

void TestApp::Draw()
{

}