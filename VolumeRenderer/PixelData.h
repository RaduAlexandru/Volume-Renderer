#pragma once
class PixelData
{
public:
	PixelData();
	~PixelData();

	int getPixelValue(int x, int y, int z);


	unsigned char** data;
	int width;
	int height;
	int frames;
	int numberOfBytes;
};



