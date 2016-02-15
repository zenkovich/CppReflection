#include "Reflection.h"

Reflection Reflection::instance;

Reflection::Reflection()
{}

Reflection::~Reflection()
{
	for (auto type : mTypes)
		delete type;
}

const std::vector<Type*>& Reflection::GetTypes()
{
	return instance.mTypes;
}

IObject* Reflection::CreateTypeSample(const std::string& typeName)
{
	for (auto type : instance.mTypes)
	{
		if (type->Name() == typeName)
			return type->CreateSample();
	}

	return nullptr;
}

Type* Reflection::GetType(Type::Id id)
{
	for (auto type : instance.mTypes)
	{
		if (type->ID() == id)
			return type;
	}

	return nullptr;
}