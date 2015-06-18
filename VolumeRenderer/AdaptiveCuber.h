#pragma once
#include "MarchingCuber.h"
class AdaptiveCuber:public MarchingCuber
{
public:
	AdaptiveCuber();
	~AdaptiveCuber();
	AdaptiveCuber(unsigned char*** pixelData, std::vector<glm::vec3>* verts, std::vector<glm::vec3>* normals, int isoLevel, int frames, int pixelDataHeight, int pixelDataWidth, int cellSizeX, int cellSizeY, int cellSizeZ, int pointerOffset, int interpolateDepth, bool linearInterpolation, int octreeMaxDepth, std::vector< boost::unordered_map< std::pair<int, int>, glm::vec3> >* gradient, int tolerance);

	int run();
	void calculateGradient();
	OctreeCube createInitialCube();
	int generateOctree_tree_version(OctreeCube&, int = 0);
	inline bool cubeNeedsSubdivision(OctreeCube &cube);
	void correctlyAssignLeafs(OctreeCube* root);
	int polygoniseOctree2(OctreeCube* root);
	int polygoniseAssignToCube(OctreeCube* currentCube);
	void crackPatch(OctreeCube* root);
	glm::vec3 pointOnLine(glm::vec3 a, glm::vec3 b, glm::vec3& x);
	int readPointsFromOctree(OctreeCube* root);

	int octreeMaxDepth;
	std::vector< boost::unordered_map< std::pair<int, int>, glm::vec3> >* gradient;
	int tolerance;

	std::vector<OctreeCube*>  octreeVector;
};

