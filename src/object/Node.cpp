#include <stdexcept>

#include <object/Node.hpp>


Node::Node(Identifier<Node> identifier)
    : Identity<Node>(this, identifier)
    , m_parent(nullptr)
{
}

std::unique_ptr<Node> Node::create(Identifier<Node> identifier)
{
    if (identifier)
    {
        return std::unique_ptr<Node>(new Node(identifier));
    }
    return std::unique_ptr<Node>();
}

Node* Node::addChild(Pointer&& child)
{
    if (child && !child->m_parent)
    {
        child->m_parent = this;
        m_children.push_back(std::move(child));
        return m_children.back().get();
    }
    return nullptr;
}

Node::Pointer Node::removeChild(Identifier<Node> childIdentifier)
{
    for(auto it = m_children.begin(); it != m_children.end(); ++it)
    {
        if ((*it)->getIdentifier() == childIdentifier)
        {
            Pointer child = std::move(*it);
            child->m_parent = nullptr;
            m_children.erase(it);
            return std::move(child);
        }
    }
    return Pointer();
}

Node* Node::getParent()
{
    return m_parent;
}

const Node* Node::getParent() const
{
    return m_parent;
}

Node::ChildrenView Node::getChildren()
{
    return ChildrenView(m_children);
}

Node::ConstChildrenView Node::getChildren() const
{
    return ConstChildrenView(m_children);
}

void Node::setCategory(NodeCategory category)
{
    m_category = category;
}

NodeCategory Node::getCategory() const
{
    return m_category;
}

void Node::setName(std::string_view name)
{
    m_name = name;
}

const std::string& Node::getName() const
{
    return m_name;
}
