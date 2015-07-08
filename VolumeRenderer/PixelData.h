#pragma once


class PixelData
{
public:
	PixelData();
	~PixelData();

	


	unsigned char** data;

	

	int width;
	int height;
	int frames;
	int numberOfBytes;



	int borderXRight, borderXLeft, borderZCloser, borderZFurther, borderYTop, borderYBottom;

	int getPixelValue(int x, int y, int z);
	int getSmoothPixelValue(int x, int y, int z);

	int convolveX(int x, int y, int z);
	int convolveY(int x, int y, int z);
	int convolveZ(int x, int y, int z);

	/*int getWidthWithBorder();
	int getHeightWithBorder();
	int getFramesWithBorder();*/

	
};



