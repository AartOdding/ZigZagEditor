#pragma once

#include <memory>
#include <vector>

#include <object/Identity.hpp>
#include <tci/views.hpp>


class ZObject : public Identity<ZObject>
{
    ZObject() = delete;
    ZObject(ZObject&&) = delete;
    ZObject(const ZObject&) = delete;

public:

    using Pointer = std::unique_ptr<ZObject>;

    using Children = std::vector<Pointer>;
    using ChildrenView = tci::view<Children>;
    using ConstChildrenView = tci::const_view<Children>;

    static Pointer create(Identifier<ZObject> identifier);
    ~ZObject() = default;

    void addChild(Pointer&& child);
    Pointer removeChild(Identifier<ZObject> childIdentifier);
    
    ZObject* getParent();
    const ZObject* getParent() const;

    ChildrenView getChildren();
    ConstChildrenView getChildren() const;

private:

    ZObject(Identifier<ZObject> identifier);

    ZObject* m_parent;
    std::vector<std::unique_ptr<ZObject>> m_children;

};
