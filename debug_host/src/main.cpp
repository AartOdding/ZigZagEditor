#include <cstdint>

#include <iostream>

#include <interop/MainloopInterop.hpp>
#include <interop/ObjectInterop.hpp>
#include <interop/ProjectInterop.hpp>


ZIGZAG_API void initialize();
ZIGZAG_API void render();
ZIGZAG_API void shutdown();
ZIGZAG_API bool shouldQuit();


std::uint64_t nextID = 5000;


bool createObject(std::uint64_t objectType, std::uint64_t parentObject)
{
	std::cout << "[host] creating object of type: " << objectType << " with parent: " << parentObject << std::endl;
	onObjectCreated(++nextID, parentObject);
	return true;
}

bool removeObject(std::uint64_t objectIdentifier)
{
	std::cout << "[host] removing object with identifier: " << objectIdentifier << std::endl;
	return true;
}


int main()
{
	initialize();

	installObjectDelegates(createObject, removeObject);

	onProjectCreated("project", ++nextID);

	onNewObjectTypeAdded("ZigZag.OpenGL.Texture.SineWave", ++nextID, ObjectTypeCategory::Operator);
	onNewObjectTypeAdded("ZigZag.OpenGL.Texture.SquareWave", ++nextID, ObjectTypeCategory::Operator);
	onNewObjectTypeAdded("ZigZag.OpenGL.Texture.Noise", ++nextID, ObjectTypeCategory::Operator);
	onNewObjectTypeAdded("ZigZag.OpenGL.Geometry.Triangles", ++nextID, ObjectTypeCategory::Operator);
	onNewObjectTypeAdded("ZigZag.OpenGL.Geometry.Sphere", ++nextID, ObjectTypeCategory::Operator);
	onNewObjectTypeAdded("ZigZag.OpenGL.Geometry.PlatonicSolid", ++nextID, ObjectTypeCategory::Operator);
	onNewObjectTypeAdded("ZigZag.Image.LoadImage", ++nextID, ObjectTypeCategory::Operator);
	onNewObjectTypeAdded("ZigZag.Image.SaveImage", ++nextID, ObjectTypeCategory::Operator);
	onNewObjectTypeAdded("ZigZag.OpenCV.EdgeDetection", ++nextID, ObjectTypeCategory::Operator);
	onNewObjectTypeAdded("ZigZag.OpenCV.MatrixFromImage", ++nextID, ObjectTypeCategory::Operator);


	while (!shouldQuit())
	{
		render();
	}

	shutdown();
}
