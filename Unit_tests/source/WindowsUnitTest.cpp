#include "CppUnitTest.h"
#include "Window.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unit_tests
{		
	TEST_CLASS(Window_Test)
	{
	public:
		
		TEST_METHOD(Init_Fail)
		{
			Window w;
			bool success = w.Init("test", 100, 100);
			Assert::IsFalse(success);
		}

		TEST_METHOD(Init_Pass)
		{
			Assert::IsTrue(glfwInit());
			Window w;
			Assert::IsTrue(w.Init("test", 100, 100));
			glfwTerminate();
		}

	};
}