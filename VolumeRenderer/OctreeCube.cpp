#include "OctreeCube.h"


OctreeCube::OctreeCube()
{
	origin.x = 0;
	origin.y = 0;
	origin.z = 0;
	sizeX = 0;
	sizeY = 0;
	sizeZ = 0;
	isLeaf = false;
	needsChecking = true;
	polygonised = false;
	containsVerts = false;
	points = NULL;
	depth = 0;

	parent = NULL;
	for (int i = 0; i < 8; i++){
		children[i] = NULL;
	}
}


OctreeCube::~OctreeCube()
{
}



/*! \brief Crea los 8 hijos y les asignas las posiciones y el codigo de localizacion adeacuado (segun la ordenacion Z)
*/
int OctreeCube::subdivide(){



	//Number of cubes and position

	/*
	    /        /
	   /  4     /    5
	  /        /
	//-------------------
	//|   o    !    1   !
	//!        !        !
	//!-----------------
	//!   2    !    3   !
	//!        !        !
	//-------------------

	*/



	
	for (int i = 0; i < 8; i++){
		children[i] = new OctreeCube();
	}



	for (int i = 0; i < 8; i++){

		children[i]->isLeaf = true;
		children[i]->sizeX = sizeX / 2;
		children[i]->sizeY = sizeY / 2;
		children[i]->sizeZ = sizeZ / 2;
		children[i]->depth = depth + 1;

		//Copy the locational code of the father. Later when we create each children, they will change their correspondign bit

		children[i]->locationalCode.resize(locationalCode.size());
		for (int j = 0; j < locationalCode.size(); j++){
			children[i]->locationalCode[j] = locationalCode[j];
		}

	}


	//Cube 1
	children[0]->origin.x = origin.x;
	children[0]->origin.y = origin.y + sizeY / 2;
	children[0]->origin.z = origin.z;
	children[0]->parent = this;
	children[0]->locationalCode[children[0]->depth - 1] = 0;
	//cubes.push_back(subdivs[0]);
	//Cube 2
	children[1]->origin.x = origin.x + sizeX / 2;
	children[1]->origin.y = origin.y + sizeY / 2;
	children[1]->origin.z = origin.z;
	children[1]->parent = this;
	children[1]->locationalCode[children[1]->depth - 1] = 1;
	//cubes.push_back(subdivs[1]);
	//Cube 3
	children[2]->origin.x = origin.x;
	children[2]->origin.y = origin.y;
	children[2]->origin.z = origin.z;
	children[2]->parent = this;
	children[2]->locationalCode[children[2]->depth - 1] = 2;
	//cubes.push_back(subdivs[3]);
	//Cube 4
	children[3]->origin.x = origin.x + sizeX / 2;
	children[3]->origin.y = origin.y;
	children[3]->origin.z = origin.z;
	children[3]->parent = this;
	children[3]->locationalCode[children[3]->depth - 1] = 3;
	//cubes.push_back(subdivs[2]);
	

	//Cube 5
	children[4]->origin.x = origin.x;
	children[4]->origin.y = origin.y + sizeY / 2;
	children[4]->origin.z = origin.z + sizeZ / 2;
	children[4]->parent = this;
	children[4]->locationalCode[children[4]->depth - 1] = 4;
	//cubes.push_back(subdivs[4]);
	//Cube 6
	children[5]->origin.x = origin.x + sizeX / 2;
	children[5]->origin.y = origin.y + sizeY / 2;
	children[5]->origin.z = origin.z + sizeZ / 2;
	children[5]->parent = this;
	children[5]->locationalCode[children[5]->depth - 1] = 5;
	//cubes.push_back(subdivs[5]);
	//Cube 7
	children[6]->origin.x = origin.x;
	children[6]->origin.y = origin.y;
	children[6]->origin.z = origin.z + sizeZ / 2;
	children[6]->parent = this;
	children[6]->locationalCode[children[6]->depth - 1] = 6;
	//cubes.push_back(subdivs[7]);
	//Cube 8
	children[7]->origin.x = origin.x + sizeX / 2;
	children[7]->origin.y = origin.y;
	children[7]->origin.z = origin.z + sizeZ / 2;
	children[7]->parent = this;
	children[7]->locationalCode[children[7]->depth - 1] = 7;
	//cubes.push_back(subdivs[6]);
	





	return 0;
}