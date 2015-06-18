#include "AdaptiveCuber.h"
#include "OctreeCube.h"
#include <math.h>
#include <queue>
#define PI 3.14159265
using namespace std;

AdaptiveCuber::AdaptiveCuber()
{
}


AdaptiveCuber::~AdaptiveCuber()
{
}

AdaptiveCuber::AdaptiveCuber(PixelData* pixelData, std::vector<glm::vec3>* verts, std::vector<glm::vec3>* normals, int isoLevel, int cellSizeX, int cellSizeY, int cellSizeZ,  int interpolateDepth, int octreeMaxDepth, std::vector< boost::unordered_map< std::pair<int, int>, glm::vec3> >* gradient, int tolerance):
MarchingCuber(pixelData,  verts, normals,  isoLevel,  cellSizeX,  cellSizeY,  cellSizeZ,  interpolateDepth) {

	this->octreeMaxDepth = octreeMaxDepth;
	this->gradient = gradient;
	this->tolerance = tolerance;

}


int AdaptiveCuber::run(){
	verts->clear();
	normals->clear();
	std::cout << "Starting adaptive marching cubes 3" << std::endl;
	//Calculate the gradients
	//Create the original cube
	/*
	7---------6
	size Z
	/			  !
	3---------2	  5
	!
	size Y
	!
	origin----size X
	*/


	int cubesSize = 0, depth = 0, maxDepth = octreeMaxDepth;

	if (gradient->size() == 0 || gradient->empty())
		calculateGradient();
	cout << "Finished calculating gradients" << endl;


	OctreeCube* initial = new OctreeCube;
	OctreeCube returnedFromCreateInitial;
	returnedFromCreateInitial = createInitialCube();
	initial->origin.x = returnedFromCreateInitial.origin.x;
	initial->origin.y = returnedFromCreateInitial.origin.y;
	initial->origin.z = returnedFromCreateInitial.origin.z;

	initial->sizeX = returnedFromCreateInitial.sizeX;
	initial->sizeY = returnedFromCreateInitial.sizeY;
	initial->sizeZ = returnedFromCreateInitial.sizeZ;

	octreeVector.push_back(initial);
	std::cout << "starting to generate octree" << std::endl;
	generateOctree_tree_version(*initial);
	std::cout << "finished generateing octree" << std::endl;

	correctlyAssignLeafs(initial);

	//Grab each octreebube in our octree and polygonise it
	//polygoniseOctree(initial);
	polygoniseOctree2(initial);	//Polygonise octree will polygonise each octree but assign the points to the cube itself an not to the model->verts


	std::cout << "starting patch cracks" << std::endl;
	crackPatch(initial);
	//crackPatch(initial);
	//crackPatch(initial);
	std::cout << "finished patching cracks" << std::endl;

	readPointsFromOctree(initial);





	//boost::thread workerThread(boost::bind(&VolumeRenderer::generateNormals, this));
	//model->generatingMesh = false;

	octreeVector.clear();
	return 0;

}



