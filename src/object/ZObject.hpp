#pragma once

#include <memory>
#include <vector>

#include <object/Identifier.hpp>

/*
ISSUES:

    setParent should only be accessible when coming from csharp callback

    nothing stops loops in object hierarchy

*/



class ZObject
{
public:

    ZObject() = delete;
    ZObject(ZObject&&) = delete;
    ZObject(const ZObject&) = delete;

    ~ZObject();
    
    static std::unique_ptr<ZObject> create(Identifier identifier);

    ZObject* createChild(Identifier identifier);
    void addChild(std::unique_ptr<ZObject>&& child);
    std::unique_ptr<ZObject> stealFromParent();
    
    Identifier getIdentifier() const;
    ZObject* getParent();
    const std::vector<std::unique_ptr<ZObject>>& getChildren();

private:

    ZObject(Identifier identifier);

    Identifier m_identifier;
    ZObject* m_parent;
    std::vector<std::unique_ptr<ZObject>> m_children;

};
