#include "FieldInfo.h"

#include "Type.h"

FieldInfo::FieldInfo():
	mOffset(0), mIsProperty(false), mType(nullptr), mIsPtr(false)
{}

FieldInfo::FieldInfo(const std::string& name, unsigned int offset, bool isProperty, bool isPtr, Type* type):
	mName(name), mOffset(offset), mIsProperty(isProperty), mType(type), mIsPtr(isPtr)
{}

FieldInfo::FieldInfo(const FieldInfo& other) :
	mName(other.mName), mOffset(other.mOffset), mIsProperty(other.mIsProperty), mType(other.mType),
	mIsPtr(other.mIsPtr)
{
	for (auto attr : other.mAttributes)
	{
		IAttribute* newAttr = attr->Clone();
		newAttr->mOwnerFieldInfo = this;
		mAttributes.push_back(newAttr);
	}
}

FieldInfo::~FieldInfo()
{
	for (auto attr : mAttributes)
		delete attr;
}

FieldInfo& FieldInfo::operator=(const FieldInfo& other)
{
	for (auto attr : mAttributes)
		delete attr;

	mAttributes.clear();

	for (auto attr : other.mAttributes)
	{
		IAttribute* newAttr = attr->Clone();
		newAttr->mOwnerFieldInfo = this;
		mAttributes.push_back(newAttr);
	}

	mName = other.mName;
	mOffset = other.mOffset;
	mIsProperty = other.mIsProperty;
	mIsPtr = other.mIsPtr;
	mType = other.mType;

	return *this;
}

bool FieldInfo::operator==(const FieldInfo& other) const
{
	return mName == other.mName && mOffset == other.mOffset && mIsProperty == other.mIsProperty &&
		mAttributes == other.mAttributes;
}

FieldInfo* FieldInfo::Clone() const
{
	return new FieldInfo(*this);
}

const std::string& FieldInfo::Name() const
{
	return mName;
}

bool FieldInfo::IsProperty() const
{
	return mIsProperty;
}

const FieldInfo::AttributesVec& FieldInfo::Attributes() const
{
	return mAttributes;
}

FieldInfo* FieldInfo::SearchFieldPath(void* obj, void* target, const std::string& path, std::string& res)
{
	if (!mType)
		return false;

	for (auto field : mType->mFields)
	{
		char* fieldObj = field->GetValuePtr<char>(obj);
		if (fieldObj == target)
		{
			res = path + "/" + field->mName;
			return field;
		}

		FieldInfo* childField = field->SearchFieldPath(fieldObj, target, path + "/" + field->mName, res);
		if (childField)
			return childField;
	}

	return nullptr;
}

void* FieldInfo::SearchFieldPtr(void* obj, const std::string& path, FieldInfo*& fieldInfo)
{
	return mType->GetFieldPtr<char>(obj, path, fieldInfo);
}