void AdaptiveCuber::calculateGradient(){


	//Sobel kernel on page 15 of http://www.diva-portal.se/smash/get/diva2:515510/FULLTEXT01.pdf

	unsigned char* dataPointer;
	int value = 0;
	int dx = 0, dy = 0, dz = 0, magnitude = 0, left = 0, right = 0, top = 0, bottom = 0, closev = 0, farv = 0, center = 0;
	float angle = -1;
	glm::vec3  gradientAtPoint;


	//std::ofstream outputFile;
	//outputFile.open("GradientAtFrame100.txt");

	/*if (!model->gradient.empty())
	return;*/


	//Create as many gradients frames as the number of frames in the dicom file
	gradient->resize(frames);
	if (gradient->size() == 0)
		return;





	//Here i is y axis and j is the x axis
	for (int i = 0; i < pixelDataHeight; i = i + 1){
		//if (i % 10 == 0)
			//emit progressValueChangedSignal(i * 100 / model->pixelDataHeight);
		for (int j = 0; j < pixelDataWidth; j = j + 1){
			for (int k = 0; k < frames; k = k + 1){			//You need to check why it doesnt work without the -1 in the frames

				if (i == 0 || i == pixelDataHeight - 1 || j == 0 || j == pixelDataWidth - 1 || k == 0 || k == frames - 1){		//If we are in a border we just put it at 0
					//model->gradient[k].push_back(glm::vec3(-1,-1,-1));
					//model->gradient[k].insert((j,i), (-1, -1, -1));
					continue;
				}

				//std::cout << "values are" << i  << "  "<< j << "  " << k  << std::endl;




				//Now we take the values
				dataPointer = &((pixelData->data)[k][0]);
				dataPointer = dataPointer + (j + i*pixelDataWidth)*pointerOffset;
				memcpy(&center, dataPointer, pointerOffset);

				/*if (abs(center - model->isoLevel)>500)
				continue;*/


				dataPointer = &((pixelData->data)[k][0]);
				dataPointer = dataPointer + (j - 1 + i*pixelDataWidth)*pointerOffset;
				memcpy(&left, dataPointer, pointerOffset);

				dataPointer = &((pixelData->data)[k][0]);
				dataPointer = dataPointer + (j + 1 + i*pixelDataWidth)*pointerOffset;
				memcpy(&right, dataPointer, pointerOffset);

				dataPointer = &((pixelData->data)[k][0]);
				dataPointer = dataPointer + (j + (i - 1)*pixelDataWidth)*pointerOffset;
				memcpy(&top, dataPointer, pointerOffset);

				dataPointer = &((pixelData->data)[k][0]);
				dataPointer = dataPointer + (j + (i + 1)*pixelDataWidth)*pointerOffset;
				memcpy(&bottom, dataPointer, pointerOffset);

				dataPointer = &((pixelData->data)[k - 1][0]);
				dataPointer = dataPointer + (j + i*pixelDataWidth)*pointerOffset;
				memcpy(&closev, dataPointer, pointerOffset);

				dataPointer = &((pixelData->data)[k + 1][0]);
				dataPointer = dataPointer + (j + i*pixelDataWidth)*pointerOffset;
				memcpy(&farv, dataPointer, pointerOffset);


				if (center < isoLevel && left < isoLevel && right < isoLevel &&top <isoLevel && bottom < isoLevel&& farv < isoLevel&&closev < isoLevel)
					continue;
				if (center > isoLevel && left > isoLevel && right > isoLevel &&top > isoLevel && bottom > isoLevel&& farv >isoLevel&&closev > isoLevel)
					continue;
				/*dy = left - right;
				dx = top - bottom;
				dz = closev - farv;*/

				dx = -1 * (pixelData->getPixelValue(j - 1, i + 1, k - 1)) + 1 * (pixelData->getPixelValue(j + 1, i + 1, k - 1)) -
					2 * (pixelData->getPixelValue(j - 1, i, k - 1)) + 2 * (pixelData->getPixelValue(j + 1, i, k - 1)) -
					1 * (pixelData->getPixelValue(j - 1, i - 1, k - 1)) + 1 * (pixelData->getPixelValue(j + 1, i - 1, k - 1)) -

					2 * (pixelData->getPixelValue(j - 1, i + 1, k)) + 2 * (pixelData->getPixelValue(j + 1, i + 1, k)) -
					4 * (pixelData->getPixelValue(j - 1, i, k)) + 2 * (pixelData->getPixelValue(j + 1, i, k)) -
					2 * (pixelData->getPixelValue(j - 1, i - 1, k)) + 2 * (pixelData->getPixelValue(j + 1, i - 1, k)) -

					1 * (pixelData->getPixelValue(j - 1, i + 1, k + 1)) + 1 * (pixelData->getPixelValue(j + 1, i + 1, k + 1)) -
					2 * (pixelData->getPixelValue(j - 1, i, k + 1)) + 2 * (pixelData->getPixelValue(j + 1, i, k + 1)) -
					1 * (pixelData->getPixelValue(j - 1, i - 1, k + 1)) + 1 * (pixelData->getPixelValue(j + 1, i - 1, k + 1));

				dy = 1 * (pixelData->getPixelValue(j - 1, i + 1, k - 1)) + 2 * (pixelData->getPixelValue(j, i + 1, k - 1)) + 1 * (pixelData->getPixelValue(j + 1, i + 1, k - 1)) -
					1 * (pixelData->getPixelValue(j - 1, i - 1, k - 1)) - 2 * (pixelData->getPixelValue(j, i - 1, k - 1)) - 1 * (pixelData->getPixelValue(j + 1, i - 1, k - 1)) +

					2 * (pixelData->getPixelValue(j - 1, i + 1, k)) + 4 * (pixelData->getPixelValue(j, i + 1, k)) + 2 * (pixelData->getPixelValue(j + 1, i + 1, k)) -
					2 * (pixelData->getPixelValue(j - 1, i - 1, k)) - 4 * (pixelData->getPixelValue(j, i - 1, k)) - 2 * (pixelData->getPixelValue(j + 1, i - 1, k)) +

					1 * (pixelData->getPixelValue(j - 1, i + 1, k + 1)) + 2 * (pixelData->getPixelValue(j, i + 1, k + 1)) + 1 * (pixelData->getPixelValue(j + 1, i + 1, k + 1)) -
					1 * (pixelData->getPixelValue(j - 1, i - 1, k + 1)) - 2 * (pixelData->getPixelValue(j, i - 1, k + 1)) - 1 * (pixelData->getPixelValue(j + 1, i - 1, k + 1));

				dz = -1 * (pixelData->getPixelValue(j - 1, i + 1, k - 1)) - 2 * (pixelData->getPixelValue(j, i + 1, k - 1)) - 1 * (pixelData->getPixelValue(j + 1, i + 1, k - 1)) -
					2 * (pixelData->getPixelValue(j - 1, i, k - 1)) - 4 * (pixelData->getPixelValue(j, i, k - 1)) - 2 * (pixelData->getPixelValue(j + 1, i, k - 1)) -
					1 * (pixelData->getPixelValue(j - 1, i - 1, k - 1)) - 2 * (pixelData->getPixelValue(j, i - 1, k - 1)) - 1 * (pixelData->getPixelValue(j + 1, i - 1, k - 1)) +

					1 * (pixelData->getPixelValue(j - 1, i + 1, k + 1)) + 2 * (pixelData->getPixelValue(j, i + 1, k + 1)) + 1 * (pixelData->getPixelValue(j + 1, i + 1, k + 1)) -
					2 * (pixelData->getPixelValue(j - 1, i, k + 1)) + 4 * (pixelData->getPixelValue(j, i, k + 1)) + 2 * (pixelData->getPixelValue(j + 1, i, k + 1)) -
					1 * (pixelData->getPixelValue(j - 1, i - 1, k + 1)) + 2 * (pixelData->getPixelValue(j, i - 1, k + 1)) + 1 * (pixelData->getPixelValue(j + 1, i - 1, k + 1));




				/*dx = -1 * (model->getSmoothPixelValue(j - 1, i + 1, k - 1)) + 1 * (model->getSmoothPixelValue(j + 1, i + 1, k - 1)) -
				2 * (model->getSmoothPixelValue(j - 1, i, k - 1)) + 2 * (model->getSmoothPixelValue(j + 1, i, k - 1)) -
				1 * (model->getSmoothPixelValue(j - 1, i - 1, k - 1)) + 1 * (model->getSmoothPixelValue(j + 1, i - 1, k - 1)) -

				2 * (model->getSmoothPixelValue(j - 1, i + 1, k)) + 2 * (model->getSmoothPixelValue(j + 1, i + 1, k)) -
				4 * (model->getSmoothPixelValue(j - 1, i, k)) + 2 * (model->getSmoothPixelValue(j + 1, i, k)) -
				2 * (model->getSmoothPixelValue(j - 1, i - 1, k)) + 2 * (model->getSmoothPixelValue(j + 1, i - 1, k)) -

				1 * (model->getSmoothPixelValue(j - 1, i + 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, i + 1, k + 1)) -
				2 * (model->getSmoothPixelValue(j - 1, i, k + 1)) + 2 * (model->getSmoothPixelValue(j + 1, i, k + 1)) -
				1 * (model->getSmoothPixelValue(j - 1, i - 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, i - 1, k + 1));

				dy = 1 * (model->getSmoothPixelValue(j - 1, i + 1, k - 1)) + 2 * (model->getSmoothPixelValue(j, i + 1, k - 1)) + 1 * (model->getSmoothPixelValue(j + 1, i + 1, k - 1)) -
				1 * (model->getSmoothPixelValue(j - 1, i - 1, k - 1)) - 2 * (model->getSmoothPixelValue(j, i - 1, k - 1)) - 1 * (model->getSmoothPixelValue(j + 1, i - 1, k - 1)) +

				2 * (model->getSmoothPixelValue(j - 1, i + 1, k)) + 4 * (model->getSmoothPixelValue(j, i + 1, k)) + 2 * (model->getSmoothPixelValue(j + 1, i + 1, k)) -
				2 * (model->getSmoothPixelValue(j - 1, i - 1, k)) - 4 * (model->getSmoothPixelValue(j, i - 1, k)) - 2 * (model->getSmoothPixelValue(j + 1, i - 1, k)) +

				1 * (model->getSmoothPixelValue(j - 1, i + 1, k + 1)) + 2 * (model->getSmoothPixelValue(j, i + 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, i + 1, k + 1)) -
				1 * (model->getSmoothPixelValue(j - 1, i - 1, k + 1)) - 2 * (model->getSmoothPixelValue(j, i - 1, k + 1)) - 1 * (model->getSmoothPixelValue(j + 1, i - 1, k + 1));

				dz = -1 * (model->getSmoothPixelValue(j - 1, i + 1, k - 1)) - 2 * (model->getSmoothPixelValue(j, i + 1, k - 1)) - 1 * (model->getSmoothPixelValue(j + 1, i + 1, k - 1)) -
				2 * (model->getSmoothPixelValue(j - 1, i, k - 1)) - 4 * (model->getSmoothPixelValue(j, i, k - 1)) - 2 * (model->getSmoothPixelValue(j + 1, i, k - 1)) -
				1 * (model->getSmoothPixelValue(j - 1, i - 1, k - 1)) - 2 * (model->getSmoothPixelValue(j, i - 1, k - 1)) - 1 * (model->getSmoothPixelValue(j + 1, i - 1, k - 1)) +

				1 * (model->getSmoothPixelValue(j - 1, i + 1, k + 1)) + 2 * (model->getSmoothPixelValue(j, i + 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, i + 1, k + 1)) -
				2 * (model->getSmoothPixelValue(j - 1, i, k + 1)) + 4 * (model->getSmoothPixelValue(j, i, k + 1)) + 2 * (model->getSmoothPixelValue(j + 1, i, k + 1)) -
				1 * (model->getSmoothPixelValue(j - 1, i - 1, k + 1)) + 2 * (model->getSmoothPixelValue(j, i - 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, i - 1, k + 1));
				*/

				if (dx == 0)
					dx = 1;
				if (dy == 0)
					dy = 1;
				if (dz == 0)
					dz = 1;

				/*THe 3 possible axis
				dx-dy
				dx-dz
				dy-dz
				*/


				magnitude = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
				if (magnitude < 2500)
					continue;
				gradientAtPoint.x = atan2(dy, dx) * 180 / PI + 180;
				gradientAtPoint.y = atan2(dx, dz) * 180 / PI + 180;
				gradientAtPoint.z = atan2(dz, dy) * 180 / PI + 180;

				/*gradientAtPoint.x = (gradientAtPoint.x > 0.0 ? gradientAtPoint.x : (360.0 + gradientAtPoint.x));
				gradientAtPoint.y = (gradientAtPoint.y > 0.0 ? gradientAtPoint.y : (360.0 + gradientAtPoint.y));
				gradientAtPoint.z = (gradientAtPoint.z > 0.0 ? gradientAtPoint.z : (360.0 + gradientAtPoint.z));*/

				gradient->at(k)[std::make_pair(j, i)] = gradientAtPoint;
				/*model->gradientPoints.push_back(j);
				model->gradientPoints.push_back(i);
				model->gradientPoints.push_back(k);*/
				//model->m[k].push_back(j,i,gradientAtPoint) ;



				/*if (k == 100){
				outputFile << " Values are "  << "Magnitude "  << magnitude << "Angles " << gradientAtPoint.x << " " << gradientAtPoint.y << " " << gradientAtPoint.z << endl;
				}*/



			}

		}
	}
}


