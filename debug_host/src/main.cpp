#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include <interop/MainloopInterop.hpp>
#include <interop/ObjectInterop.hpp>
#include <interop/ProjectInterop.hpp>


ZIGZAG_API void initialize();
ZIGZAG_API void render();
ZIGZAG_API void shutdown();
ZIGZAG_API bool shouldQuit();

void recursivelyDelete(std::uint64_t objectID);


namespace 
{
	std::uint64_t nextID = 5000;

	struct ObjType
	{
		std::string name;
		std::uint64_t id;
		NodeCategory category;
	};

	struct ObjInstance
	{
		std::string name;
		std::uint64_t id = 0;

		std::uint64_t type = 0;
		std::uint64_t parent = 0;
		std::unordered_set< std::uint64_t> children;
	};

	std::unordered_map<std::uint64_t, ObjType> objectTypes
	{
		{ ++nextID, { "ZigZag.OpenGL.Texture.SineWave", nextID, NodeCategory::Operator } },
		{ ++nextID, { "ZigZag.OpenGL.Texture.SquareWave", nextID, NodeCategory::Operator } },
		{ ++nextID, { "ZigZag.OpenGL.Texture.Noise", nextID, NodeCategory::Operator } },
		{ ++nextID, { "ZigZag.OpenGL.Geometry.Triangles", nextID, NodeCategory::Operator } },
		{ ++nextID, { "ZigZag.OpenGL.Geometry.Sphere", nextID, NodeCategory::Operator } },
		{ ++nextID, { "ZigZag.OpenGL.Geometry.PlatonicSolid", nextID, NodeCategory::Operator } },
		{ ++nextID, { "ZigZag.Image.LoadImage", nextID, NodeCategory::Operator } },
		{ ++nextID, { "ZigZag.Image.SaveImage", nextID, NodeCategory::Operator } },
		{ ++nextID, { "ZigZag.OpenCV.EdgeDetection", nextID, NodeCategory::Operator } },
		{ ++nextID, { "ZigZag.OpenCV.MatrixFromImage", nextID, NodeCategory::Operator } }
	};

	std::unordered_map<std::uint64_t, ObjInstance> objectInstances;


	struct ObjTask
	{
		virtual void execute() = 0;
	};

	struct CreateObjectTask : ObjTask
	{
		std::uint64_t typeID;
		std::uint64_t parentID;
		std::uint64_t objectID;

		CreateObjectTask(std::uint64_t t, std::uint64_t p) : typeID(t), parentID(p), objectID(++nextID) {}

		virtual void execute() override
		{
			ObjInstance object;
			object.name = "Name";
			object.id = objectID;
			object.type = typeID;
			object.parent = parentID;
			objectInstances[object.id] = object;

			onNodeConstructed(object.id, object.parent, object.type);
		}
	};

	struct DeleteObjectTask : ObjTask
	{
		std::uint64_t objectID;

		DeleteObjectTask(std::uint64_t id) : objectID(id) {}

		virtual void execute() override
		{
			auto object = objectInstances[objectID];
			
			if (object.parent != 0)
			{
				// remove the object in the parent's children.
				objectInstances[object.parent].children.erase(objectID);
			}

			// will also delete all children
			recursivelyDelete(objectID);

			onNodeDestroyed(objectID);
		}
	};

	struct ReparentingTask : ObjTask
	{
		std::uint64_t objectID;
		std::uint64_t newParentID;

		ReparentingTask(std::uint64_t object, std::uint64_t newParent) : objectID(object), newParentID(newParent) {}

		virtual void execute() override
		{
			objectInstances[objectID].parent = newParentID;
			onNodeParentChanged(objectID, newParentID);
		}
	};

	std::vector<std::unique_ptr<ObjTask>> taskQueue;
}


std::uint64_t createObject(std::uint64_t objectType, std::uint64_t parentObject)
{
	std::cout << "[Host] creating object of type: " << objectType << " with parent: " << parentObject << std::endl;

	if (objectTypes.count(objectType) == 1)
	{
		if (objectInstances.count(parentObject) == 1 || parentObject == 0)
		{
			auto task = std::make_unique<CreateObjectTask>(objectType, parentObject);
			auto id = task->objectID;
			taskQueue.push_back(std::move(task));
			return id;
		}
	}
	return 0;
}

bool destroyNode(std::uint64_t objectID)
{
	std::cout << "[Host] removing object with identifier: " << objectID << std::endl;

	if (objectInstances.count(objectID) == 1)
	{
		taskQueue.push_back(std::make_unique<DeleteObjectTask>(objectID));
		return true;
	}
	return false;
}

bool setNodeParent(std::uint64_t objectID, std::uint64_t newParentID)
{
	std::cout << "[Host] reparenting object: " << objectID << " to new parent: " << newParentID << std::endl;

	// IMPORTANT: should also check for loops

	//if (objectInstances.count(objectID) == 1)
	//{
		if (newParentID == 0 || objectInstances.count(newParentID) == 1)
		{
			taskQueue.push_back(std::make_unique<ReparentingTask>(objectID, newParentID));
			return true;
		}
	//}
	return false;
}


int main()
{
	ObjType projectType{ "ZigZag.Project", ++nextID, NodeCategory::Project };
	objectTypes[projectType.id] = projectType;

	ObjInstance projectInstance{ "Project", ++nextID, projectType.id, 0, {} };
	objectInstances[projectInstance.id] = projectInstance;


	initialize();
	installObjectDelegates(createObject, destroyNode, setNodeParent);

	for (const auto& [id, t] : objectTypes)
	{
		onTemplateAdded(t.name.c_str(), t.id, t.category);
	}

	onProjectCreated(projectInstance.name.c_str(), projectInstance.id);

	while (!shouldQuit())
	{
		for (auto& task : taskQueue)
		{
			task->execute();
		}
		taskQueue.clear();

		// Lastly render the ui
		render();
	}

	shutdown();
}


void recursivelyDelete(std::uint64_t objectID)
{
	if (objectInstances.count(objectID) == 1)
	{
		auto object = &objectInstances[objectID];

		for (auto childID : object->children)
		{
			recursivelyDelete(childID);
		}
		objectInstances.erase(objectID);
	}
}
