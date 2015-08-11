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
#include <Mesh.h>






class Model
{
public:
	Model();
	~Model();

	int isoLevel;	//!< Representa el valor umbral con el que se segmenta el volumen de datos 
	
	int cellSizeX;	//!< El tamaño de los cubos para MarchingCubes
	int cellSizeY;
	int cellSizeZ;	
	PixelData* pixelData;	//!< Puntero al volumen de datos leios de archivos dicom 

	Mesh* mesh;		//!< El mallado principal, el que se puede modificar mediante umbralizacion
	Mesh* mesh2;	//!< El mallado secundario, el usado como pin. No se pueden modificar sus vertices
	

	std::vector<int>  edgeTable;
	std::vector< std::vector<int> > triTable;

	int algorithmChosen;	//!< El algoritmo de reconstruccion elegido 1=MC 2=AMC 3=AMC(crack patch)
	int normalsAlgChosen;	//!< El algoritmo de normales elegido 1= por triangulo 2=por vertice
	bool generatingMesh;
	

	std::vector< boost::unordered_map< std::pair<int, int>, glm::vec3> > gradient;


	int tolerance;	//!< El algoritmo de reconstruccion elegido 1=MC 2=AMC 3=AMC(crack patch)
	int octreeMaxDepth;
	int interpolateDepth;
};

