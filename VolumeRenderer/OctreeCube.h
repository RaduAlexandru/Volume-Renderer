#include <glm\glm.hpp>
#include <vector>
#include <boost\unordered_map.hpp>
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
	bool needsChecking;

	int needsSubdivision( int, std::vector< boost::unordered_map< std::pair<int, int>, glm::vec3> > &  );
	int subdivide(std::vector <OctreeCube> &);
};

