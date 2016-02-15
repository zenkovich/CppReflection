#pragma once

#include <string>
#include <vector>

class Type;

// --------------------
// Function information
// --------------------
class FunctionInfo
{
public:
	struct Parameter
	{
		std::string name;
		Type*       type;
		bool        isConstant;
		bool        isReference;
		bool        isPointer;

		bool operator==(const Parameter& other) const;
	};
	typedef std::vector<Parameter> ParametersVec;

public:
	// Returns copy of this
	virtual FunctionInfo* Clone() const;

	// Check Equals operator
	bool operator==(const FunctionInfo& other) const;

	// Returns owner type
	Type* GetOwnerType() const;

	// Returns function's name
	const std::string& GetName() const;

	// Returns return value type
	Type* GetReturnType() const;

	// Returns is function constant
	bool IsConstant() const;

	// Returns function's parameters
	const ParametersVec& GetParameters() const;

	// Invokes function with parameters
	template<typename _res_type, typename ... _args>
	_res_type Invoke(void* object, _args ... args) const;

protected:
	Type*         mOwnerType;
	std::string   mName;
	Type*         mReturnType;
	bool          mIsContant;
	ParametersVec mParameters;

	friend class Type;
	friend class TypeInitializer;
};

// ----------------------------------
// Function info specialization layer
// ----------------------------------
template<typename _res_type, typename ... _args>
class ISpecFunctionInfo: public FunctionInfo
{
public:
	// Returns copy of this
	virtual FunctionInfo* Clone() const = 0;

	// Invokes function with parameters
	virtual _res_type Invoke(void* object, _args ... args) const = 0;
};

// -------------------------------
// Function specialized class info
// -------------------------------
template<typename _class_type, typename _res_type, typename ... _args>
class SpecFunctionInfo: public ISpecFunctionInfo<_res_type, _args ...>
{
public:
	// Returns copy of this
	virtual FunctionInfo* Clone() const
	{
		return new SpecFunctionInfo(*this);
	}

	// Invokes function with parameters
	_res_type Invoke(void* object, _args ... args) const
	{
		_class_type* classObject = (_class_type*)object;
		return (classObject->*mFunctionPtr)(args ...);
	}

protected:
	_res_type(_class_type::*mFunctionPtr)(_args ... args);

	friend class TypeInitializer;
};

// ----------------------------------------
// Constant function specialized class info
// ----------------------------------------
template<typename _class_type, typename _res_type, typename ... _args>
class SpecConstFunctionInfo: public ISpecFunctionInfo<_res_type, _args ...>
{
public:
	// Returns copy of this
	virtual FunctionInfo* Clone() const
	{
		return new SpecConstFunctionInfo(*this);
	}

	// Invokes function with parameters
	_res_type Invoke(void* object, _args ... args) const
	{
		_class_type* classObject = (_class_type*)object;
		return (classObject->*mFunctionPtr)(args ...);
	}

protected:
	_res_type(_class_type::*mFunctionPtr)(_args ... args) const;

	friend class TypeInitializer;
};

template<typename _res_type, typename ... _args>
_res_type FunctionInfo::Invoke(void* object, _args ... args) const
{
	const ISpecFunctionInfo<_res_type, _args ...>* thisSpec = (const ISpecFunctionInfo<_res_type, _args ...>*)this;
	return thisSpec->Invoke(object, args ...);
}