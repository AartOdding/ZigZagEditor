#pragma once

#include <object/Identifier.hpp>
#include <object/NodeCategory.hpp>
#include <util/Platform.hpp>


class Node;
class Template;

namespace Host
{
	Identifier<Node> createNode(Identifier<Template> templateID, Identifier<Node> parentID);
	bool destroyNode(Identifier<Node> nodeID);
	bool setNodeParent(Identifier<Node> nodeID, Identifier<Node> newParentID);
}


typedef std::uint64_t(*CreateNodeDelegate)(std::uint64_t templateID, std::uint64_t parentID);
typedef bool (*DestroyNodeDelegate)(std::uint64_t nodeID);
typedef bool (*SetNodeParentDelegate)(std::uint64_t nodeID, std::uint64_t newParentID);

ZIGZAG_API void installObjectDelegates(CreateNodeDelegate create, DestroyNodeDelegate destroy,
	                                   SetNodeParentDelegate setParent);


ZIGZAG_API void onTemplateAdded(const char* name, std::uint64_t templateID, NodeCategory category);

ZIGZAG_API void onNodeConstructed(std::uint64_t nodeID, std::uint64_t parentID, std::uint64_t templateID);
ZIGZAG_API void onNodeDestroyed(std::uint64_t nodeID);
ZIGZAG_API void onNodeParentChanged(std::uint64_t nodeID, std::uint64_t newParentID);

