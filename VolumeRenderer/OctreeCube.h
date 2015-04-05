#include <glm\glm.hpp>
#include <vector>
#pragma once
class OctreeCube
{
public:
	OctreeCube();
	~OctreeCube();
	glm::vec3 pos[8];

	glm::vec3 origin;
	int sizeX, sizeY, sizeZ;

	bool isLeaf;

	int needsSubdivision();
	int subdivide(std::vector <OctreeCube> &);
};

