#include "PixelData.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>


PixelData::PixelData()
{
	data = NULL;
	width=0;
	height=0;
	frames=0;
	numberOfBytes=0;

	borderXRight = 0;
	borderXLeft = 0;
	borderZCloser = 0;
	borderZFurther = 0;
	borderYTop = 200; 
	borderYBottom = 0;

}


PixelData::~PixelData()
{
}

int PixelData::getPixelValue(int x, int y, int z){

	if (x<0 || x>width || y<0 || y>height || z < 0 || z >= frames)
		return 1;


	unsigned char* dataPointer;
	int value = 0;

	dataPointer = &(((data))[z][0]);
	dataPointer = dataPointer + (x + y*width)*numberOfBytes;
	memcpy(&value, dataPointer, numberOfBytes);
	return value;
}
