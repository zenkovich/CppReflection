#include "FunctionInfo.h"

bool FunctionInfo::operator==(const FunctionInfo& other) const
{
	return mName == other.mName && mIsContant == other.mIsContant &&
		mReturnType == other.mReturnType && mParameters == other.mParameters;
}

FunctionInfo* FunctionInfo::Clone() const
{
	return new FunctionInfo(*this);
}

Type* FunctionInfo::GetOwnerType() const
{
	return mOwnerType;
}

const std::string& FunctionInfo::GetName() const
{
	return mName;
}

Type* FunctionInfo::GetReturnType() const
{
	return mReturnType;
}

bool FunctionInfo::IsConstant() const
{
	return mIsContant;
}

const FunctionInfo::ParametersVec& FunctionInfo::GetParameters() const
{
	return mParameters;
}

bool FunctionInfo::Parameter::operator==(const Parameter& other) const
{
	return name == other.name && type == other.type && isConstant == other.isConstant && isPointer == other.isPointer &&
		isReference == other.isReference;
}