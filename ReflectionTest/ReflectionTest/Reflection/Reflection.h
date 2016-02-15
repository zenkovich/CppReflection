#pragma once

#include <vector>

#include "IObject.h"
#include "Type.h"

// ------------------------------
// Reflection in application container
// ------------------------------
class Reflection
{
public:
	static Reflection instance;

public:
	// Constructor. Initializes dummy type
	Reflection();

	// Destructor. Destroys all types
	~Reflection();

	// Returns array of all registered types
	static const std::vector<Type*>& GetTypes();

	// Returns a copy of type sample
	static IObject* CreateTypeSample(const std::string& typeName);

	// Returns type by type id
	static Type* GetType(Type::Id id);

	// Initializes type
	template<typename _type>
	static void InitializeType(const std::string& name);

protected:
	std::vector<Type*> mTypes;           // All registered types
	unsigned int       mLastGivenTypeId; // Last given type index
};

template<typename _type>
void Reflection::InitializeType(const std::string& name)
{
	_type* sample = nullptr;

	_type::InitializeType(sample);
	_type::type.mName = name;
	_type::type.mId = instance.mLastGivenTypeId++;
	_type::type.mTypeCreator = new Type::TypeCreator<_type>();

	instance.mTypes.push_back(&_type::type);
}