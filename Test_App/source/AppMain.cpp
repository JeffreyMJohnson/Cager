//include your derived app class header here
//#include "MyGameApp.h"
#include "TestApp.h"

void main()
{
	//instantiate derived app class on heap hotdog here
	//MyGameApp* app = new myGameApp();
	TestApp* app = new TestApp();

	if (app->StartUp())
	{
		while (app->Update())
		{
			app->Draw();
		}
		app->ShutDown();
	}
	delete app;
	return;
}