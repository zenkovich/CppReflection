#include "C:\work\ReflectionTest\ReflectionTest\ReflectionGenerated.h"

#include "Reflection/Reflection.h"

// Includes
#include "C:\work\ReflectionTest\ReflectionTest\Serialization.h"
#include "C:\work\ReflectionTest\ReflectionTest\Test.h"

// Types declarations
Type ISerializable::type;
Type TestA::type;
Type TestB::type;

// Types initializations
void ::ISerializable::InitializeType(::ISerializable* sample)
{
	auto funcInfo = TypeInitializer::RegFunction<::ISerializable, void, pugi::xml_node&>(&type, "Serialize", &::ISerializable::Serialize);
	TypeInitializer::RegFuncParam<pugi::xml_node&>(funcInfo, "node");
	funcInfo = TypeInitializer::RegFunction<::ISerializable, void, const pugi::xml_node&>(&type, "Deserialize", &::ISerializable::Deserialize);
	TypeInitializer::RegFuncParam<const pugi::xml_node&>(funcInfo, "node");
}

void ::TestA::InitializeType(::TestA* sample)
{
	TypeInitializer::RegField(&type, "intParam", (size_t)(char*)(&sample->intParam) - (size_t)(char*)sample, sample->intParam).AddAttribute<SerializableAttribute<decltype(intParam)>>();
	TypeInitializer::RegField(&type, "floatParam", (size_t)(char*)(&sample->floatParam) - (size_t)(char*)sample, sample->floatParam).AddAttribute<SerializableAttribute<decltype(floatParam)>>();
	TypeInitializer::RegField(&type, "floatParam2", (size_t)(char*)(&sample->floatParam2) - (size_t)(char*)sample, sample->floatParam2).AddAttribute<SerializableAttribute<decltype(floatParam2)>>();
	TypeInitializer::RegField(&type, "stringParam", (size_t)(char*)(&sample->stringParam) - (size_t)(char*)sample, sample->stringParam).AddAttribute<SerializableAttribute<decltype(stringParam)>>();
	auto funcInfo = TypeInitializer::RegFunction<::TestA, void, int, float, std::string>(&type, "Func", &::TestA::Func);
	TypeInitializer::RegFuncParam<int>(funcInfo, "a");
	TypeInitializer::RegFuncParam<float>(funcInfo, "b");
	TypeInitializer::RegFuncParam<std::string>(funcInfo, "c");
	funcInfo = TypeInitializer::RegFunction<::TestA, void, int, float, std::string>(&type, "FuncVirt", &::TestA::FuncVirt);
	TypeInitializer::RegFuncParam<int>(funcInfo, "a");
	TypeInitializer::RegFuncParam<float>(funcInfo, "b");
	TypeInitializer::RegFuncParam<std::string>(funcInfo, "c");
}

void ::TestB::InitializeType(::TestB* sample)
{
	TypeInitializer::RegField(&type, "bParam", (size_t)(char*)(&sample->bParam) - (size_t)(char*)sample, sample->bParam).AddAttribute<SerializableAttribute<decltype(bParam)>>();
	TypeInitializer::RegField(&type, "bParamATATA", (size_t)(char*)(&sample->bParamATATA) - (size_t)(char*)sample, sample->bParamATATA).AddAttribute<SerializableAttribute<decltype(bParamATATA)>>();
	TypeInitializer::RegField(&type, "bParam2", (size_t)(char*)(&sample->bParam2) - (size_t)(char*)sample, sample->bParam2).AddAttribute<SerializableAttribute<decltype(bParam2)>>();
	TypeInitializer::RegField(&type, "pa", (size_t)(char*)(&sample->pa) - (size_t)(char*)sample, sample->pa);
	auto funcInfo = TypeInitializer::RegFunction<::TestB, void, int, float, std::string>(&type, "FuncVirt", &::TestB::FuncVirt);
	TypeInitializer::RegFuncParam<int>(funcInfo, "a");
	TypeInitializer::RegFuncParam<float>(funcInfo, "b");
	TypeInitializer::RegFuncParam<std::string>(funcInfo, "c");
}


// Registering all types
void RegReflectionTypes()
{
	// Initialize types
	Reflection::InitializeType<::ISerializable>("::ISerializable");
	Reflection::InitializeType<::TestA>("::TestA");
	Reflection::InitializeType<::TestB>("::TestB");

	// Resolve inheritance
	TypeInitializer::AddBaseType(&::TestA::type, &::ISerializable::type);
	TypeInitializer::AddBaseType(&::TestB::type, &::TestA::type);

}