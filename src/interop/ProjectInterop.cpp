#include <Application.hpp>
#include <object/ZObject.hpp>
#include <util/Platform.hpp>

#include <cstdint>
#include <iostream>


ZIGZAG_API void onProjectCreated(const char* name, std::uint64_t projectID)
{
    std::cout << "project created: " << name << ", id: " << projectID << std::endl;
    auto rootObject = ZObject::create(Identifier<ZObject>(projectID));
    Application::getGlobalInstance()->setRootObject(std::move(rootObject));
}

ZIGZAG_API void onProjectCleared()
{
    std::cout << "project cleared" << std::endl;
    Application::getGlobalInstance()->clearRootObject();
}
