#include <Project.hpp>


Project* Project::getGlobalInstance()
{
    static Project globalInstance;
    return &globalInstance;
}

void Project::setRootObject(std::unique_ptr<ZObject>&& object)
{
    m_root = std::move(object);
}

void Project::clearRootObject()
{
    m_root.reset();
}