OctreeCube AdaptiveCuber::createInitialCube(){
	OctreeCube cube;
	cube.origin.x = 0;
	cube.origin.y = 0;
	cube.origin.z = 0;


	int cubeSize = std::max(std::max(pixelDataWidth, pixelDataHeight), frames - 1);	//WE get the maximum value
	cubeSize = pow(2, ceil(log(cubeSize) / log(2)));	// we round it up to the neerest power of 2

	cube.sizeX = cubeSize;
	cube.sizeY = cubeSize;
	cube.sizeZ = cubeSize;

	//model->cubes.push_back(cube);
	return cube;
}


int AdaptiveCuber::generateOctree_tree_version(OctreeCube& currentCube, int currentDepth){




	//Grab the initial one and see if it needs subdivision and if we are also below the maximum octree depth, if it needs subdiviosn, sundivid it, and also recursivelly call the same function for the children

	if (currentDepth < octreeMaxDepth && cubeNeedsSubdivision(currentCube) && currentCube.needsChecking){
		//std::cout << "The cube doesnt have children now" << std::endl;
		currentCube.subdivide_tree_version();
		//std::cout << "The cube should have children now" << std::endl;

		octreeVector.push_back(currentCube.children[0]);
		octreeVector.push_back(currentCube.children[1]);
		octreeVector.push_back(currentCube.children[2]);
		octreeVector.push_back(currentCube.children[3]);
		octreeVector.push_back(currentCube.children[4]);
		octreeVector.push_back(currentCube.children[5]);
		octreeVector.push_back(currentCube.children[6]);
		octreeVector.push_back(currentCube.children[7]);

		generateOctree_tree_version(*(currentCube.children[0]), currentDepth + 1);
		generateOctree_tree_version(*(currentCube.children[1]), currentDepth + 1);
		generateOctree_tree_version(*(currentCube.children[2]), currentDepth + 1);
		generateOctree_tree_version(*(currentCube.children[3]), currentDepth + 1);
		generateOctree_tree_version(*(currentCube.children[4]), currentDepth + 1);
		generateOctree_tree_version(*(currentCube.children[5]), currentDepth + 1);
		generateOctree_tree_version(*(currentCube.children[6]), currentDepth + 1);
		generateOctree_tree_version(*(currentCube.children[7]), currentDepth + 1);
	}
	else{
		currentCube.isLeaf = true;	// we need to check if this value is set correctly
		currentCube.needsChecking = false;
		return 0;
	}
	return 0;
}


