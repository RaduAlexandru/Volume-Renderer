#include "Model.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>





Model::Model()
{
	isoLevel = 14000;
	cellSizeX=6;
	cellSizeY=6;
	cellSizeZ=6;
	//frames = 0;
	
	frame_to_display = 0;
	//pixelDataHeight = 0;
	//pixelDataWidth = 0;

	
	showWireframe = false;
	perspectiveActivated = true;
	showMesh = true;
	showMesh2 = true;
	showGradient = false;
	showCubes = false;
	tolerance = 60;
	octreeMaxDepth = 8;
	interpolateDepth = 50;

	algorithmChosen=1;
	normalsAlgChosen = 1;
	orientation = 1;
	generatingMesh = false;


	xPosPoint = 0;
	yPosPoint = 0;
	zPosPoint = 0;

	pixelData = new PixelData();

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


}


Model::~Model()
{
}



int Model::getSmoothPixelValue(int x, int y, int z){

	//kernel is this one http://mathforum.org/mathimages/imgUpload/math/8/f/4/8f4eb18c8c35a55c00629e7dde0480f9.png

	int value = 0;

	

	value = 4 * pixelData->getPixelValue(x, y, z) + 2 * pixelData->getPixelValue(x + 1, y, z) + 2 * pixelData->getPixelValue(x - 1, y, z) +
		2 * pixelData->getPixelValue(x, y + 1, z) + 2 * pixelData->getPixelValue(x, y - 1, z)
		+ 1 * pixelData->getPixelValue(x - 1, y + 1, z) + 1 * pixelData->getPixelValue(x + 1, y + 1, z) + 1 * pixelData->getPixelValue(x - 1, y - 1, z) + 1 * pixelData->getPixelValue(x + 1, y - 1, z)

		+ 2 * pixelData->getPixelValue(x, y, z - 1) + 1 * pixelData->getPixelValue(x - 1, y, z - 1) + 1 * pixelData->getPixelValue(x, y + 1, z - 1) + 1 * pixelData->getPixelValue(x, y - 1, z - 1) + 1 * pixelData->getPixelValue(x + 1, y, z - 1)
		+ 2 * pixelData->getPixelValue(x, y, z + 1) + 1 * pixelData->getPixelValue(x - 1, y, z + 1) + 1 * pixelData->getPixelValue(x, y + 1, z + 1) + 1 * pixelData->getPixelValue(x, y - 1, z + 1) + 1 * pixelData->getPixelValue(x + 1, y, z + 1);

	value = value / 28;
	return value;

}