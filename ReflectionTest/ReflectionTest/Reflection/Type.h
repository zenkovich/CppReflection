#pragma once

#include <vector>

#include "FieldInfo.h"
#include "FunctionInfo.h"

class IObject;

// -----------------------
// Object type information
// -----------------------
class Type
{
public:
	typedef unsigned int Id;
	typedef std::vector<FieldInfo*> FieldInfosVec;
	typedef std::vector<FunctionInfo*> FunctionsInfosVec;
	typedef std::vector<Type*> TypesVec;

	struct Dummy
	{
		static Type type;
	};

	struct ITypeCreator
	{
		virtual ITypeCreator* Clone() const = 0;
		virtual IObject* Create() const = 0;
	};

	template<typename _type>
	struct TypeCreator: public ITypeCreator
	{
		ITypeCreator* Clone() const { return new TypeCreator<_type>(); }
		IObject* Create() const { return new _type(); }
	};

public:
	// Default constructor
	Type();

	// Copy constructor
	Type(const Type& other);

	// Destructor
	~Type();

	// Check equals operator
	bool operator==(const Type& other) const;

	// Check not equals operator
	bool operator!=(const Type& other) const;

	// Copy operator
	Type& operator=(const Type& other);

	// Returns name of type
	const std::string& Name() const;

	// Returns id of type
	Id ID() const;

	// Creates sample copy and returns him
	IObject* CreateSample() const;

	// Returns inherited types
	TypesVec DerivedTypes() const;

	// Returns vector of base types
	const TypesVec& BaseTypes() const;

	// Returns fields informations array
	const FieldInfosVec& Fields() const;

	// Returns functions informations array
	const FunctionsInfosVec& Functions() const;

	// Returns field information by name
	const FieldInfo* Field(const std::string& name) const;

	// Returns filed pointer by path
	template<typename _type>
	_type* GetFieldPtr(void* object, const std::string& path, FieldInfo*& fieldInfo) const;

	// Returns field path by pointer from source object
	std::string GetFieldPath(void* sourceObject, void *targetObject) const;

	// Returns field info by pointer from source object
	FieldInfo* FindFieldInfo(void* sourceObject, void *targetObject, std::string &res) const;

	// Returns function info by name
	const FunctionInfo* GetFunction(const std::string& name) const;

	// Invokes function with name
	template<typename _res_type, typename ... _args>
	_res_type Invoke(const std::string& name, void* object, _args ... args) const;

	// Adds basic type
	void AddBaseType(Type* baseType);

protected:
	std::string       mName;        // Name of object type
	FieldInfosVec     mFields;      // Fields information
	FunctionsInfosVec mFunctions;   // Functions informations
	Id                mId;          // Id of type
	TypesVec          mBaseTypes;   // Base types ids
	ITypeCreator*     mTypeCreator; // Type creator

	friend class FieldInfo;
	friend class FunctionInfo;
	friend class Reflection;
	friend class TypeInitializer;
};

class TypeInitializer
{
public:
	// Adds basic type
	static void AddBaseType(Type* type, Type* baseType);

	// Registers field in type
	template<typename _type>
	static FieldInfo& RegField(Type* type, const std::string& name, size_t offset, _type*& value);

	// Registers field in type
	template<typename _type>
	static FieldInfo& RegField(Type* type, const std::string& name, size_t offset, _type& value);

	// Registers function in type
	template<typename _class_type, typename _res_type, typename ... _args>
	static FunctionInfo* RegFunction(Type* type, const std::string& name, _res_type(_class_type::*pointer)(_args ...));

	// Registers function in type
	template<typename _class_type, typename _res_type, typename ... _args>
	static FunctionInfo* RegFunction(Type* type, const std::string& name, _res_type(_class_type::*pointer)(_args ...) const);

	// Registers parameter in function info
	template<typename _type>
	static FunctionInfo* RegFuncParam(FunctionInfo* info, const std::string& name);
};

template<typename _type>
FieldInfo& TypeInitializer::RegField(Type* type, const std::string& name, size_t offset, _type*& value)
{
	Type* valType = &std::conditional<std::is_base_of<IObject, _type>::value, _type, Type::Dummy>::type::type;
	type->mFields.push_back(new FieldInfo(name, offset, false, true, valType));
	return *type->mFields[type->mFields.size() - 1];
}

template<typename _type>
FieldInfo& TypeInitializer::RegField(Type* type, const std::string& name, size_t offset, _type& value)
{
	Type* valType = &std::conditional<std::is_base_of<IObject, _type>::value, _type, Type::Dummy>::type::type;
	type->mFields.push_back(new FieldInfo(name, offset, false, false, valType));
	return *type->mFields[type->mFields.size() - 1];
}

template<typename _class_type, typename _res_type, typename ... _args>
FunctionInfo* TypeInitializer::RegFunction(Type* type, const std::string& name, _res_type(_class_type::*pointer)(_args ...))
{
	Type* retType = &std::conditional<std::is_base_of<IObject, _res_type>::value, _res_type, Type::Dummy>::type::type;

	auto funcInfo = new SpecFunctionInfo<_class_type, _res_type, _args ...>();
	funcInfo->mName = name;
	funcInfo->mFunctionPtr = pointer;
	funcInfo->mReturnType = retType;
	funcInfo->mIsContant = false;
	type->mFunctions.push_back(funcInfo);

	return funcInfo;
}

template<typename _class_type, typename _res_type, typename ... _args>
FunctionInfo* TypeInitializer::RegFunction(Type* type, const std::string& name, _res_type(_class_type::*pointer)(_args ...) const)
{
	Type* retType = &std::conditional<std::is_base_of<IObject, _res_type>::value, _res_type, Type::Dummy>::type::type;

	auto funcInfo = new SpecConstFunctionInfo<_class_type, _res_type, _args ...>();
	funcInfo->mName = name;
	funcInfo->mFunctionPtr = pointer;
	funcInfo->mReturnType = retType;
	funcInfo->mIsContant = true;
	type->mFunctions.push_back(funcInfo);

	return funcInfo;
}

template<typename _type>
FunctionInfo* TypeInitializer::RegFuncParam(FunctionInfo* info, const std::string& name)
{
	Type* valType = &std::conditional<std::is_base_of<IObject, _type>::value, _type, Type::Dummy>::type::type;

	FunctionInfo::Parameter param;
	param.type = valType;
	param.name = name;
	param.isPointer = std::is_pointer<_type>::value;
	param.isConstant = std::is_const<_type>::value;
	param.isReference = std::is_reference<_type>::value;
	info->mParameters.push_back(param);

	return info;
}

template<typename _type>
_type* Type::GetFieldPtr(void* object, const std::string& path, FieldInfo*& fieldInfo) const
{
	int delPos = path.find("/");
	std::string pathPart = path.substr(0, delPos);

	for (auto field : mFields)
	{
		if (field->mName == pathPart)
		{
			if (delPos == -1)
			{
				fieldInfo = field;
				return field->GetValuePtr<_type>(object);
			}
			else
			{
				char* val = field->GetValuePtr<char>(object);

				if (!val)
					return nullptr;

				return (_type*)(field->SearchFieldPtr(val, path.substr(delPos + 1), fieldInfo));
			}
		}
	}

	return nullptr;
}

template<typename _res_type, typename ... _args>
_res_type Type::Invoke(const std::string& name, void* object, _args ... args) const
{
	auto func = GetFunction(name);
	if (func)
		return func->Invoke<_res_type, _args ...>(object, args ...);

	return _res_type();
}
