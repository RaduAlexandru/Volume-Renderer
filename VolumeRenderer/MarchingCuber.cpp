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

MarchingCuber::MarchingCuber(PixelData* pixelData, std::vector<glm::vec3>* verts, std::vector<glm::vec3>* normals, int isoLevel, int cellSizeX, int cellSizeY, int cellSizeZ, int interpolateDepth)
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
	this->verts = verts;
	this->normals=normals;

	this->isoLevel=isoLevel;
	this->frames=pixelData->frames;
	this->pixelDataHeight = pixelData->height;
	this->pixelDataWidth = pixelData->width;
	this->cellSizeX=cellSizeX;
	this->cellSizeY=cellSizeY;
	this->cellSizeZ=cellSizeZ;
	this->pointerOffset = pixelData->numberOfBytes;
	this->interpolateDepth=interpolateDepth;
	this->linearInterpolation=linearInterpolation;


}

MarchingCuber::~MarchingCuber()
{
}



void MarchingCuber::run(){
	verts->clear();
	normals->clear();
	std::cout << "marching cubes algorithm" << std::endl;
	const clock_t begin_time = clock();

	cout << "frame is " << frames << endl;

	//cout << "we march the cubes and write the points in the model" << endl;
	CELL cell;
	/*int cellSizeX, cellSizeY, cellSizeZ, frame;
	cellSizeX = model->cellSizeX;
	cellSizeY = model->cellSizeY;
	cellSizeZ = model->cellSizeZ;*/
	//frame = model->frame_to_display;
	unsigned char* dataPointer;
	int value = 0;
	//int pointerOffset = model->numberOfBytes;

	std::ofstream outputFile;
	outputFile.open("PixelsValues.txt");

	//Now we make a square and march it
	//i is the y axis and j is the x axis
	//Since the bitmap is stored in reverse, we shall asign the y position to height-i insted of just i, thus mirroring the position
	//WE also put - CellsizeY insted of + because we consider the coordinated in the y acis as going from bottom to top insted of top to bottom like usual. Maybe in the dicom files you will change the y position to be only equals to i
	for (int i = 0 + pixelData->borderYBottom; i < pixelDataHeight - cellSizeY -pixelData->borderYTop; i = i + cellSizeY){	//WE make it to be till height -cellsizez because otherwise the last cube will be out of bound

		
		if (i % (pixelDataHeight/10) == 0)
			emit progressValueChangedSignal(i * 100 / pixelDataHeight);
		for (int j = 0 + pixelData->borderXLeft; j < pixelDataWidth - cellSizeX - pixelData->borderXRight; j = j + cellSizeX){
			for (int k = 0 + pixelData->borderZCloser; k < frames - cellSizeZ - pixelData->borderZFurther; k = k + cellSizeZ){

				//cout << "k is" << k << endl;
				//if (model->getPixelValue(j, i, k)!=0)
				//cout << model->getPixelValue(j, i, k) << endl;



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
				//polygonise(cell, model->isoLevel, model->totalPoints);
				polygonise(cell);

			}
		}
	}

	//boost::thread workerThread(boost::bind(&VolumeRenderer::generateNormals, this));
	//model->generatingMesh = false;
	outputFile.close();

	//ui.glwidget->setFrame(5);
	//ui.glwidget->setDataToDisplay(output_pixels, model->totalPoints, width_image, height_image);

	//showPixels(output_pixels,totalPoints);
	std::cout << "Elapsed Time: " << float(clock() - begin_time) / CLOCKS_PER_SEC << endl;

	emit finishedMeshSignal();
	//Now we write to obj file
	/*long numberOfVertsWritten = 0;
	ofstream myfile;
	myfile.open("feetWithSkin.obj");
	myfile << "Writing this to a file.\n";
	for (int i = 0; i < model->verts.size()-3; i=i+3){
	myfile << "v " << model->verts[i] << " " << model->verts[i + 1] << " " << model->verts[i + 2] << "\n";
	numberOfVertsWritten++;
	if (numberOfVertsWritten == 3){
	myfile << "f " << -3 << " " << -2 << " " << -1 << "\n\n";
	numberOfVertsWritten = 0;
	}
	}
	//for (int i = 0; i < model->verts.size() - 3; i = i + 3){
	//	myfile << "f " << i+1 << " " << i+2 << " " << i+3 << "\n";
	//}
	myfile.close(); */
}






int MarchingCuber::polygonise(CELL & cell){

	//std::cout << "we are poligonizisng" << std::endl;
	//vector<Model::POINTF> points;
	glm::vec3 vertlist[12];

	//int isoLevel = model->isoLevel;

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
	//int pointerOffset = model->numberOfBytes;
	int value1 = 0, value2 = 0;

	/* Create the triangles */

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


		verts->push_back(point1);
		verts->push_back(point2);
		verts->push_back(point3);


	}






	return 1;
}




/*! \brief Brief description.
Interpolate es un algoritmo recursivo que recibe como parametro la posicion de los dos puntos que definen una arista 
y sus respectivos valores y devuelve la posicion del punto que mas se aproxima a la superficie. Si el valor de la interpolacion está puesto a 0,
no se realzia ningun calculo y se devuelve el punto que esta en medio de la arista. De esta manera produce resultados rapidos pero no supone una buena aproximacion de la superficie del volumen
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