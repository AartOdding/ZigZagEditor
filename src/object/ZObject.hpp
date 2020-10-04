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
    
    Identifier getIdentifier() const;
    ZObject* getParent();
    const std::vector<std::unique_ptr<ZObject>>& getChildren();

    std::unique_ptr<ZObject> stealFromParent();
    void addChild(std::unique_ptr<ZObject>&& child);

private:

    ZObject(Identifier identifier);

    Identifier m_identifier;
    ZObject* m_parent;
    std::vector<std::unique_ptr<ZObject>> m_children;

};