inline bool AdaptiveCuber::cubeNeedsSubdivision(OctreeCube &cube){


	//return (int)rand % 2;

	//std::ofstream outputFile;
	//outputFile.open("GradientsFromSubdivision1.txt");


	glm::vec3 firstAngle(-1, -1, -1);
	boost::unordered_map< std::pair<int, int>, glm::vec3>::iterator it;



	//We first check if the cube is totally out of the volume
	if (cube.origin.x > pixelDataWidth || cube.origin.y > pixelDataHeight || cube.origin.z >frames - 1){
		cube.needsChecking = false;
		cube.isLeaf = true;
		return false;
	}

	//Then we check if it is in the border between the actual volume and the initial cube in which case we need to subdivide
	if (cube.origin.x + cube.sizeX > pixelDataWidth || cube.origin.y + cube.sizeY > pixelDataHeight || cube.origin.z + cube.sizeZ > frames - 1){
		cube.needsChecking = true;
		return true;
	}

	//From now on all the cubes will be totally within the volume


	for (int k = cube.origin.z; k < cube.sizeZ + cube.origin.z; k = k + 1){
		for (int i = cube.origin.y; i < cube.sizeY + cube.origin.y; i = i + 1){
			for (int j = cube.origin.x; j < cube.sizeX + cube.origin.x; j = j + 1){






				glm::vec3 actualValue(-1, -1, -1);
				it = gradient->at(k).find(std::make_pair(j, i));
				if (it != gradient->at(k).end())
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



				//Now I will try to make a different way of seeing diference between angles. This time by looking at all the angles at the same time, and seeing the total magnitude of change in angles
				int angleDifX = 180 - abs(abs(firstAngle.x - actualValue.x) - 180);
				int angleDifY = 180 - abs(abs(firstAngle.y - actualValue.y) - 180);
				int angleDifZ = 180 - abs(abs(firstAngle.z - actualValue.z) - 180);

				int angleMagnitude = sqrt(angleDifX*angleDifX + angleDifY*angleDifY + angleDifZ*angleDifZ);
				if (angleMagnitude > tolerance && firstAngle != glm::vec3(-1, -1, -1) && actualValue != glm::vec3(-1, -1, -1))
					return true;




				/*int angleDif;
				angleDif = 180 - abs(abs(firstAngle.x - actualValue.x) - 180);


				if (angleDif > tolerance && firstAngle != glm::vec3(-1, -1, -1) && actualValue != glm::vec3(-1, -1, -1)){
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
				}*/


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
	cube.needsChecking = false;
	return false;
}


void AdaptiveCuber::correctlyAssignLeafs(OctreeCube* root){
	OctreeCube* currentCube;
	std::queue<OctreeCube*> queue;
	queue.push(root);
	while (!queue.empty()){
		currentCube = queue.front();
		queue.pop();
		if (currentCube->children[0] != NULL){
			queue.push(currentCube->children[0]);
			queue.push(currentCube->children[1]);
			queue.push(currentCube->children[2]);
			queue.push(currentCube->children[3]);
			queue.push(currentCube->children[4]);
			queue.push(currentCube->children[5]);
			queue.push(currentCube->children[6]);
			queue.push(currentCube->children[7]);
		}


		if (currentCube->children[0] == NULL){
			currentCube->isLeaf = true;
		}
		else{
			currentCube->isLeaf = false;
		}
	}

}

/*Not being used*/
int AdaptiveCuber::polygoniseOctree(OctreeCube* currentCube, int currentDepth){

	//Recursive alrogithm, we go to all the children of current cube and if the children is null, we go back in the recursive stack and polygonise the current cube
	/*
	if (currentCube != NULL){
		polygoniseOctree((currentCube->children[0]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)  //We check if the current cube is a left (children is null) and has not been polygonised yet
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
		polygoniseOctree((currentCube->children[1]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
		polygoniseOctree((currentCube->children[2]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
		polygoniseOctree((currentCube->children[3]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
		polygoniseOctree((currentCube->children[4]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
		polygoniseOctree((currentCube->children[5]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
		polygoniseOctree((currentCube->children[6]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
		polygoniseOctree((currentCube->children[7]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
	}
	else{
		return 0;
	}
	*/

	return 0;

}



int AdaptiveCuber::polygoniseOctree2(OctreeCube* root){

	OctreeCube* currentCube;
	std::queue<OctreeCube*> queue;
	queue.push(root);
	while (!queue.empty()){
		currentCube = queue.front();
		queue.pop();
		if (currentCube->children[0] != NULL){
			queue.push(currentCube->children[0]);
			queue.push(currentCube->children[1]);
			queue.push(currentCube->children[2]);
			queue.push(currentCube->children[3]);
			queue.push(currentCube->children[4]);
			queue.push(currentCube->children[5]);
			queue.push(currentCube->children[6]);
			queue.push(currentCube->children[7]);
		}

		//Grab the current cube and polygonise it f it's leaf
		if (currentCube->isLeaf){
			polygoniseAssignToCube(currentCube);
		}

	}
	return 0;
}


int AdaptiveCuber::polygoniseAssignToCube(OctreeCube* currentCube){



	//Create the cell
	CELL cell;

	for (int j = 0; j < 8; j++){
		cell.position[j].x = 0;
		cell.position[j].x = 0;
		cell.position[j].x = 0;
		cell.val[j] = 0;
	}

	cell.position[0].x = currentCube->origin.x;
	cell.position[0].y = currentCube->origin.y;
	cell.position[0].z = currentCube->origin.z;
	cell.val[0] = pixelData->getPixelValue(currentCube->origin.x, currentCube->origin.y, currentCube->origin.z);

	cell.position[1].x = currentCube->origin.x + currentCube->sizeX;
	cell.position[1].y = currentCube->origin.y;
	cell.position[1].z = currentCube->origin.z;
	cell.val[1] = pixelData->getPixelValue(currentCube->origin.x + currentCube->sizeX, currentCube->origin.y, currentCube->origin.z);

	cell.position[2].x = currentCube->origin.x + currentCube->sizeX;
	cell.position[2].y = currentCube->origin.y + currentCube->sizeY;
	cell.position[2].z = currentCube->origin.z;
	cell.val[2] = pixelData->getPixelValue(currentCube->origin.x + currentCube->sizeX, currentCube->origin.y + currentCube->sizeY, currentCube->origin.z);

	cell.position[3].x = currentCube->origin.x;
	cell.position[3].y = currentCube->origin.y + currentCube->sizeY;
	cell.position[3].z = currentCube->origin.z;
	cell.val[3] = pixelData->getPixelValue(currentCube->origin.x, currentCube->origin.y + currentCube->sizeY, currentCube->origin.z);
	//////
	cell.position[4].x = currentCube->origin.x;
	cell.position[4].y = currentCube->origin.y;
	cell.position[4].z = currentCube->origin.z + currentCube->sizeZ;
	cell.val[4] = pixelData->getPixelValue(currentCube->origin.x, currentCube->origin.y, currentCube->origin.z + currentCube->sizeZ);

	cell.position[5].x = currentCube->origin.x + currentCube->sizeX;
	cell.position[5].y = currentCube->origin.y;
	cell.position[5].z = currentCube->origin.z + currentCube->sizeZ;
	cell.val[5] = pixelData->getPixelValue(currentCube->origin.x + currentCube->sizeX, currentCube->origin.y, currentCube->origin.z + currentCube->sizeZ);

	cell.position[6].x = currentCube->origin.x + currentCube->sizeX;
	cell.position[6].y = currentCube->origin.y + currentCube->sizeY;
	cell.position[6].z = currentCube->origin.z + currentCube->sizeZ;
	cell.val[6] = pixelData->getPixelValue(currentCube->origin.x + currentCube->sizeX, currentCube->origin.y + currentCube->sizeY, currentCube->origin.z + currentCube->sizeZ);

	cell.position[7].x = currentCube->origin.x;
	cell.position[7].y = currentCube->origin.y + currentCube->sizeY;
	cell.position[7].z = currentCube->origin.z + currentCube->sizeZ;
	cell.val[7] = pixelData->getPixelValue(currentCube->origin.x, currentCube->origin.y + currentCube->sizeY, currentCube->origin.z + currentCube->sizeZ);


	//Now we have the cell for the cube created, so we polygonise it


	glm::vec3 vertlist[12];


	int cubeIndex = 0;

	if (cell.val[0] > isoLevel) cubeIndex += 1;
	if (cell.val[1] > isoLevel) cubeIndex += 2;
	if (cell.val[2] > isoLevel) cubeIndex += 4;
	if (cell.val[3] > isoLevel) cubeIndex += 8;
	if (cell.val[4] > isoLevel) cubeIndex += 16;
	if (cell.val[5] > isoLevel) cubeIndex += 32;
	if (cell.val[6] > isoLevel) cubeIndex += 64;
	if (cell.val[7] > isoLevel) cubeIndex += 128;


	if (cubeIndex == 255 || cubeIndex == 0){
		return 0;	//REturn a 0 to indicate that the cell doesnt contain any vertices
	}

	glm::vec3 point;

	/* Find the vertices where the surface intersects the cube */
	if (edgeTable[cubeIndex] & 1) {
		interpolate(isoLevel, cell.position[0], cell.position[1], cell.val[0], cell.val[1], point);
		vertlist[0] = point;
	}
	if (edgeTable[cubeIndex] & 2) {
		interpolate(isoLevel, cell.position[1], cell.position[2], cell.val[1], cell.val[2], point);
		vertlist[1] = point;
	}
	if (edgeTable[cubeIndex] & 4) {
		interpolate(isoLevel, cell.position[2], cell.position[3], cell.val[2], cell.val[3], point);
		vertlist[2] = point;
	}
	if (edgeTable[cubeIndex] & 8) {
		interpolate(isoLevel, cell.position[3], cell.position[0], cell.val[3], cell.val[0], point);
		vertlist[3] = point;

	}
	if (edgeTable[cubeIndex] & 16) {
		interpolate(isoLevel, cell.position[4], cell.position[5], cell.val[4], cell.val[5], point);
		vertlist[4] = point;
	}
	if (edgeTable[cubeIndex] & 32) {
		interpolate(isoLevel, cell.position[5], cell.position[6], cell.val[5], cell.val[6], point);
		vertlist[5] = point;
	}
	if (edgeTable[cubeIndex] & 64) {
		interpolate(isoLevel, cell.position[6], cell.position[7], cell.val[6], cell.val[7], point);
		vertlist[6] = point;
	}
	if (edgeTable[cubeIndex] & 128) {
		interpolate(isoLevel, cell.position[7], cell.position[4], cell.val[7], cell.val[4], point);
		vertlist[7] = point;
	}
	if (edgeTable[cubeIndex] & 256) {
		interpolate(isoLevel, cell.position[0], cell.position[4], cell.val[0], cell.val[4], point);
		vertlist[8] = point;
	}
	if (edgeTable[cubeIndex] & 512) {
		interpolate(isoLevel, cell.position[1], cell.position[5], cell.val[1], cell.val[5], point);
		vertlist[9] = point;
	}
	if (edgeTable[cubeIndex] & 1024) {
		interpolate(isoLevel, cell.position[2], cell.position[6], cell.val[2], cell.val[6], point);
		vertlist[10] = point;
	}
	if (edgeTable[cubeIndex] & 2048) {
		interpolate(isoLevel, cell.position[3], cell.position[7], cell.val[3], cell.val[7], point);
		vertlist[11] = point;
	}


	glm::vec3 normal;
	unsigned char* dataPointer;
	int value1 = 0, value2 = 0;

	/* Create the triangles */

	//WE create a vector with the points and we assignt it to the cube
	vector<glm::vec3> *points = new vector<glm::vec3>;

	glm::vec3 point1, point2, point3;
	for (int i = 0; triTable[cubeIndex][i] != -1; i += 3) {

		point1.x = (vertlist[triTable[cubeIndex][i]]).x;
		point1.y = (vertlist[triTable[cubeIndex][i]]).y;
		point1.z = (vertlist[triTable[cubeIndex][i]]).z;

		point2.x = (vertlist[triTable[cubeIndex][i + 1]]).x;
		point2.y = (vertlist[triTable[cubeIndex][i + 1]]).y;
		point2.z = (vertlist[triTable[cubeIndex][i + 1]]).z;

		point3.x = (vertlist[triTable[cubeIndex][i + 2]]).x;
		point3.y = (vertlist[triTable[cubeIndex][i + 2]]).y;
		point3.z = (vertlist[triTable[cubeIndex][i + 2]]).z;

		points->push_back(point1);
		points->push_back(point2);
		points->push_back(point3);

		/*verts.push_back(point1);
		verts.push_back(point2);
		verts.push_back(point3);*/
	}

	currentCube->points = points;
	currentCube->containsVerts = true;

}



void AdaptiveCuber::crackPatch(OctreeCube* root){


	OctreeCube* currentCube;
	OctreeCube* adyacent;

	OctreeCube* right, *left, *top, *bottom, *further, *closer;

	int count = 0;
	//Do the same but using the octree vector insted of a queue
	for (int i = 0; i < octreeVector.size(); i++){
		currentCube = octreeVector[i];
		count++;
		right = NULL;
		left = NULL;
		top = NULL;
		bottom = NULL;
		further = NULL;
		closer = NULL;

		if (currentCube->isLeaf && currentCube->containsVerts){
			//std::cout << "We need to check it's adyacents of cube "  << count << std::endl;
			//std::cout << "checking cube  " << count << "of " << model->octreeVector.size() << std::endl;
			for (int j = 0; j < octreeVector.size(); j++){



				//We check if it adyacent is the one on the right, left, top, bottom, further and closer cube
				adyacent =octreeVector[j];

				//Right cube
				if (right == NULL &&

					adyacent->origin.x == currentCube->origin.x + currentCube->sizeX &&
					adyacent->origin.y == currentCube->origin.y &&
					adyacent->origin.z == currentCube->origin.z &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					right = adyacent;
				}

				//Left cube
				if (left == NULL &&

					adyacent->origin.x == currentCube->origin.x - currentCube->sizeX &&
					adyacent->origin.y == currentCube->origin.y &&
					adyacent->origin.z == currentCube->origin.z &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					left = adyacent;
				}

				//Top cube
				if (top == NULL &&

					adyacent->origin.x == currentCube->origin.x &&
					adyacent->origin.y == currentCube->origin.y + currentCube->sizeY &&
					adyacent->origin.z == currentCube->origin.z &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					top = adyacent;
				}

				//bottom cube
				if (bottom == NULL &&

					adyacent->origin.x == currentCube->origin.x &&
					adyacent->origin.y == currentCube->origin.y - currentCube->sizeY &&
					adyacent->origin.z == currentCube->origin.z &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					bottom = adyacent;
				}

				//further cube
				if (further == NULL &&

					adyacent->origin.x == currentCube->origin.x &&
					adyacent->origin.y == currentCube->origin.y &&
					adyacent->origin.z == currentCube->origin.z + currentCube->sizeZ &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					further = adyacent;
				}


				//closer cube
				if (closer == NULL &&

					adyacent->origin.x == currentCube->origin.x &&
					adyacent->origin.y == currentCube->origin.y &&
					adyacent->origin.z == currentCube->origin.z - currentCube->sizeZ &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					closer = adyacent;
				}

				if (right != NULL && left != NULL && top != NULL && bottom != NULL && further != NULL && closer != NULL){
					break;	//WE stop searching because we found all the neihbours
				}


			}















			/*

			if (currentCube->origin.x == 256 && currentCube->origin.y == 168 && currentCube->origin.z == 56 && currentCube->sizeX == 8){
			std::cout << "found cube" << std::endl << std::endl;
			//Grab the points that are on the closer face
			//get rid of the duplicates

			//grab all the  points from the closer cube
			//remove those that are not on the face

			if (closer!=NULL)
			std::cout << "we have a closer cube" << std::endl;


			//we grab the points from the cube that are on the closer face
			std::vector<glm::vec3> pointsOnFace;
			pointsOnFace.clear();
			for (int i = 0; i < currentCube->points->size(); i++){
			if (currentCube->points->at(i).z == (currentCube->origin.z)){
			pointsOnFace.push_back(currentCube->points->at(i));
			}
			}

			for (int i = 0; i < pointsOnFace.size(); i++){
			std::cout << "all point from currentcube on closer face " << pointsOnFace[i].x << "  " << pointsOnFace[i].y << "  " << pointsOnFace[i].z << std::endl;
			}

			//Remove the duplicates from points on face
			std::vector<glm::vec3> pointsOnFaceNoDuplicates;
			bool skipPoint = false;
			for (int i = 0; i < pointsOnFace.size(); i++){
			skipPoint = false;
			//For each point check if it's not already in the pointOnFaceNoDuplicates
			for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
			if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
			//There is already on in pointface no duplicates with the same value
			skipPoint = true;
			}
			}
			if (!skipPoint)
			pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
			}

			//The points from the current cube with no duplicates
			for (int i = 0; i < pointsOnFaceNoDuplicates.size(); i++){
			std::cout << "point from currentcube on closer face " << pointsOnFaceNoDuplicates[i].x << "  " << pointsOnFaceNoDuplicates[i].y << "  " << pointsOnFaceNoDuplicates[i].z << std::endl;
			}



			//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
			if (pointsOnFaceNoDuplicates.size() == 2){
			//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
			std::vector<glm::vec3> pointsOnFace2Old;
			std::vector<glm::vec3> pointsOnFace2New;

			std::queue<OctreeCube*> queue;
			OctreeCube* child;
			queue.push(closer);
			while (!queue.empty()){
			child = queue.front();
			queue.pop();
			if (!child->isLeaf){
			queue.push(child->children[0]);
			queue.push(child->children[1]);
			queue.push(child->children[2]);
			queue.push(child->children[3]);
			queue.push(child->children[4]);
			queue.push(child->children[5]);
			queue.push(child->children[6]);
			queue.push(child->children[7]);
			}
			//Get the points from that child and put them in pointsOnFace2Old
			if (child->points != NULL){
			for (int j = 0; j < child->points->size(); j++){
			if (child->points->at(j).z == currentCube->origin.z){
			pointsOnFace2Old.push_back(child->points->at(j));
			}
			}
			}
			}


			std::cout << "the points from the closer cube on the face" << std::endl;
			for (int i = 0; i < pointsOnFace2Old.size(); i++){
			std::cout << "point from closer cube on closer face " << pointsOnFace2Old[i].x << "  " << pointsOnFace2Old[i].y << "  " << pointsOnFace2Old[i].z << std::endl;
			}



			//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)


			//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
			//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
			//pointsOnFace2New.reserve(pointsOnFace2Old.size());
			for (int i = 0; i < pointsOnFace2Old.size(); i++){
			pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));

			//std::cout << "point 1 " << pointsOnFaceNoDuplicates[0].x << "  " << pointsOnFaceNoDuplicates[0].y << "  " << pointsOnFaceNoDuplicates[0].z << std::endl;
			//std::cout << "point 2 " << pointsOnFaceNoDuplicates[1].x << "  " << pointsOnFaceNoDuplicates[1].y << "  " << pointsOnFaceNoDuplicates[1].z << std::endl;
			//std::cout << "point for projection " << pointsOnFace2Old[i].x << "  " << pointsOnFace2Old[i].y << "  " << pointsOnFace2Old[i].z << std::endl;
			//std::cout << "point projected " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << std::endl;
			//std::cin.get();
			//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
			}


			std::cout  << "the points from the closer cube projected" << std::endl;
			for (int i = 0; i < pointsOnFace2New.size(); i++){
			std::cout << "point from closer cube projected " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << std::endl;
			}


			//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

			std::cout << std::endl << "interchanging old with the new" << std::endl;

			queue.push(closer);
			while (!queue.empty()){
			child = queue.front();
			queue.pop();
			if (!child->isLeaf){
			queue.push(child->children[0]);
			queue.push(child->children[1]);
			queue.push(child->children[2]);
			queue.push(child->children[3]);
			queue.push(child->children[4]);
			queue.push(child->children[5]);
			queue.push(child->children[6]);
			queue.push(child->children[7]);
			}
			if (child->points != NULL){
			for (int j = 0; j < child->points->size(); j++){
			//For each one of those points, check to see if it matches on in pointsOnFace2old, If it does, change it with the point in the same index in pointsOnFace2New
			for (int k = 0; k < pointsOnFace2Old.size(); k++){
			if (child->points->at(j).x == pointsOnFace2Old[k].x && child->points->at(j).y == pointsOnFace2Old[k].y && child->points->at(j).z == pointsOnFace2Old[k].z){

			std::cout << "changing " << pointsOnFace2Old[k].x << "|" << pointsOnFace2Old[k].y << "|" << pointsOnFace2Old[k].z << "|" << " With " <<
			pointsOnFace2New[k].x << "|" << pointsOnFace2New[k].y << "|" << pointsOnFace2New[k].z << "|" << std::endl;


			child->points->at(j).x = pointsOnFace2New[k].x;
			child->points->at(j).y = pointsOnFace2New[k].y;
			child->points->at(j).z = pointsOnFace2New[k].z;
			}
			}

			}
			}
			}


			}




			}

			*/
















			/*if (right != NULL && left != NULL && top != NULL && bottom != NULL && further != NULL && closer != NULL){
			std::cout << "found one with all the neighbours" << std::endl;
			}*/

			//Now we have the current cube and it's neighbour, we need to check and see if there are any conflicts woth the neighbours. A conflict occurs if the current cube is leaf and the neighbour isint

			//The right one is not a leaf, we must check it's chilcren and return their verts. If the children have any verts, check if they are on the correspondant face
			if (right != NULL && !right->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).x == (currentCube->origin.x + currentCube->sizeX)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					skipPoint = false;
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(right);
					while (!queue.empty()){
						child = queue.front();
						queue.pop();
						if (!child->isLeaf){
							queue.push(child->children[0]);
							queue.push(child->children[1]);
							queue.push(child->children[2]);
							queue.push(child->children[3]);
							queue.push(child->children[4]);
							queue.push(child->children[5]);
							queue.push(child->children[6]);
							queue.push(child->children[7]);
						}
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).x != (currentCube->origin.x + currentCube->sizeX)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));

						//std::cout << "point 1 " << pointsOnFaceNoDuplicates[0].x << "  " << pointsOnFaceNoDuplicates[0].y << "  " << pointsOnFaceNoDuplicates[0].z << std::endl;
						//std::cout << "point 2 " << pointsOnFaceNoDuplicates[1].x << "  " << pointsOnFaceNoDuplicates[1].y << "  " << pointsOnFaceNoDuplicates[1].z << std::endl;
						//std::cout << "point for projection " << pointsOnFace2Old[i].x << "  " << pointsOnFace2Old[i].y << "  " << pointsOnFace2Old[i].z << std::endl;
						//std::cout << "point projected " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << std::endl;
						//std::cin.get();

						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(right);
					while (!queue.empty()){
						child = queue.front();
						queue.pop();
						if (!child->isLeaf){
							queue.push(child->children[0]);
							queue.push(child->children[1]);
							queue.push(child->children[2]);
							queue.push(child->children[3]);
							queue.push(child->children[4]);
							queue.push(child->children[5]);
							queue.push(child->children[6]);
							queue.push(child->children[7]);
						}
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								//For each one of those points, check to see if it matches on in pointsOnFace2old, If it does, change it with the point in the same index in pointsOnFace2New
								for (int k = 0; k < pointsOnFace2Old.size(); k++){
									if (child->points->at(j).x == pointsOnFace2Old[k].x && child->points->at(j).y == pointsOnFace2Old[k].y && child->points->at(j).z == pointsOnFace2Old[k].z){
										child->points->at(j).x = pointsOnFace2New[k].x;
										child->points->at(j).y = pointsOnFace2New[k].y;
										child->points->at(j).z = pointsOnFace2New[k].z;
									}
								}

							}
						}
					}


				}




				/*std::cout << "point on the right hace are " << pointsOnFaceNoDuplicates.size() << std::endl;
				if (pointsOnFaceNoDuplicates.size() >= 3){
				std::cout << "cube has sizes " << endl << " origin.x= " << currentCube->origin.x << endl <<
				" origin.y= " << currentCube->origin.y <<
				" origin.z= " << currentCube->origin.z <<
				" sizex= " << currentCube->sizeX <<
				" sizey= " << currentCube->sizeY <<
				" sizez= " << currentCube->sizeZ << endl;
				std::cout << "the vertices are" << endl;
				for (int j = 0; j < currentCube->points->size(); j++){
				std::cout << "  " << currentCube->points->at(j).x << "  " << currentCube->points->at(j).y << "  " << currentCube->points->at(j).z << endl;
				}
				std::cin.get();
				std::cout << endl << endl;
				}*/
			}

















			///////////////////////////////////////////////////////
			if (left != NULL && !left->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).x == (currentCube->origin.x)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					skipPoint = false;
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(left);
					while (!queue.empty()){
						child = queue.front();
						queue.pop();
						if (!child->isLeaf){
							queue.push(child->children[0]);
							queue.push(child->children[1]);
							queue.push(child->children[2]);
							queue.push(child->children[3]);
							queue.push(child->children[4]);
							queue.push(child->children[5]);
							queue.push(child->children[6]);
							queue.push(child->children[7]);
						}
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).x != (currentCube->origin.x)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(left);
					while (!queue.empty()){
						child = queue.front();
						queue.pop();
						if (!child->isLeaf){
							queue.push(child->children[0]);
							queue.push(child->children[1]);
							queue.push(child->children[2]);
							queue.push(child->children[3]);
							queue.push(child->children[4]);
							queue.push(child->children[5]);
							queue.push(child->children[6]);
							queue.push(child->children[7]);
						}
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								//For each one of those points, check to see if it matches on in pointsOnFace2old, If it does, change it with the point in the same index in pointsOnFace2New
								for (int k = 0; k < pointsOnFace2Old.size(); k++){
									if (child->points->at(j).x == pointsOnFace2Old[k].x && child->points->at(j).y == pointsOnFace2Old[k].y && child->points->at(j).z == pointsOnFace2Old[k].z){
										child->points->at(j).x = pointsOnFace2New[k].x;
										child->points->at(j).y = pointsOnFace2New[k].y;
										child->points->at(j).z = pointsOnFace2New[k].z;
									}
								}

							}
						}
					}


				}
			}



			if (top != NULL && !top->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).y == (currentCube->origin.y + currentCube->sizeY)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					skipPoint = false;
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(top);
					while (!queue.empty()){
						child = queue.front();
						queue.pop();
						if (!child->isLeaf){
							queue.push(child->children[0]);
							queue.push(child->children[1]);
							queue.push(child->children[2]);
							queue.push(child->children[3]);
							queue.push(child->children[4]);
							queue.push(child->children[5]);
							queue.push(child->children[6]);
							queue.push(child->children[7]);
						}
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).y != (currentCube->origin.y + currentCube->sizeY)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(top);
					while (!queue.empty()){
						child = queue.front();
						queue.pop();
						if (!child->isLeaf){
							queue.push(child->children[0]);
							queue.push(child->children[1]);
							queue.push(child->children[2]);
							queue.push(child->children[3]);
							queue.push(child->children[4]);
							queue.push(child->children[5]);
							queue.push(child->children[6]);
							queue.push(child->children[7]);
						}
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								//For each one of those points, check to see if it matches on in pointsOnFace2old, If it does, change it with the point in the same index in pointsOnFace2New
								for (int k = 0; k < pointsOnFace2Old.size(); k++){
									if (child->points->at(j).x == pointsOnFace2Old[k].x && child->points->at(j).y == pointsOnFace2Old[k].y && child->points->at(j).z == pointsOnFace2Old[k].z){
										child->points->at(j).x = pointsOnFace2New[k].x;
										child->points->at(j).y = pointsOnFace2New[k].y;
										child->points->at(j).z = pointsOnFace2New[k].z;
									}
								}

							}
						}
					}


				}
			}

			if (bottom != NULL && !bottom->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).y == (currentCube->origin.y)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					skipPoint = false;
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(bottom);
					while (!queue.empty()){
						child = queue.front();
						queue.pop();
						if (!child->isLeaf){
							queue.push(child->children[0]);
							queue.push(child->children[1]);
							queue.push(child->children[2]);
							queue.push(child->children[3]);
							queue.push(child->children[4]);
							queue.push(child->children[5]);
							queue.push(child->children[6]);
							queue.push(child->children[7]);
						}
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).y != (currentCube->origin.y)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(bottom);
					while (!queue.empty()){
						child = queue.front();
						queue.pop();
						if (!child->isLeaf){
							queue.push(child->children[0]);
							queue.push(child->children[1]);
							queue.push(child->children[2]);
							queue.push(child->children[3]);
							queue.push(child->children[4]);
							queue.push(child->children[5]);
							queue.push(child->children[6]);
							queue.push(child->children[7]);
						}
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								//For each one of those points, check to see if it matches on in pointsOnFace2old, If it does, change it with the point in the same index in pointsOnFace2New
								for (int k = 0; k < pointsOnFace2Old.size(); k++){
									if (child->points->at(j).x == pointsOnFace2Old[k].x && child->points->at(j).y == pointsOnFace2Old[k].y && child->points->at(j).z == pointsOnFace2Old[k].z){
										child->points->at(j).x = pointsOnFace2New[k].x;
										child->points->at(j).y = pointsOnFace2New[k].y;
										child->points->at(j).z = pointsOnFace2New[k].z;
									}
								}

							}
						}
					}


				}
			}


			if (further != NULL && !further->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).z == (currentCube->origin.z + currentCube->sizeZ)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					skipPoint = false;
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(further);
					while (!queue.empty()){
						child = queue.front();
						queue.pop();
						if (!child->isLeaf){
							queue.push(child->children[0]);
							queue.push(child->children[1]);
							queue.push(child->children[2]);
							queue.push(child->children[3]);
							queue.push(child->children[4]);
							queue.push(child->children[5]);
							queue.push(child->children[6]);
							queue.push(child->children[7]);
						}
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).z != (currentCube->origin.z + currentCube->sizeZ)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(further);
					while (!queue.empty()){
						child = queue.front();
						queue.pop();
						if (!child->isLeaf){
							queue.push(child->children[0]);
							queue.push(child->children[1]);
							queue.push(child->children[2]);
							queue.push(child->children[3]);
							queue.push(child->children[4]);
							queue.push(child->children[5]);
							queue.push(child->children[6]);
							queue.push(child->children[7]);
						}
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								//For each one of those points, check to see if it matches on in pointsOnFace2old, If it does, change it with the point in the same index in pointsOnFace2New
								for (int k = 0; k < pointsOnFace2Old.size(); k++){
									if (child->points->at(j).x == pointsOnFace2Old[k].x && child->points->at(j).y == pointsOnFace2Old[k].y && child->points->at(j).z == pointsOnFace2Old[k].z){
										child->points->at(j).x = pointsOnFace2New[k].x;
										child->points->at(j).y = pointsOnFace2New[k].y;
										child->points->at(j).z = pointsOnFace2New[k].z;
									}
								}

							}
						}
					}


				}
			}


			if (closer != NULL && !closer->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).z == (currentCube->origin.z)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					skipPoint = false;
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(closer);
					while (!queue.empty()){
						child = queue.front();
						queue.pop();
						if (!child->isLeaf){
							queue.push(child->children[0]);
							queue.push(child->children[1]);
							queue.push(child->children[2]);
							queue.push(child->children[3]);
							queue.push(child->children[4]);
							queue.push(child->children[5]);
							queue.push(child->children[6]);
							queue.push(child->children[7]);
						}
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).z != (currentCube->origin.z)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					//pointsOnFace2New.reserve(pointsOnFace2Old.size());
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));

						//std::cout << "point 1 " << pointsOnFaceNoDuplicates[0].x << "  " << pointsOnFaceNoDuplicates[0].y << "  " << pointsOnFaceNoDuplicates[0].z << std::endl;
						//std::cout << "point 2 " << pointsOnFaceNoDuplicates[1].x << "  " << pointsOnFaceNoDuplicates[1].y << "  " << pointsOnFaceNoDuplicates[1].z << std::endl;
						//std::cout << "point for projection " << pointsOnFace2Old[i].x << "  " << pointsOnFace2Old[i].y << "  " << pointsOnFace2Old[i].z << std::endl;
						//std::cout << "point projected " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << std::endl;
						//std::cin.get();
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(closer);
					while (!queue.empty()){
						child = queue.front();
						queue.pop();
						if (!child->isLeaf){
							queue.push(child->children[0]);
							queue.push(child->children[1]);
							queue.push(child->children[2]);
							queue.push(child->children[3]);
							queue.push(child->children[4]);
							queue.push(child->children[5]);
							queue.push(child->children[6]);
							queue.push(child->children[7]);
						}
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								//For each one of those points, check to see if it matches on in pointsOnFace2old, If it does, change it with the point in the same index in pointsOnFace2New
								for (int k = 0; k < pointsOnFace2Old.size(); k++){
									if (child->points->at(j).x == pointsOnFace2Old[k].x && child->points->at(j).y == pointsOnFace2Old[k].y && child->points->at(j).z == pointsOnFace2Old[k].z){
										child->points->at(j).x = pointsOnFace2New[k].x;
										child->points->at(j).y = pointsOnFace2New[k].y;
										child->points->at(j).z = pointsOnFace2New[k].z;
									}
								}

							}
						}
					}


				}
			}










		}
	}






	//Go breath-first through the octree
	//For each cube, find its neibhours
	//check if thos eneigbours have a conflict, if there is a conflict then resolve it
	/*OctreeCube* currentCube;
	OctreeCube* adyacent;
	std::queue<OctreeCube*> queue;
	std::queue<OctreeCube*> queueInternal;
	int count = 0;

	queue.push(root);
	while (!queue.empty()){
	currentCube = queue.front();
	queue.pop();
	if (currentCube->children[0] != NULL){
	queue.push(currentCube->children[0]);
	queue.push(currentCube->children[1]);
	queue.push(currentCube->children[2]);
	queue.push(currentCube->children[3]);
	queue.push(currentCube->children[4]);
	queue.push(currentCube->children[5]);
	queue.push(currentCube->children[6]);
	queue.push(currentCube->children[7]);
	}

	std::cout << "iter number " << count << std::endl;
	count++;

	if (currentCube->children[0] != NULL)	//If the current cube is not a leaf one then it does not polygonons in it so it doesnt need checking  for conflicts with adyacent cubes
	continue;

	//For this current cube, make another loop so that it finds the nighours
	queueInternal.push(root);
	while (!queueInternal.empty()){
	adyacent = queueInternal.front();
	queueInternal.pop();
	if (adyacent->children[0] != NULL){
	queueInternal.push(adyacent->children[0]);
	queueInternal.push(adyacent->children[1]);
	queueInternal.push(adyacent->children[2]);
	queueInternal.push(adyacent->children[3]);
	queueInternal.push(adyacent->children[4]);
	queueInternal.push(adyacent->children[5]);
	queueInternal.push(adyacent->children[6]);
	queueInternal.push(adyacent->children[7]);
	}
	}

	}*/




	//THe code below is higly unoptimized because is On^6
	/*for (int cube = 0; cube < model->cubes.size(); cube++){
	if (model->cubes[cube].isLeaf && model->cubes[cube].needsChecking){
	// check the adyacent cubes, if there are not leaf that means that they are subdivided and thus the face that divides the two cubes is a transition face

	//we search for the cube that is on the right
	for (int ady = 0; ady < model->cubes.size(); ady++){
	if (model->cubes[ady].origin.x == (model->cubes[cube].origin.x + model->cubes[cube].sizeX) &&
	model->cubes[ady].origin.y == model->cubes[cube].origin.y &&
	model->cubes[ady].origin.z == model->cubes[cube].origin.z &&
	!model->cubes[ady].isLeaf){
	cout << "found a cube on the right that is not leaf" << endl;
	}
	}

	//we search for the cube that is on the left
	for (int ady = 0; ady < model->cubes.size(); ady++){
	if (model->cubes[ady].origin.x == (model->cubes[cube].origin.x - model->cubes[cube].sizeX) &&
	model->cubes[ady].origin.y == model->cubes[cube].origin.y &&
	model->cubes[ady].origin.z == model->cubes[cube].origin.z &&
	!model->cubes[ady].isLeaf){
	cout << "found a cube on the left that is not leaf" << endl;
	}
	}

	//we search for the cube that is toward the screen
	for (int ady = 0; ady < model->cubes.size(); ady++){
	if (model->cubes[ady].origin.x == model->cubes[cube].origin.x &&
	model->cubes[ady].origin.y == model->cubes[cube].origin.y &&
	model->cubes[ady].origin.z == (model->cubes[cube].origin.z + model->cubes[cube].sizeZ) &&
	!model->cubes[ady].isLeaf){
	cout << "found a cube on the z+ that is not leaf" << endl;
	}
	}

	//we search for the cube that is toward me
	for (int ady = 0; ady < model->cubes.size(); ady++){
	if (model->cubes[ady].origin.x == model->cubes[cube].origin.x &&
	model->cubes[ady].origin.y == model->cubes[cube].origin.y &&
	model->cubes[ady].origin.z == (model->cubes[cube].origin.z - model->cubes[cube].sizeZ) &&
	!model->cubes[ady].isLeaf){
	cout << "found a cube on the z- that is not leaf" << endl;
	}
	}

	//we search for the cube that is on top
	for (int ady = 0; ady < model->cubes.size(); ady++){
	if (model->cubes[ady].origin.x == model->cubes[cube].origin.x &&
	model->cubes[ady].origin.y == (model->cubes[cube].origin.y + model->cubes[cube].sizeY) &&
	model->cubes[ady].origin.z == model->cubes[cube].origin.z &&
	!model->cubes[ady].isLeaf){
	cout << "found a cube on the top that is not leaf" << endl;
	}
	}

	//we search for the cube that is on bottom
	for (int ady = 0; ady < model->cubes.size(); ady++){
	if (model->cubes[ady].origin.x == model->cubes[cube].origin.x &&
	model->cubes[ady].origin.y == (model->cubes[cube].origin.y - model->cubes[cube].sizeY) &&
	model->cubes[ady].origin.z == model->cubes[cube].origin.z &&
	!model->cubes[ady].isLeaf){
	cout << "found a cube on the bottom that is not leaf" << endl;
	}
	}


	cout << "Finished with that cube" << cube << " of " << model->cubes.size() << endl << endl;

	}

	}*/
}


