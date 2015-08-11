#include <glm\glm.hpp>
#include <vector>
#include <boost\unordered_map.hpp>
#pragma once
class OctreeCube
{
public:
	OctreeCube();
	~OctreeCube();


	glm::vec3 origin;			//!< El punto de origen del cubo coincide con la esquina inferior izquierda
	int sizeX, sizeY, sizeZ;	//!< EL tamaño del cubo en las 3 dirrecciones;

	bool isLeaf;
	bool needsChecking;			//!< Si se ha descubierto que el cubo no contiene una superficie, se descarta de cualquier otro calculo posterior como crack patch
	bool polygonised;
	bool containsVerts;

	OctreeCube* children[8];
	OctreeCube* parent;
	std::vector<glm::vec3> *points;		//!< Referencia a los puntos (los triangulos) creados dentro de el

	int depth;	//!< La profundidad a la que se encuentra en el arbol, usada para calcular los vecinos

	std::vector<int> locationalCode;//!< Codigo de localizacion, usado para el calculo de vecinos
	int subdivide();
};

