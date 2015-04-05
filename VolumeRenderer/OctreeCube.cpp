#include "OctreeCube.h"


OctreeCube::OctreeCube()
{
	origin.x = 0;
	origin.y = 0;
	origin.z = 0;
	sizeX = 0;
	sizeY = 0;
	sizeZ = 0;
}


OctreeCube::~OctreeCube()
{
}

int OctreeCube::needsSubdivision(){
	int needSubdivision;
	needSubdivision = rand() % 2;
	return needSubdivision;
}
int OctreeCube::subdivide(std::vector <OctreeCube> & cubes){

	


	//Create the cubes
	//add them t the cubes vector


	/*
	     /        /
	    /  5     /    6
	   /        /
	//-------------------
	//|   1    !    2   !
	//!        !        !
	//!-----------------
	//!   4    !    3   !
	//!        !        !
	//-------------------

	octreeCube.topLeft.x = cubes[cube].topLeft.x;
	octreeCube.topLeft.y = cubes[cube].topLeft.y;

	octreeCube.topRight.x = (cubes[cube].topRight.x - cubes[cube].topLeft.x) / 2 + cubes[cube].topLeft.x;
	octreeCube.topRight.y = cubes[cube].topRight.y;

	octreeCube.bottomLeft.x = cubes[cube].bottomLeft.x;
	octreeCube.bottomLeft.y = (cubes[cube].topLeft.y - cubes[cube].bottomLeft.y) / 2 + cubes[cube].bottomLeft.y;

	octreeCube.bottomRight.x = (cubes[cube].bottomRight.x - cubes[cube].bottomLeft.x) / 2 + cubes[cube].bottomLeft.x;
	octreeCube.bottomRight.y = (cubes[cube].topRight.y - cubes[cube].bottomRight.y) / 2 + cubes[cube].bottomRight.y;

	octreeCube.isLeaf = true;
	cubes.push_back(octreeCube);

	//Cube2
	octreeCube.topLeft.x = (cubes[cube].topRight.x - cubes[cube].topLeft.x) / 2 + cubes[cube].topLeft.x;
	octreeCube.topLeft.y = cubes[cube].topLeft.y;

	octreeCube.topRight = cubes[cube].topRight;


	octreeCube.bottomLeft.x = (cubes[cube].bottomRight.x - cubes[cube].bottomLeft.x) / 2 + cubes[cube].bottomLeft.x;
	octreeCube.bottomLeft.y = (cubes[cube].topRight.y - cubes[cube].bottomRight.y) / 2 + cubes[cube].bottomRight.y;

	octreeCube.bottomRight.x = cubes[cube].bottomRight.x;
	octreeCube.bottomRight.y = (cubes[cube].topRight.y - cubes[cube].bottomRight.y) / 2 + cubes[cube].bottomRight.y;

	octreeCube.isLeaf = true;
	cubes.push_back(octreeCube);

	//Cube3
	octreeCube.topLeft.x = cubes[cube].topLeft.x;
	octreeCube.topLeft.y = (cubes[cube].topLeft.y - cubes[cube].bottomLeft.y) / 2 + cubes[cube].bottomLeft.y;

	octreeCube.topRight.x = (cubes[cube].bottomRight.x - cubes[cube].bottomLeft.x) / 2 + cubes[cube].bottomLeft.x;
	octreeCube.topRight.y = (cubes[cube].topRight.y - cubes[cube].bottomRight.y) / 2 + cubes[cube].bottomRight.y;

	octreeCube.bottomLeft.x = cubes[cube].bottomLeft.x;
	octreeCube.bottomLeft.y = cubes[cube].bottomLeft.y;

	octreeCube.bottomRight.x = (cubes[cube].bottomRight.x - cubes[cube].bottomLeft.x) / 2 + cubes[cube].bottomLeft.x;
	octreeCube.bottomRight.y = cubes[cube].bottomRight.y;

	octreeCube.isLeaf = true;
	cubes.push_back(octreeCube);

	//CUBE 4
	octreeCube.topLeft.x = (cubes[cube].topRight.x - cubes[cube].topLeft.x) / 2 + cubes[cube].topLeft.x;
	octreeCube.topLeft.y = (cubes[cube].topLeft.y - cubes[cube].bottomLeft.y) / 2 + cubes[cube].bottomLeft.y;

	octreeCube.topRight.x = cubes[cube].topRight.x;
	octreeCube.topRight.y = (cubes[cube].topRight.y - cubes[cube].bottomRight.y) / 2 + cubes[cube].bottomRight.y;

	octreeCube.bottomLeft.x = (cubes[cube].bottomRight.x - cubes[cube].bottomLeft.x) / 2 + cubes[cube].bottomLeft.x;
	octreeCube.bottomLeft.y = cubes[cube].bottomLeft.y;

	octreeCube.bottomRight = cubes[cube].bottomRight;


	octreeCube.isLeaf = true;
	cubes.push_back(octreeCube);

	*/



	//OctreeCube subdivs[8];
	OctreeCube cube;
	std::vector <OctreeCube> subdivs;
	for (int i = 0; i < 8; i++){
		subdivs.push_back(cube);
	}


	
	for (int i = 0; i < 8; i++){
		
		subdivs[i].isLeaf = true;
		subdivs[i].sizeX = sizeX / 2;
		subdivs[i].sizeY = sizeY / 2;
		subdivs[i].sizeZ = sizeZ / 2;
	}
	

	//Cube 1
	subdivs[0].origin.x = origin.x;
	subdivs[0].origin.y = origin.y + sizeY / 2;
	subdivs[0].origin.z = origin.z;
	//cubes.push_back(subdivs[0]);
	//Cube 2
	subdivs[1].origin.x = origin.x + sizeX / 2;
	subdivs[1].origin.y = origin.y + sizeY / 2;
	subdivs[1].origin.z = origin.z;
	//cubes.push_back(subdivs[1]);
	//Cube 3
	subdivs[2].origin.x = origin.x + sizeX / 2;
	subdivs[2].origin.y = origin.y;
	subdivs[2].origin.z = origin.z;
	//cubes.push_back(subdivs[2]);
	//Cube 4
	subdivs[3].origin.x = origin.x;
	subdivs[3].origin.y = origin.y;
	subdivs[3].origin.z = origin.z;
	//cubes.push_back(subdivs[3]);

	//Cube 5
	subdivs[4].origin.x = origin.x;
	subdivs[4].origin.y = origin.y + sizeY / 2;
	subdivs[4].origin.z = origin.z + sizeZ / 2;
	//cubes.push_back(subdivs[4]);
	//Cube 6
	subdivs[5].origin.x = origin.x + sizeX / 2;
	subdivs[5].origin.y = origin.y + sizeY / 2;
	subdivs[5].origin.z = origin.z + sizeZ / 2;
	//cubes.push_back(subdivs[5]);
	//Cube 7
	subdivs[6].origin.x = origin.x + sizeX / 2;
	subdivs[6].origin.y = origin.y;
	subdivs[6].origin.z = origin.z + sizeZ / 2;
	//cubes.push_back(subdivs[6]);
	//Cube 8
	subdivs[7].origin.x = origin.x;
	subdivs[7].origin.y = origin.y;
	subdivs[7].origin.z = origin.z + sizeZ / 2;
	//cubes.push_back(subdivs[7]);

	
	cubes.insert(cubes.end(), subdivs.begin(), subdivs.end());


	//////////////



	return 0;
}