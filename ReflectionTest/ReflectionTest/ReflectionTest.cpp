#include "ReflectionGenerated.h"

#include <string>
#include "Test.h"

int main()
{
	// Registering application types
	RegReflectionTypes();

	// List fields in class TestB
	for (auto fld : TestB::type.Fields())
		printf("Field: %s\n", fld->Name().c_str());

	// Create example
	TestB a;

	// Change something
	a.intParam = -3;

	// Then serialize
	pugi::xml_document doc;
	pugi::xml_node node = doc.append_child("data");
	a.Serialize(node);
	doc.save_file("test.txt");

	// Create another example
	TestB b;

	// And deserialize
	b.Deserialize(node);

	// Get type of class TestB
	Type bType = TestB::type;

	// Get type of b
	Type bTypeToo = b.GetType();

	// Create example and print his type
	IObject* bClone = bType.CreateSample();
	printf("bClone type is %s\n", bClone->GetType().Name().c_str());

	// Get field
	const FieldInfo* info = bType.Field("bParam");

	// Get path
	std::string path = bType.GetFieldPath(&b, &b.pa->intParam);
	printf("path is %s\n", path.c_str());

	// And get pointer from path
	FieldInfo* ptrInfo;
	int* ptr = bType.GetFieldPtr<int>(&b, path, ptrInfo);

	// Try to invoke function void TypeB::FuncVirt(int a, float b, std::string c);
	bType.Invoke<void, int, float, std::string>("FuncVirt", &b, 1, 2.0f, (std::string)"3");

    return 0;
}

