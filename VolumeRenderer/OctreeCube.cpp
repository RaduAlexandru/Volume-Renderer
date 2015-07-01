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

int OctreeCube::needsSubdivision(int tolerance, std::vector< boost::unordered_map< std::pair<int, int>, glm::vec3> > &gradient){
	/*int needSubdivision;
	//needSubdivision = rand() % 2;
	needSubdivision = 1;
	return needSubdivision;*/

	glm::vec3 firstAngle(-1, -1, -1);
	
	boost::unordered_map< std::pair<int, int>, glm::vec3>::iterator it;

	for (int k = origin.z; k <sizeZ +origin.z; k = k + 1){
		for (int i = origin.y; i < sizeY + origin.y; i = i + 1){
			for (int j = origin.x; j < sizeX + origin.x; j = j + 1){






				glm::vec3 actualValue(-1, -1, -1);
				it = gradient[k].find(std::make_pair(j, i));
				if (it !=gradient[k].end())
				{
					//element found;
					actualValue = it->second;
				}
				//actualValue = model->gradient[k].find(std::make_pair(j,i));


				if (firstAngle == glm::vec3(-1, -1, -1) && actualValue != glm::vec3(-1, -1, -1)){
					firstAngle = actualValue;

				}

				/*if (firstAngle.x == -1 && firstAngle.y == -1 && firstAngle.z == -1 && actualValue.x != -1 && actualValue.y != -1 && actualValue.z != -1){
				firstAngle.x = actualValue.x;
				firstAngle.y = actualValue.y;
				firstAngle.z = actualValue.z;
				}*/

				//std::cout << "i= " << i << " j= " << j << " k= " << k << std::endl;
				//if (cubesSubdivided < 2 && firstAngleGiven==1)
				//outputFile << " " << firstAngle.x << " " << firstAngle.y << " " << firstAngle.z << " -- " << actualValue.x << " " << actualValue.y << " " << actualValue.z << std::endl;


				int angleDif;
				angleDif = 180 - abs(abs(firstAngle.x - actualValue.x) - 180);

				if (angleDif > tolerance && firstAngle != glm::vec3(-1, -1, -1) && actualValue != glm::vec3(-1, -1, -1)){
					//cout << "Subdivide because X" << endl;
					//return (int)rand % 2;
					/*if (cubesSubdivided < 2 && firstAngleGiven == 1)
					outputFile << "cube finished with YES subdivision" << endl;
					cubesSubdivided++;*/
					return true;
				}
				angleDif = 180 - abs(abs(firstAngle.y - actualValue.y) - 180);
				if (angleDif > tolerance && firstAngle != glm::vec3(-1, -1, -1) && actualValue != glm::vec3(-1, -1, -1)){
					//cout << "Subdivide because Y" << endl;
					return true;
				}
				angleDif = 180 - abs(abs(firstAngle.z - actualValue.z) - 180);
				if (angleDif > tolerance && firstAngle != glm::vec3(-1, -1, -1) && actualValue != glm::vec3(-1, -1, -1)){
					//cout << "Subdivide because Z" << endl;
					return true;
				}


				/*if (abs(firstAngle.x - actualValue.x) > tolerance && firstAngle.x != -1 && firstAngle.y != -1 && firstAngle.z != -1){
				return true;
				}
				if (abs(firstAngle.y - actualValue.y) > tolerance && firstAngle.x != -1 && firstAngle.y != -1 && firstAngle.z != -1){
				return true;
				}
				if (abs(firstAngle.y - actualValue.y) > tolerance && firstAngle.x != -1 && firstAngle.y != -1 && firstAngle.z != -1){
				return true;
				}*/

			}
		}
	}
	//outputFile << "cube finished with NO subdivision" << endl;
	//cube.isLeaf=false;	//If it doesnt need subdivision we just mark it as a non-leaf (as a father of no cubes) just so we don't recheck it later 
	needsChecking = false;
	return false;

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

int OctreeCube::subdivide_tree_version(){




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

	*/



	//THe new one and the correct one for the locational codes is this one

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




	//OctreeCube subdivs[8];
	
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
	


	//cubes.insert(cubes.end(), subdivs.begin(), subdivs.end());


	//////////////



	return 0;
}