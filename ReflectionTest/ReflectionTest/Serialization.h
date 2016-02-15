#pragma once

#include "PugiXml/pugixml.hpp"
#include "Reflection/IAttribute.h"
#include "Reflection/IObject.h"

class ISerializableAttribute: public IAttribute
{
public:
	virtual void Serialize(void* object, pugi::xml_node& node) {}
	virtual void Deserialize(void* object, const pugi::xml_node& node) const {}
};

template<typename _type>
class SerializableAttribute: public ISerializableAttribute
{
public:
	void Serialize(void* object, pugi::xml_node& node);
	void Deserialize(void* object, const pugi::xml_node& node) const;

	IAttribute* Clone() const { return new SerializableAttribute(*this); }
};

class ISerializable: public IObject
{
public:
	IOBJECT(ISerializable);

	virtual void Serialize(pugi::xml_node& node) {}
	virtual void Deserialize(const pugi::xml_node& node) {}
};

#define SERIALIZABLE(CLASS)                                                                \
    static Type type;								  									   \
	CLASS* Clone() const { return new CLASS(*this); } 								       \
	const Type& GetType() const { return type; };	  								       \
	void Serialize(pugi::xml_node& node)												   \
	{																					   \
		for (auto field : type.Fields())												   \
		{																				   \
			ISerializableAttribute* srlzAttr = field->Attribute<ISerializableAttribute>(); \
			if (srlzAttr)																   \
				srlzAttr->Serialize(this, node);										   \
		}																				   \
	}																					   \
																						   \
	void Deserialize(const pugi::xml_node& node)										   \
	{																					   \
		for (auto field : type.Fields())												   \
		{																				   \
			ISerializableAttribute* srlzAttr = field->Attribute<ISerializableAttribute>(); \
			if (srlzAttr)																   \
				srlzAttr->Deserialize(this, node);										   \
		}																				   \
	}																					   \
    static void InitializeType(CLASS* sample)   									       

template<typename _type>
inline _type GetXmlAttributeValue(pugi::xml_attribute& attr)
{
	return attr.value();
}

template<>
inline int GetXmlAttributeValue<int>(pugi::xml_attribute& attr)
{
	return attr.as_int();
}

template<>
inline float GetXmlAttributeValue<float>(pugi::xml_attribute& attr)
{
	return attr.as_float();
}


template<typename _type>
inline void SetXmlAttributeValue(pugi::xml_attribute& attr, const _type& value)
{
	attr = value;
}

template<>
inline void SetXmlAttributeValue<std::string>(pugi::xml_attribute& attr, const std::string& value)
{
	attr = value.c_str();
}

template<typename _type>
void SerializableAttribute<_type>::Deserialize(void* object, const pugi::xml_node& node) const
{
	pugi::xml_attribute val = node.child(mOwnerFieldInfo->Name().c_str()).attribute("value");
	if (val)
	{
		_type* valPtr = mOwnerFieldInfo->GetValuePtrStrong<_type>(object);
		*valPtr = GetXmlAttributeValue<_type>(val);
	}
}

template<typename _type>
void SerializableAttribute<_type>::Serialize(void* object, pugi::xml_node& node)
{
	SetXmlAttributeValue<_type>(node.append_child(mOwnerFieldInfo->Name().c_str()).append_attribute("value"),
								mOwnerFieldInfo->GetValue<_type>(object));
}
