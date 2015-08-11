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

	tolerance = 60;
	octreeMaxDepth = 8;
	interpolateDepth = 50;

	algorithmChosen=1;
	normalsAlgChosen = 1;
	generatingMesh = false;


	

	pixelData = new PixelData();
	mesh = new Mesh;
	mesh2 = new Mesh;

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


