#include "MarchingCuber.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <math.h> 
#include <ctime>
#include <boost/math/special_functions/round.hpp>
#include <boost/thread.hpp>

using namespace std;

MarchingCuber::MarchingCuber()
{
}



MarchingCuber::MarchingCuber(QObject *_parent)
{
}

MarchingCuber::MarchingCuber(PixelData* pixelData, Mesh* mesh, int isoLevel, int cellSizeX, int cellSizeY, int cellSizeZ, int interpolateDepth)
{
	int v;

	//Read values from the EdgeTable file, and read them as hex values. Store them in an 1D vector of 256
	const char *filename = "edgeTable.txt";
	std::ifstream edgeTableFile("edgeTable.txt");
	while (edgeTableFile >> std::hex >> v){
		edgeTable.push_back(v);
	}


	//Read values from the triTableFile, store them in a 2D Vector of 256x16
	std::ifstream triTableFile("triTable.txt");
	std::string line;
	while (std::getline(triTableFile, line)) {
		std::istringstream is(line);
		triTable.push_back(
			std::vector<int>(std::istream_iterator<int>(is),
			std::istream_iterator<int>()));
	}

	this->pixelData=pixelData;
	this->mesh = mesh;

	this->isoLevel=isoLevel;
	this->frames=pixelData->frames;
	this->pixelDataHeight = pixelData->height;
	this->pixelDataWidth = pixelData->width;
	this->cellSizeX=cellSizeX;
	this->cellSizeY=cellSizeY;
	this->cellSizeZ=cellSizeZ;
	this->interpolateDepth=interpolateDepth;
	//this->linearInterpolation=linearInterpolation;


}

MarchingCuber::~MarchingCuber()
{
}


/*! \brief Inicial el algoritmo de Marching Cubes
*/
void MarchingCuber::run(){
	mesh->verts.clear();
	mesh->normals.clear();
	
	std::cout << "marching cubes algorithm" << std::endl;
	const clock_t begin_time = clock();

	cout << "frame is " << frames << endl;

	
	CELL cell;
	unsigned char* dataPointer;
	int value = 0;


	
	for (int i = 0 + pixelData->borderYBottom; i < pixelDataHeight - cellSizeY -pixelData->borderYTop; i = i + cellSizeY){	

		
		if (i % (pixelDataHeight/10) == 0)
			emit progressValueChangedSignal(i * 100 / pixelDataHeight);
		for (int j = 0 + pixelData->borderXLeft; j < pixelDataWidth - cellSizeX - pixelData->borderXRight; j = j + cellSizeX){
			for (int k = 0 + pixelData->borderZCloser; k < frames - cellSizeZ - pixelData->borderZFurther; k = k + cellSizeZ){

				
				/*
				7---------6
				/			 !
				/			 !
				3---------2	 5
				!         !
				!         !
				0---------1
				*/

				cell.position[0].x = j;
				cell.position[0].y = i;
				cell.position[0].z = k;
				cell.val[0] = pixelData->getPixelValue(j, i, k);

				cell.position[1].x = j + cellSizeX;
				cell.position[1].y = i;
				cell.position[1].z = k;
				cell.val[1] = pixelData->getPixelValue(j + cellSizeX, i, k);

				cell.position[2].x = j + cellSizeX;
				cell.position[2].y = i + cellSizeY;
				cell.position[2].z = k;
				cell.val[2] = pixelData->getPixelValue(j + cellSizeX, i + cellSizeY, k);

				cell.position[3].x = j;
				cell.position[3].y = i + cellSizeY;
				cell.position[3].z = k;
				cell.val[3] = pixelData->getPixelValue(j, i + cellSizeY, k);
				//////
				cell.position[4].x = j;
				cell.position[4].y = i;
				cell.position[4].z = k + cellSizeZ;
				cell.val[4] = pixelData->getPixelValue(j, i, k + cellSizeZ);

				cell.position[5].x = j + cellSizeX;
				cell.position[5].y = i;
				cell.position[5].z = k + cellSizeZ;
				cell.val[5] = pixelData->getPixelValue(j + cellSizeX, i, k + cellSizeZ);

				cell.position[6].x = j + cellSizeX;
				cell.position[6].y = i + cellSizeY;
				cell.position[6].z = k + cellSizeZ;
				cell.val[6] = pixelData->getPixelValue(j + cellSizeX, i + cellSizeY, k + cellSizeZ);

				cell.position[7].x = j;
				cell.position[7].y = i + cellSizeY;
				cell.position[7].z = k + cellSizeZ;
				cell.val[7] = pixelData->getPixelValue(j, i + cellSizeY, k + cellSizeZ);



				//now we have that cell and we have to polygonise it
				polygonise(cell);

			}
		}
	}

	
	
	std::cout << "Elapsed Time: " << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;

	emit finishedMeshSignal();
	
}





