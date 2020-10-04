#pragma once

#include <object/Identifier.hpp>
#include <object/ZObject.hpp>


class Project
{
public:

    Project(Project&&) = delete;
    Project(const Project&) = delete;

    static Project* getGlobalInstance();

    void setRootObject(std::unique_ptr<ZObject>&& object);
    void clearRootObject();

private:

    Project() = default;

    std::unique_ptr<ZObject> m_root;

};
