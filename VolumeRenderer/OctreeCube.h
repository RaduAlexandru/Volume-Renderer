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
	bool polygonised;
	bool containsVerts;

	OctreeCube* children[8];
	OctreeCube* parent;
	std::vector<glm::vec3> *points;

	int depth;

	std::vector<int> locationalCode;

	int needsSubdivision( int, std::vector< boost::unordered_map< std::pair<int, int>, glm::vec3> > &  );
	int subdivide(std::vector <OctreeCube> &);

	int subdivide_tree_version();
};

