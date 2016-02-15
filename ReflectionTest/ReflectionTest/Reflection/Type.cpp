#include "Type.h"

#include "Reflection.h"

Type::Type():
	mId(0)
{}

Type::Type(const Type& other):
	mId(other.mId), mName(other.mName), mBaseTypes(other.mBaseTypes)
{
	for (auto field : other.mFields)
	{
		auto fieldCopy = field->Clone();
		mFields.push_back(fieldCopy);
	}

	for (auto func : other.mFunctions)
	{
		auto funcCopy = func->Clone();
		funcCopy->mOwnerType = this;
		mFunctions.push_back(funcCopy);
	}

	mTypeCreator = other.mTypeCreator->Clone();
}

Type::~Type()
{
	for (auto field : mFields)
		delete field;

	for (auto func : mFunctions)
		delete func;

	delete mTypeCreator;
}

Type& Type::operator=(const Type& other)
{
	for (auto field : mFields)
		delete field;

	for (auto func : mFunctions)
		delete func;

	delete mTypeCreator;

	mFields.clear();
	mFunctions.clear();
	mId = other.mId;
	mName = other.mName;
	mBaseTypes = other.mBaseTypes;

	for (auto field : other.mFields)
	{
		auto fieldCopy = field->Clone();
		mFields.push_back(fieldCopy);
	}

	for (auto func : other.mFunctions)
	{
		auto funcCopy = func->Clone();
		funcCopy->mOwnerType = this;
		mFunctions.push_back(funcCopy);
	}

	mTypeCreator = other.mTypeCreator->Clone();

	return *this;
}

const std::string& Type::Name() const
{
	return mName;
}

Type::Id Type::ID() const
{
	return mId;
}

const Type::TypesVec& Type::BaseTypes() const
{
	return mBaseTypes;
}

const Type::FieldInfosVec& Type::Fields() const
{
	return mFields;
}

const Type::FunctionsInfosVec& Type::Functions() const
{
	return mFunctions;
}

const FieldInfo* Type::Field(const std::string& name) const
{
	for (auto field : mFields)
		if (field->Name() == name)
			return field;

	return nullptr;
}

const FunctionInfo* Type::GetFunction(const std::string& name) const
{
	for (auto func : mFunctions)
		if (func->mName == name)
			return func;

	return nullptr;
}

Type::TypesVec Type::DerivedTypes() const
{
	TypesVec res;
	for (auto type : Reflection::GetTypes())
	{
		auto baseTypes = type->BaseTypes();
		for (auto btype : baseTypes)
		{
			if (btype->mId == mId)
				res.push_back(type);
		}
	}
	return res;
}

IObject* Type::CreateSample() const
{
	return mTypeCreator->Create();
}

std::string Type::GetFieldPath(void* sourceObject, void *targetObject) const
{
	if (sourceObject == targetObject)
		return "";

	std::string res;

	for (auto field : mFields)
	{
		char* fieldObject = field->GetValuePtr<char>(sourceObject);

		if (fieldObject == targetObject)
			return field->mName;

		if (field->SearchFieldPath(fieldObject, targetObject, field->mName, res))
			return res;
	}

	return res;
}

FieldInfo* Type::FindFieldInfo(void* sourceObject, void *targetObject, std::string &res) const
{
	for (auto field : mFields)
	{
		char* fieldObject = field->GetValuePtr<char>(sourceObject);

		if (fieldObject == targetObject)
		{
			res = field->mName;
			return field;
		}

		if (!fieldObject)
			continue;

		FieldInfo* childField = field->SearchFieldPath(fieldObject, targetObject, field->mName, res);
		if (childField)
			return childField;
	}

	return nullptr;
}

bool Type::operator!=(const Type& other) const
{
	return other.mId != mId;
}

bool Type::operator==(const Type& other) const
{
	return other.mId == mId;
}

void TypeInitializer::AddBaseType(Type* type, Type* baseType)
{
	type->mBaseTypes.push_back(baseType);

	for (auto field : baseType->mFields)
		type->mFields.insert(type->mFields.begin(), field->Clone());

	for (auto func : baseType->mFunctions)
		type->mFunctions.insert(type->mFunctions.begin(), func->Clone());
}

Type Type::Dummy::type;