#pragma once

#include <vector>
#include <list>
#include <glm/glm.hpp>
#include "OctreeCube.h"
#include "PixelData.h"
//#include <boost/variant.hpp>
//#include <boost/any.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <map>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <boost/unordered_map.hpp>


class MarchingCuber
{
public:
	MarchingCuber();
	MarchingCuber(PixelData* pixelData, std::vector<glm::vec3>* verts, std::vector<glm::vec3>* normals, int isoLevel ,  int cellSizeX, int cellSizeY, int cellSizeZ, int interpolateDepth );
	~MarchingCuber();

	typedef struct {
		glm::vec3 position[8];	//Position of the 8 vertices
		glm::vec3 normal[8];	//normals of those vertices I dont know if I'm going to use it
		double val[8];		//value at those vertices
	} CELL;

	void run();
	int polygonise(CELL &);
	inline void interpolate(int, glm::vec3 point1, glm::vec3 point2, float, float, glm::vec3&, int depth = 0);


	

	std::vector<int>  edgeTable;
	std::vector< std::vector<int> > triTable;

	//unsigned char*** pixelData;

	PixelData* pixelData;

	std::vector<glm::vec3>* verts;
	std::vector<glm::vec3>* normals;

	int isoLevel; //!< Detailed Representa el valor umbral con el que se segmenta el volumen de datos 
	int frames;
	int pixelDataHeight;
	int pixelDataWidth;
	int cellSizeX;
	int cellSizeY;
	int cellSizeZ;
	int pointerOffset;
	int interpolateDepth;
	bool linearInterpolation;

	


};

