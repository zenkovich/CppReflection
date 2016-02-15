#pragma once

#include "Type.h"

// ----------------------------------------------------
// Basic object interface with type information support
// ----------------------------------------------------
class IObject
{
public:
	// Virtual destructor
	virtual ~IObject() {}

	// Cloning interface
	virtual IObject* Clone() const = 0;

	// Returns type
	virtual const Type& GetType() const = 0;
};

// IObject header definition
#define IOBJECT(CLASS)  							   \
	CLASS* Clone() const { return new CLASS(*this); }  \
	static Type type;								   \
	const Type& GetType() const { return type; };	   \
	friend struct Type::TypeCreator<CLASS>;            \
	static void InitializeType(CLASS* sample)    
