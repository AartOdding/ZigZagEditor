#include <Application.hpp>
#include <interop/ProjectInterop.hpp>
#include <object/ZObject.hpp>

#include <iostream>


ZIGZAG_API void onProjectCreated(const char* name, std::uint64_t projectID)
{
    std::cout << "[editor dll] project created: " << name << " " << projectID << std::endl;
    auto rootObject = ZObject::create(Identifier<ZObject>(projectID));
    Application::getGlobalInstance()->setRootObject(std::move(rootObject));
}

ZIGZAG_API void onProjectCleared()
{
    std::cout << "[editor dll] project cleared" << std::endl;
    Application::getGlobalInstance()->clearRootObject();
}
