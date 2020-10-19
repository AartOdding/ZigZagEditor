#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <object/Identity.hpp>
#include <object/NodeCategory.hpp>
#include <object/NodeTemplate.hpp>
#include <tci/views.hpp>



class Node : public Identity<Node>
{
    Node() = delete;
    Node(Node&&) = delete;
    Node(const Node&) = delete;

public:

    using Pointer = std::unique_ptr<Node>;

    using Children = std::vector<Pointer>;
    using ChildrenView = tci::view<Children>;
    using ConstChildrenView = tci::const_view<Children>;

    static Pointer create(Identifier<Node> identifier);
    ~Node() = default;

    Node* addChild(Pointer&& child);
    Pointer removeChild(Identifier<Node> childIdentifier);
    
    Node* getParent();
    const Node* getParent() const;

    ChildrenView getChildren();
    ConstChildrenView getChildren() const;

    void setCategory(NodeCategory category);
    NodeCategory getCategory() const;

    void setName(std::string_view name);
    const std::string& getName() const;

private:

    Node(Identifier<Node> identifier);

    Node* m_parent;
    std::vector<std::unique_ptr<Node>> m_children;
    std::string m_name;
    NodeCategory m_category = NodeCategory::Operator;

};
