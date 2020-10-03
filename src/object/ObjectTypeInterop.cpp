#include <Platform.hpp>
#include <object/ObjectTypeLibrary.hpp>

#include <iostream>



ZIGZAG_EXPORT void onNewObjectTypeAdded(const char* name, std::uint64_t uniqueID, ObjectTypeCategory category)
{
	std::cout << "Object added: name: " << name << ", id: " << uniqueID << ", type: ";

	switch (category)
	{
	case ObjectTypeCategory::DataSource:
		std::cout << "data source" << std::endl;
		break;
	case ObjectTypeCategory::Operator:
		std::cout << "oeprator" << std::endl;
		break;
	case ObjectTypeCategory::Object:
		std::cout << "object" << std::endl;
		break;
	default:
		std::cout << "error" << std::endl;
		break;
	}
}


