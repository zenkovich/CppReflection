#include "Test.h"

void TestA::Func(int a, float b, std::string c)
{
	intParam = a;
	floatParam = b;
	stringParam = c;

	printf("TestA::Func(%i, %f, %s)\n", a, b, c.c_str());
}

void TestA::FuncVirt(int a, float b, std::string c)
{
	intParam = a;
	floatParam = b;
	stringParam = c;

	printf("TestA::FuncVirt(%i, %f, %s)\n", a, b, c.c_str());
}

void TestB::FuncVirt(int a, float b, std::string c)
{
	intParam = a;
	floatParam = b;
	stringParam = c;
	bParam = b;

	printf("TestB::FuncVirt(%i, %f, %s)\n", a, b, c.c_str());
}