/*! \brief Dado un cubo, te crea triangulos dentro de él para aproximar la superficie
*/
int MarchingCuber::polygonise(CELL & cell){

	
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

	/* Create the triangles */

	glm::vec3 point1, point2, point3;
	for (int i = 0; triTable[cubeIndex][i] != -1; i += 3) {

		
		mesh->verts.push_back((vertlist[triTable[cubeIndex][i]]));
		mesh->verts.push_back((vertlist[triTable[cubeIndex][i + 1]]));
		mesh->verts.push_back((vertlist[triTable[cubeIndex][i + 2]]));


	}






	return 1;
}




/*! \brief Interpolacion lineal entre los vertices de una artista y el valor umbral para detectar el punto de interseccion
*
* Interpolate es un algoritmo recursivo que recibe como parametro la posicion de los dos puntos que definen una arista 
* y sus respectivos valores y devuelve la posicion del punto que mas se aproxima a la superficie. Si el valor de la interpolacion esta puesto a 0,
* no se realzia ningun calculo y se devuelve el punto que esta en medio de la arista. De esta manera produce resultados rapidos pero no supone una buena aproximacion de la superficie del volumen
*/
inline void MarchingCuber::interpolate(int isoLevel, glm::vec3 point1, glm::vec3 point2, float val1, float val2, glm::vec3& resultPoint, int depth){


	if (depth == interpolateDepth && depth!=0){
		return;
	}

	//Model::POINTF point;
	//std::cout << "depth is " << depth << std::endl;

	if (interpolateDepth ==0){

		resultPoint.x = (point1.x + point2.x) / 2.0;
		resultPoint.y = (point1.y + point2.y) / 2.0;
		resultPoint.z = (point1.z + point2.z) / 2.0;
	}
	else{

		float mu;
		if (abs(isoLevel - val1) < 0.00001){
			resultPoint = point1;
			return;
		}

		if (abs(isoLevel - val2) < 0.00001){
			resultPoint = point1;
			return;
		}

		mu = (isoLevel - val1) / (val2 - val1);
		resultPoint.x = point1.x + mu * (point2.x - point1.x);
		resultPoint.y = point1.y + mu * (point2.y - point1.y);
		resultPoint.z = point1.z + mu * (point2.z - point1.z);

		//Now we just do it again
		//We dedice witch one is the highest between point 1 and point2

		//The resulting point is inside the surface
		long valueOfResultingPoint = pixelData->getPixelValue(boost::math::iround(resultPoint.x), boost::math::iround(resultPoint.y), boost::math::iround(resultPoint.z));
		if (valueOfResultingPoint > isoLevel){
			//We chose the lowest point and we use that to interpolate
			if (val1 < val2)
				interpolate(isoLevel, point1, resultPoint, val1, valueOfResultingPoint, resultPoint, depth + 1);
			else
				interpolate(isoLevel, point2, resultPoint, val2, valueOfResultingPoint, resultPoint, depth + 1);
		}
		//The resulting point is outside the surface
		if (valueOfResultingPoint < isoLevel){
			if (val2 < val1)
				interpolate(isoLevel, point1, resultPoint, val1, valueOfResultingPoint, resultPoint, depth + 1);
			else
				interpolate(isoLevel, point2, resultPoint, val2, valueOfResultingPoint, resultPoint, depth + 1);
		}
		//The points is exactly on the surface




	}

	return;
}