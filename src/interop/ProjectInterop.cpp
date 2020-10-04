#include <Platform.hpp>
#include <Project.hpp>
#include <object/ZObject.hpp>

#include <cstdint>
#include <iostream>


ZIGZAG_EXPORT void onProjectCreated(const char* name, std::uint64_t projectID)
{
    std::cout << "project created: " << name << ", id: " << projectID << std::endl;
    auto rootObject = ZObject::create(Identifier(projectID));
    Project::getGlobalInstance()->setRootObject(std::move(rootObject));
}

ZIGZAG_EXPORT void onProjectCleared()
{
    std::cout << "project cleared" << std::endl;
    Project::getGlobalInstance()->clearRootObject();
}
