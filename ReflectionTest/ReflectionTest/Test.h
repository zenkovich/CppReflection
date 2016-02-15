#pragma once

#include "Reflection/IObject.h"
#include "Serialization.h"

class TestA: public ISerializable
{
public:
	// @SERIALIZABLE
	int intParam = 3;

	// @SERIALIZABLE
	float floatParam = 4.5f;

	// @SERIALIZABLE
	float floatParam2 = 4.5f;

	// @SERIALIZABLE
	std::string stringParam = "string!";

	SERIALIZABLE(TestA);

	void Func(int a, float b, std::string c);

	virtual void FuncVirt(int a, float b, std::string c);
};

class TestB: public TestA
{
public:
	// @SERIALIZABLE
	float bParam = 3.3f;

	// @SERIALIZABLE
	float bParamATATA = 3.3f;

	// @SERIALIZABLE
	float bParam2 = 3.3f;

	TestA* pa = new TestA();

	SERIALIZABLE(TestB);

	void FuncVirt(int a, float b, std::string c);
};
