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
	borderYTop = 0; 
	borderYBottom = 0;

}


PixelData::~PixelData()
{
}

int PixelData::getPixelValue(int x, int y, int z){

	if (x<0 || x>width || y<0 || y>height || z < 0 || z >= frames)
		return 1;

	if (data == NULL)
		return 1;

	unsigned char* dataPointer;
	int value = 0;

	dataPointer = &(((data))[z][0]);
	dataPointer = dataPointer + (x + y*width)*numberOfBytes;
	memcpy(&value, dataPointer, numberOfBytes);
	return value;
}



int PixelData::getSmoothPixelValue(int x, int y, int z){

	//kernel is this one http://mathforum.org/mathimages/imgUpload/math/8/f/4/8f4eb18c8c35a55c00629e7dde0480f9.png

	/*int value = 0;



	value = 4 *getPixelValue(x, y, z) + 2 * getPixelValue(x + 1, y, z) + 2 * getPixelValue(x - 1, y, z) +
		2 * getPixelValue(x, y + 1, z) + 2 * getPixelValue(x, y - 1, z)
		+ 1 * getPixelValue(x - 1, y + 1, z) + 1 * getPixelValue(x + 1, y + 1, z) + 1 * getPixelValue(x - 1, y - 1, z) + 1 * getPixelValue(x + 1, y - 1, z)

		+ 2 * getPixelValue(x, y, z - 1) + 1 * getPixelValue(x - 1, y, z - 1) + 1 * getPixelValue(x, y + 1, z - 1) + 1 * getPixelValue(x, y - 1, z - 1) + 1 *getPixelValue(x + 1, y, z - 1)
		+ 2 * getPixelValue(x, y, z + 1) + 1 * getPixelValue(x - 1, y, z + 1) + 1 * getPixelValue(x, y + 1, z + 1) + 1 * getPixelValue(x, y - 1, z + 1) + 1 * getPixelValue(x + 1, y, z + 1);

	value = value / 28;
	return value;*/

	//////Gausain filtern of 5x5


	int arr[5][5][5] = { {
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 0, 0 },
			{ 0, 1, 4, 1, 0 },
			{ 0, 0, 1, 0, 0 },
			{ 0, 0, 0, 0, 0 }, },


		{
			{ 0, 1, 4, 1, 0 },
			{ 1, 4, 16, 4, 1 },
			{ 4, 7, 26, 7, 4 },
			{ 1, 4, 16, 4, 1 },
			{ 0, 1, 4, 1, 0 }, },

		{
			{ 1, 4, 7, 4, 1 },
			{ 4, 16, 26, 16, 4 },
			{ 7, 26, 41, 26, 7 },
			{ 4, 16, 26, 16, 4 },
			{ 1, 4, 7, 4, 1 },},

		{
			{ 0, 1, 4, 1, 0 },
			{ 1, 4, 16, 4, 1 },
			{ 4, 7, 26, 7, 4 },
			{ 1, 4, 16, 4, 1 },
			{ 0, 1, 4, 1, 0 }, },

		{
			{ 0, 0, 0, 0, 0 },
			{ 0, 0, 1, 0, 0 },
			{ 0, 1, 4, 1, 0 },
			{ 0, 0, 1, 0, 0 },
			{ 0, 0, 0, 0, 0 }, },

	};

	int acum = 0;

	for (int i = 0; i < 5; i++){
		for (int j = 0; j < 5; j++){
			for (int k = 0; k < 5; k++){
				acum += arr[i][j][k] * getPixelValue(x - 2 + i, y - 2 + j, z - 2 + k);

			}
		}
	}

	acum = acum / 513;
	return acum;




}