#pragma once


class PixelData
{
public:
	PixelData();
	~PixelData();

	


	unsigned char** data;	//!< El volumen de datos en un array 2D. Cada capa se guarda en un indice difernte como un array


	

	int width;
	int height;
	int frames;
	int numberOfBytes;



	int borderXRight, borderXLeft, borderZCloser, borderZFurther, borderYTop, borderYBottom; //!< Representa los planos de corte


	int getPixelValue(int x, int y, int z);
	int getSmoothPixelValue(int x, int y, int z);

	int convolveX(int x, int y, int z);
	int convolveY(int x, int y, int z);
	int convolveZ(int x, int y, int z);

	
};



