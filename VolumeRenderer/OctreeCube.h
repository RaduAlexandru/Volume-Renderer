#include <glm\glm.hpp>
#pragma once
class OctreeCube
{
public:
	OctreeCube();
	~OctreeCube();
	glm::vec3 topLeft, topRight, bottomLeft, bottomRight;
	bool isLeaf;
};

