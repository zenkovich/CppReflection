#pragma once

#include <vector>
#include <string>

#include "IAttribute.h"

class Type;

// -----------------------
// Class field information
// -----------------------
class FieldInfo
{
public:
	typedef std::vector<IAttribute*> AttributesVec;

public:
	// Default constructor
	FieldInfo();

	// Constructor
	FieldInfo(const std::string& name, unsigned int offset, bool isProperty, bool isPtr, Type* type);

	// Copy-constructor
	FieldInfo(const FieldInfo& other);

	// Destructor
	~FieldInfo();

	// Copy-operator
	FieldInfo& operator=(const FieldInfo& other);

	// Equal operator
	bool operator==(const FieldInfo& other) const;

	// Returns cloned copy
	virtual FieldInfo* Clone() const;

	// Adds attribute by type and parameters
	template<typename _attr_type, typename ... _args>
	FieldInfo& AddAttribute(_args ... args);

	// Returns name of field
	const std::string& Name() const;

	// Returns is it property or not
	bool IsProperty() const;

	// Returns value of field in specified object
	template<typename _type>
	_type GetValue(void* object) const;

	// Returns value pointer of field in specified object
	template<typename _type>
	_type* GetValuePtr(void* object) const;

	// Returns value pointer of field in specified object
	template<typename _type>
	_type* GetValuePtrStrong(void* object) const;

	// Sets value of field in specified object
	template<typename _type>
	void SetValue(void* object, _type value) const;

	// Returns attribute of field, if it is not exist - returns nullptr
	template<typename _attr_type>
	_attr_type* Attribute() const;

	// Returns true if exist attribute with specified type
	template<typename _attr_type>
	bool HaveAttribute() const;

	// Returns attributes array
	const AttributesVec& Attributes() const;

protected:
	std::string   mName;       // Name of field
	unsigned int  mOffset;     // Offset of field in bytes from owner address
	bool          mIsProperty; // Is it property or field
	bool          mIsPtr;      // Is property Ptr<>
	Type*         mType;       // Field type
	AttributesVec mAttributes; // Attributes array

protected:
	// Searches field recursively by pointer
	virtual FieldInfo* SearchFieldPath(void* obj, void* target, const std::string& path, std::string& res);

	// Searches field recursively by path
	virtual void* SearchFieldPtr(void* obj, const std::string& path, FieldInfo*& fieldInfo);

	friend class Type;

	template<typename _type>
	friend class AccessorFieldInfo;
};

template<typename _attr_type>
bool FieldInfo::HaveAttribute() const
{
	for (auto attr : mAttributes)
	{
		if (dynamic_cast<_attr_type*>(attr))
			return true;
	}

	return false;
}

template<typename _attr_type>
_attr_type* FieldInfo::Attribute() const
{
	for (auto attr : mAttributes)
	{
		_attr_type* res = dynamic_cast<_attr_type*>(attr);
		if (res)
			return res;
	}

	return nullptr;
}

template<typename _type>
void FieldInfo::SetValue(void* object, _type value) const
{
	*(_type*)((char*)object + mOffset) = value;
}

template<typename _type>
_type FieldInfo::GetValue(void* object) const
{
	return *(_type*)((char*)object + mOffset);
}

template<typename _type>
_type* FieldInfo::GetValuePtrStrong(void* object) const
{
	return (_type*)((char*)object + mOffset);
}

template<typename _type>
_type* FieldInfo::GetValuePtr(void* object) const
{
	if (mIsPtr) return *(_type**)((char*)object + mOffset);

	return (_type*)((char*)object + mOffset);
}

template<typename _attr_type, typename ... _args>
FieldInfo& FieldInfo::AddAttribute(_args ... args)
{
	_attr_type* attribute = new _attr_type(args ...);
	attribute->mOwnerFieldInfo = this;
	mAttributes.push_back(attribute);
	return *this;
}


