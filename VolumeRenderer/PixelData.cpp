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


/*! \brief Dada la posicion x,y,z devuelve el pixel de esa posicion.
*/
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


/*! \brief Dada la posicion x,y,z devuelve el pixel de esa posicion pero filtreado con un kernel gausiano
*
* Se utiliza como prueba para ver el cambio en los gradientes cuando el volumen de datos está difuminado con un filtro gausiano de 5x5x5. 
* El tiempo computacion empleado en difuminar tal cantidad de datos no justifica la mejora en las direcciones de los gradientes.
*/
int PixelData::getSmoothPixelValue(int x, int y, int z){

	

	//////Gausain filtern of 5x5x5


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



/*! \brief Convolucion Sobel en la dirrecion Z
*
* Comentado está otro kernel sobel que da resultados parecidos
*/
int PixelData::convolveZ(int x, int y, int z){

	int arr[3][3][3] = { {
			{ -1, -2, -1 },
			{ -2, -4, -2 },
			{ -1, -2, -1 } },


			{
				{ 0, 0, 0 },
				{ 0, 0, 0 },
				{ 0, 0, 0 } },

				{
					{ 1, 2, 1 },
					{ 2, 4, 2 },
					{ 1, 2, 1 } },

	};

	/*int arr[3][3][3] = { {
	{ -1, 0, 1 },
	{ -3, 0, 3 },
	{ -1, 0, 1 } },


	{
	{ -3, 0, 3 },
	{ -6, 0, 6 },
	{ -3, 0, 3 } },

	{
	{ -1, 0, 1 },
	{ -3, 0, 3 },
	{ -1, 0, 1 } },

	};*/

	int acum = 0;

	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			for (int k = 0; k < 3; k++){
				acum += arr[i][j][k] * getPixelValue(x - 1 + k, y + 1 - j, z - 1 + i);

			}
		}
	}
	return acum;



}


/*! \brief Convolucion Sobel en la dirrecion Y
*/
int PixelData::convolveY(int x, int y, int z){

	int arr[3][3][3] = { {
			{ 1, 2, 1 },
			{ 0, 0, 0 },
			{ -1, -2, -1 } },


			{
				{ 2, 4, 2 },
				{ 0, 0, 0 },
				{ -2, -4, -2 } },

				{
					{ 1, 2, 1 },
					{ 0, 0, 0 },
					{ -1, -2, -1 } },

	};

	/*int arr[3][3][3] = { {
	{ 1, 3, 1 },
	{ 0, 0, 0 },
	{ -1, -3, -1 } },


	{
	{ 3, 6, 3 },
	{ 0, 0, 0 },
	{ -3, -6, -3 } },

	{
	{ 1, 3, 1 },
	{ 0, 0, 0 },
	{ 1, 3, 1 } },

	};*/

	int acum = 0;

	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			for (int k = 0; k < 3; k++){
				acum += arr[i][j][k] * getPixelValue(x - 1 + k, y + 1 - j, z - 1 + i);

			}
		}
	}
	return acum;



}

/*! \brief Convolucion Sobel en la dirrecion X
*/
int PixelData::convolveX(int x, int y, int z){

	int arr[3][3][3] = { {
			{ -1, 0, 1 },
			{ -2, 0, 2 },
			{ -1, 0, 1 } },


			{
				{ -2, 0, 2 },
				{ -4, 0, 4 },
				{ -2, 0, 2 } },

				{
					{ -1, 0, 1 },
					{ -2, 0, 2 },
					{ -1, 0, 1 } },

	};


	/*int arr[3][3][3] = { {
	{ -1, -3, -1 },
	{ -3, -6, -3 },
	{ -1, -3, -1 } },


	{
	{ 0, 0, 0 },
	{ 0, 0, 0 },
	{ 0, 0, 0 } },

	{
	{ 1, 3, 1 },
	{ 3, 6, 3 },
	{ 1, 3, 1 } },

	};*/

	int acum = 0;

	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			for (int k = 0; k < 3; k++){
				//acum += arr[i][j][k] * pixelData->getPixelValue(x - 1 + k, y + 1 - j, z + 1 - i);
				acum += arr[i][j][k] * getPixelValue(x - 1 + k, y + 1 - j, z - 1 + i);
				//std::cout << "val is" << arr[i][j][k] << std::endl;
			}
		}
	}
	return acum;



}