glm::vec3 AdaptiveCuber::pointOnLine(glm::vec3 a, glm::vec3 b, glm::vec3& x){



	double dot = a.x*b.x + a.y*b.y + a.z*b.z;
	double lenSq1 = a.x*a.x + a.y*a.y + a.z*a.z;
	double lenSq2 = b.x*b.x + b.y*b.y + b.z*b.z;
	double angle = acos(dot / sqrt(lenSq1 * lenSq2));

	//std::cout << "the angles is " << angle << std::endl;


	glm::vec3 x_b, a_b, a_bDiv, projection;
	float x_b_a_bDot, a_bModule, a_bModulePow, div, x_bModule, optional;

	x_b.x = x.x - b.x;
	x_b.y = x.y - b.y;
	x_b.z = x.z - b.z;

	a_b.x = a.x - b.x;
	a_b.y = a.y - b.y;
	a_b.z = a.z - b.z;



	/////////////Optional
	x_bModule = sqrt(x_b.x*x_b.x + x_b.y*x_b.y + x_b.z*x_b.z);
	a_bModule = sqrt(a_b.x*a_b.x + a_b.y*a_b.y + a_b.z*a_b.z);
	optional = x_bModule*a_bModule*cos(angle);

	/*if (angle < 0.001){
	std:cout << "angle is too small" << std::endl;
	return x;
	}*/


	///////End of optional


	x_b_a_bDot = x_b.x*a_b.x + x_b.y*a_b.y + x_b.z*a_b.z;
	a_bModule = sqrt(a_b.x*a_b.x + a_b.y*a_b.y + a_b.z*a_b.z);
	a_bModulePow = a_bModule*a_bModule;

	//div = optional / a_bModulePow;
	div = x_b_a_bDot / a_bModulePow;

	a_bDiv.x = a_b.x*div;
	a_bDiv.y = a_b.y*div;
	a_bDiv.z = a_b.z*div;

	projection.x = b.x + a_bDiv.x;
	projection.y = b.y + a_bDiv.y;
	projection.z = b.z + a_bDiv.z;

	return projection;
}


int AdaptiveCuber::readPointsFromOctree(OctreeCube* root){
	OctreeCube* currentCube;
	std::queue<OctreeCube*> queue;
	queue.push(root);
	while (!queue.empty()){
		currentCube = queue.front();
		queue.pop();
		if (currentCube->children[0] != NULL){
			queue.push(currentCube->children[0]);
			queue.push(currentCube->children[1]);
			queue.push(currentCube->children[2]);
			queue.push(currentCube->children[3]);
			queue.push(currentCube->children[4]);
			queue.push(currentCube->children[5]);
			queue.push(currentCube->children[6]);
			queue.push(currentCube->children[7]);
		}

		//Grab the current cube and polygonise it f it's leaf
		if (currentCube->points != NULL){
			for (int i = 0; i < currentCube->points->size(); i++){
				verts->push_back(currentCube->points->at(i));

			}
		}

	}
	return 0;
}