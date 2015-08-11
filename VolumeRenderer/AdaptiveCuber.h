#pragma once
#include "MarchingCuber.h"
class AdaptiveCuber:public MarchingCuber
{
public:
	AdaptiveCuber();
	~AdaptiveCuber();
	AdaptiveCuber(PixelData* pixelData, Mesh* mesh, int isoLevel, int cellSizeX, int cellSizeY, int cellSizeZ, int interpolateDepth, int octreeMaxDepth, std::vector< boost::unordered_map< std::pair<int, int>, glm::vec3> >* gradient, int tolerance);

	int run();
	int runWithCracks();
	void calculateGradient();
	OctreeCube* createInitialCube();
	int generateOctree_tree_version(OctreeCube&, int = 0);
	inline bool cubeNeedsSubdivision(OctreeCube &cube);
	int polygoniseOctree2(OctreeCube* root);
	int polygoniseAssignToCube(OctreeCube* currentCube);
	void crackPatch(OctreeCube* root);
	glm::vec3 pointOnLine(glm::vec3 a, glm::vec3 b, glm::vec3& x);
	int readPointsFromOctree(OctreeCube* root);
	void findNeighbours(OctreeCube* currentCube, OctreeCube* root,OctreeCube*& right, OctreeCube*& left, OctreeCube*& top, OctreeCube*& bottom, OctreeCube*& further, OctreeCube*& closer);
	std::vector<int> getLocationalOfNeighbours(std::vector<int> currentLocation, int depth, int direction);
	OctreeCube* traverseWithLocation(OctreeCube* root, std::vector<int> neighLoc, int depth);
	void patchFace(OctreeCube* lowRes, OctreeCube* highRes, int direction);
	void getPointsFromFace(OctreeCube* cube, OctreeCube* reference, std::vector<glm::vec3>& vec, int direction);
	std::vector<glm::vec3>  removeDuplicates(std::vector<glm::vec3> vec);
	bool checkSurfaceOrientation(OctreeCube* lowRes, OctreeCube* highRes);

	int octreeMaxDepth; //!< Maxima profundidad que puede alcanzar el arbol
	std::vector< boost::unordered_map< std::pair<int, int>, glm::vec3> >* gradient; //!< Puntero a los gradientes del modelo
	int tolerance; //!< EL maximo valor que los angulos de los gradientes pueden variar dentro de un cubo antes de que sea neesario subdividir

	std::vector<OctreeCube*>  octreeVector; //!< Se guarda punteros a los vectores del arbol para facilitar el recorrido y el borrado



};

