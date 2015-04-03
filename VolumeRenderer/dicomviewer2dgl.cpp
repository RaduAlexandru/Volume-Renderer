#include "dicomviewer2dgl.h"
#include <iostream>
#include <math.h> 
#include "OctreeCube.h"
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#define PI 3.14159265



DicomViewer2DGL::DicomViewer2DGL(QWidget *parent)
	: QOpenGLWidget(parent)
{
	frame_to_display = 0;
	tolerance = 0;
}

DicomViewer2DGL::~DicomViewer2DGL()
{

}


void DicomViewer2DGL::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.8, 0.8, 0.8, 0);
	//glEnable(GL_DEPTH_TEST);
	//sendDataToGL();
}

void DicomViewer2DGL::paintGL()
{

	
	glViewport(0.0, 0.0, width(), height());
	glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	//glOrtho(0.0f, model->pixelDataWidth, model->pixelDataHeight, 0.0f, 1.0f, 0.0f);	//When you do the 3d one you will need to change the last parameter to be the depth of the image (number of frames)
	glOrtho(0.0f, model->pixelDataWidth,0.0f, model->pixelDataHeight,  1.0f, 0.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	
	glPixelZoom((float)this->width() / (float)model->pixelDataWidth, (float)this->height() / (float)model->pixelDataHeight);  //used to scale the image down if the image is to big to fit in the window. 
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 2); //In case the size of the image is not a power of 2

	if (model->pixelDataHeight != 0){
		if (model->numberOfBytes == 1)
			//glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, ((model->pixelData))[frame_to_display]);
			if (!gradient.empty())
				glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_INT, gradient.data());
		if (model->numberOfBytes == 2)
			//glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_SHORT, ((model->pixelData))[frame_to_display]);
			if (!gradient.empty())
				glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_INT, gradient.data());
		if (model->numberOfBytes == 3)
			glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_INT, ((model->pixelData))[frame_to_display]);

	}
	

	if (cubes.empty()){
		//std::cout << "cubes is empty" << std::endl;
		return;
	}

	//std::cout << "drawing cubes" << std::endl;
	glColor3f(1.f, 0.f, 0.f);
	glBegin(GL_LINES);
	for (int i = 0; i < cubes.size(); i = i +1){
		glVertex2f(cubes[i].topLeft.x, cubes[i].topLeft.y);
		glVertex2f(cubes[i].topRight.x, cubes[i].topRight.y);

		glVertex2f(cubes[i].topRight.x, cubes[i].topRight.y);
		glVertex2f(cubes[i].bottomRight.x, cubes[i].bottomRight.y);

		glVertex2f(cubes[i].bottomRight.x, cubes[i].bottomRight.y);
		glVertex2f(cubes[i].bottomLeft.x, cubes[i].bottomLeft.y);

		glVertex2f(cubes[i].bottomLeft.x, cubes[i].bottomLeft.y);
		glVertex2f(cubes[i].topLeft.x, cubes[i].topLeft.y);
	}
	glEnd();

	glColor3f(0.f, 1.f, 0.f);
	glBegin(GL_LINES);
	glVertex2f(selectedCube.topLeft.x, selectedCube.topLeft.y);
	glVertex2f(selectedCube.topRight.x, selectedCube.topRight.y);

	glVertex2f(selectedCube.topRight.x, selectedCube.topRight.y);
	glVertex2f(selectedCube.bottomRight.x, selectedCube.bottomRight.y);

	glVertex2f(selectedCube.bottomRight.x, selectedCube.bottomRight.y);
	glVertex2f(selectedCube.bottomLeft.x, selectedCube.bottomLeft.y);

	glVertex2f(selectedCube.bottomLeft.x, selectedCube.bottomLeft.y);
	glVertex2f(selectedCube.topLeft.x, selectedCube.topLeft.y);
	glEnd();


	/*glColor3f(0.f, 0.f, 1.f);
	glPointSize(4.0f);
	glBegin(GL_LINES);
	glVertex2f(0, 0);
	glVertex2f(100, 100);
	glEnd();*/
	
	

}

void DicomViewer2DGL::resizeGL(int w, int h)
{

}

void DicomViewer2DGL::setFrame(int frame)
{
	model->frame_to_display = frame;
	frame_to_display = frame; //We actually have to make something like frameToDisplay=frame% model-> numberOfFrameInDicomFile
	//std::cout << "the first few pixels have value" << (int)model->pixelData[frame_to_display][1] << std::endl;

	//Here we calculate the gradient image and we just store it on top of the one that we should be viewing
	unsigned char* dataPointer;
	int value=0;
	int pointerOffset = model->numberOfBytes;
	int dx=0, dy=0, magnitude=0, left=0, right=0, top=0, bottom=0;
	int multiplier_magnitude = pow(2, 32 - model->numberOfBytes * 8);	//We need the multiplier to scale the magnitude of the gradient to the correct value to be represented on screen. The Dicom image will be of 8,16 or 24 bits. But then we store the magnitude value in a int value (32). So we have to scale it up by multypling by that multiplier. so we scale from 0-256 up to 0-162663236 (maximum value of an int)
	long long multiplier_angle = pow(2, 32 - 8 ); //In the case of the angle the multiplier is static since it only scales from the 0-256 range up to 0-1623563 range. It does not depend on the number of bytes of the dicom image
	
	multiplier_angle = 11930464;
	int angle=0; //maybe it should be float

	gradient.clear();
	std::ofstream outputFile;
	outputFile.open("SelectedEverythingAngles.txt");
	//...
	
	//Here i is y axis and j is the x axis
	for (int i = 0; i < model->pixelDataHeight; i = i + 1){
		for (int j = 0; j <model->pixelDataWidth ; j = j + 1){

			
			if (i == 0 || i == model->pixelDataHeight || j == 0 || j == model->pixelDataWidth){		//If we are in a border we just put it at 0
				gradient.push_back(0);
				continue;
			}

			//Now we take the values
			dataPointer = &(model->pixelData[frame][0]);
			dataPointer = dataPointer + (j-1 + i*model->pixelDataWidth)*pointerOffset;
			memcpy(&left, dataPointer, pointerOffset);

			dataPointer = &(model->pixelData[frame][0]);
			dataPointer = dataPointer + (j + 1 + i*model->pixelDataWidth)*pointerOffset;
			memcpy(&right, dataPointer, pointerOffset);

			dataPointer = &(model->pixelData[frame][0]);
			dataPointer = dataPointer + (j + (i - 1)*model->pixelDataWidth)*pointerOffset;
			memcpy(&top, dataPointer, pointerOffset);

			dataPointer = &(model->pixelData[frame][0]);
			dataPointer = dataPointer + (j + (i + 1)*model->pixelDataWidth)*pointerOffset;
			memcpy(&bottom, dataPointer, pointerOffset);

			dy = left - right;
			dx = top - bottom;

			if (dx == 0)
				dx = 1;
			if (dy == 0)
				dy = 1;

			

			magnitude = hypot(dx,dy);
			angle = atan2(dy, dx) * 180 / PI;
			angle = angle +180; //to avoid negative values

			//if (angle!=225)
				//outputFile << "angle is " << angle << std::endl;

			if (magnitude<2500)
			//if (magnitude<30)
				gradient.push_back(255*multiplier_angle);
			else
				gradient.push_back(angle* multiplier_angle  );

			//gradient.push_back(magnitude* multiplier_magnitude);

		}				
	}

	/*int gradientInt = 0;
	for (int i = 0; i < gradient.size(); i++){
		gradientInt = gradient[i];
		std::cout << "gradients value is" << gradientInt << std::endl;
		if (gradientInt < 0){
			std::cout << "WTF" << std::endl;
			system("pause");
		}
	}*/



	
	

	cubes.clear();
	//Create initial cube
	//Look inside of it to see if the angle changes enough. If it does subdivide it and add the new cubes in 
	OctreeCube octreeCube;

	/*octreeCube.topLeft.x = 0;
	octreeCube.topLeft.y = 0;

	octreeCube.topRight.x = model->pixelDataWidth;
	octreeCube.topRight.y = 0;

	octreeCube.bottomLeft.x = 0;
	octreeCube.bottomLeft.y = model->pixelDataHeight;

	octreeCube.bottomRight.x = model->pixelDataWidth;
	octreeCube.bottomRight.y = model->pixelDataHeight;*/

	octreeCube.topLeft.x = 0;
	octreeCube.topLeft.y = model->pixelDataHeight;

	octreeCube.topRight.x = model->pixelDataWidth;
	octreeCube.topRight.y = model->pixelDataHeight;

	octreeCube.bottomLeft.x = 0;
	octreeCube.bottomLeft.y = 0;

	octreeCube.bottomRight.x = model->pixelDataWidth;
	octreeCube.bottomRight.y = 0;

	octreeCube.isLeaf = true;
	cubes.push_back(octreeCube);

	int depth = 0;
	int cubesSize = 0;
	bool needSubdivision=false;
	long long firstAngle=-1;
	

	//WE check the cubes The ones that are leaf, we check if it needs subdivision, the children we add them to the vector of cubes
	while (1){
		cubesSize = cubes.size();
		for (int cube = 0; cube < cubesSize; cube++){


			//We check the cube in the middle right /For some reason it doesnt see it so maybe we can create it
			/*if (cubes[cube].topLeft.x == model->pixelDataWidth / 2 && cubes[cube].topLeft.y == model->pixelDataHeight &&
				cubes[cube].topRight.x == model->pixelDataWidth / 2 + model->pixelDataWidth / 8 && cubes[cube].topRight.y == model->pixelDataHeight &&
				cubes[cube].bottomLeft.x == model->pixelDataWidth / 2 && cubes[cube].bottomLeft.y == model->pixelDataHeight - model->pixelDataHeight / 8 &&
				cubes[cube].bottomRight.x == model->pixelDataWidth / 2 + model->pixelDataWidth / 8 && cubes[cube].bottomRight.y == model->pixelDataHeight - model->pixelDataHeight / 8){
				std::cout << "we check that cube " << std::endl;
			}*/
			/*selectedCube.topLeft.x = model->pixelDataWidth / 2;
			selectedCube.topLeft.y = model->pixelDataHeight;
			selectedCube.topRight.x = model->pixelDataWidth / 2 + model->pixelDataWidth / 8;
			selectedCube.topRight.y = model->pixelDataHeight;
			selectedCube.bottomLeft.x = model->pixelDataWidth / 2;
			selectedCube.bottomLeft.y = model->pixelDataHeight - model->pixelDataHeight / 8;
			selectedCube.bottomRight.x = model->pixelDataWidth / 2 + model->pixelDataWidth / 8;
			selectedCube.bottomRight.y = model->pixelDataHeight - model->pixelDataHeight / 8;*/

			if (cubes[cube].topLeft.x == 192 && cubes[cube].topLeft.y == 192 &&
				cubes[cube].topRight.x == model->pixelDataWidth && cubes[cube].topRight.y == 192 &&
				cubes[cube].bottomLeft.x == 192 && cubes[cube].bottomLeft.y == 128 &&
				cubes[cube].bottomRight.x == model->pixelDataWidth && cubes[cube].bottomRight.y == 128){
				//std::cout << "we check that cube " << std::endl;
				std::ofstream outputFile;
				outputFile.open("SelectedCubeAngles.txt");

				for (int i = cubes[cube].bottomLeft.y; i < cubes[cube].topLeft.y; i = i + 1){
					for (int j = cubes[cube].topLeft.x; j < cubes[cube].topRight.x; j = j + 1){
						long long actualValue = -1;
						actualValue = gradient[j + i*model->pixelDataWidth];
						//outputFile << "value in the selected cube is" << actualValue/multiplier_angle << std::endl;
					}
				}

			}


			selectedCube.topLeft.x = 192;
			selectedCube.topLeft.y = 192;
			selectedCube.topRight.x = model->pixelDataWidth ;
			selectedCube.topRight.y = 192;
			selectedCube.bottomLeft.x = 192;
			selectedCube.bottomLeft.y = 128;
			selectedCube.bottomRight.x = model->pixelDataWidth ;
			selectedCube.bottomRight.y = 128;



			if (cubes[cube].isLeaf){
				needSubdivision = false;
				firstAngle = -1;
				//Check if it needs subdivision. We check the matrix of the image and see the angle.
				//std::cout << "cube has topLeft=" << cubes[cube].topLeft.x << " topRight= " << cubes[cube].topRight.x << " topLeft= " << cubes[cube].topLeft.y << " bottomLeft= " << cubes[cube].bottomLeft.y << std::endl;

				

				
				for (int i = cubes[cube].bottomLeft.y; i < cubes[cube].topLeft.y; i = i + 1){
					for (int j = cubes[cube].topLeft.x; j <  cubes[cube].topRight.x; j = j + 1){

						

						//int actualValue = gradient[j + (cubes[cube].bottomLeft.y - i - 1)*model->pixelDataWidth];
						long long actualValue = -1;
						actualValue=gradient[j + i*model->pixelDataWidth];

						//std::cout << "actual value is" << actualValue/multiplier_angle << std::endl;

						if (firstAngle == -1 &&  actualValue/multiplier_angle!=255){
							firstAngle =actualValue / multiplier_angle;
						}

						//std::cout << "first angle is " << firstAngle << " actualValue is " << actualValue / multiplier_angle << std::endl;

						if (abs(firstAngle - (actualValue / multiplier_angle)) >tolerance && firstAngle != -1 &&actualValue/multiplier_angle!=255 ){
							needSubdivision = true;
							//needSubdivision = (int)rand % 2;
						}

						


						





					}
				}
				//needSubdivision = rand() % 2;
				
				if (!needSubdivision)
					continue;

				//Lets assume the previous loop found enough complexity in the mesh and we need a subdivision
				//Let's assume that a subdivision is needed for that node leaf
				cubes[cube].isLeaf = false;


				//Ill just doo them again since now the  0,0  is at the bottom left
				//Cube 1

				/*
				-------------------
				|   1    !    2   !
				!        !        !
				!-----------------
				!   3    !    4   !
				!        !        !
				-------------------
				*/
				octreeCube.topLeft.x = cubes[cube].topLeft.x;
				octreeCube.topLeft.y = cubes[cube].topLeft.y;

				octreeCube.topRight.x = (cubes[cube].topRight.x - cubes[cube].topLeft.x) / 2 + cubes[cube].topLeft.x;
				octreeCube.topRight.y = cubes[cube].topRight.y;

				octreeCube.bottomLeft.x = cubes[cube].bottomLeft.x;
				octreeCube.bottomLeft.y = (cubes[cube].topLeft.y - cubes[cube].bottomLeft.y) / 2 + cubes[cube].bottomLeft.y;

				octreeCube.bottomRight.x = (cubes[cube].bottomRight.x - cubes[cube].bottomLeft.x) / 2 + cubes[cube].bottomLeft.x;
				octreeCube.bottomRight.y = (cubes[cube].topRight.y - cubes[cube].bottomRight.y) / 2 + cubes[cube].bottomRight.y;

				octreeCube.isLeaf = true;
				cubes.push_back(octreeCube);

				//Cube2
				octreeCube.topLeft.x = (cubes[cube].topRight.x - cubes[cube].topLeft.x) / 2 + cubes[cube].topLeft.x;
				octreeCube.topLeft.y = cubes[cube].topLeft.y;

				octreeCube.topRight = cubes[cube].topRight;


				octreeCube.bottomLeft.x = (cubes[cube].bottomRight.x - cubes[cube].bottomLeft.x) / 2 + cubes[cube].bottomLeft.x;
				octreeCube.bottomLeft.y = (cubes[cube].topRight.y - cubes[cube].bottomRight.y) / 2 + cubes[cube].bottomRight.y;

				octreeCube.bottomRight.x = cubes[cube].bottomRight.x;
				octreeCube.bottomRight.y = (cubes[cube].topRight.y - cubes[cube].bottomRight.y) / 2 + cubes[cube].bottomRight.y;

				octreeCube.isLeaf = true;
				cubes.push_back(octreeCube);

				//Cube3
				octreeCube.topLeft.x = cubes[cube].topLeft.x;
				octreeCube.topLeft.y = (cubes[cube].topLeft.y - cubes[cube].bottomLeft.y) / 2 + cubes[cube].bottomLeft.y;

				octreeCube.topRight.x = (cubes[cube].bottomRight.x - cubes[cube].bottomLeft.x) / 2 + cubes[cube].bottomLeft.x;
				octreeCube.topRight.y = (cubes[cube].topRight.y - cubes[cube].bottomRight.y) / 2 + cubes[cube].bottomRight.y;

				octreeCube.bottomLeft.x = cubes[cube].bottomLeft.x;
				octreeCube.bottomLeft.y = cubes[cube].bottomLeft.y;

				octreeCube.bottomRight.x = (cubes[cube].bottomRight.x - cubes[cube].bottomLeft.x) / 2 + cubes[cube].bottomLeft.x;
				octreeCube.bottomRight.y = cubes[cube].bottomRight.y;

				octreeCube.isLeaf = true;
				cubes.push_back(octreeCube);

				//CUBE 4
				octreeCube.topLeft.x = (cubes[cube].topRight.x - cubes[cube].topLeft.x) / 2 + cubes[cube].topLeft.x;
				octreeCube.topLeft.y = (cubes[cube].topLeft.y - cubes[cube].bottomLeft.y) / 2 + cubes[cube].bottomLeft.y;

				octreeCube.topRight.x = cubes[cube].topRight.x;
				octreeCube.topRight.y = (cubes[cube].topRight.y - cubes[cube].bottomRight.y) / 2 + cubes[cube].bottomRight.y;

				octreeCube.bottomLeft.x = (cubes[cube].bottomRight.x - cubes[cube].bottomLeft.x) / 2 + cubes[cube].bottomLeft.x;
				octreeCube.bottomLeft.y = cubes[cube].bottomLeft.y;

				octreeCube.bottomRight = cubes[cube].bottomRight;


				octreeCube.isLeaf = true;
				cubes.push_back(octreeCube);

			}

		}
		depth++;
		if (depth == 7){
			depth = 0;
			break;
		}
	}


	

	//Now we make the subdivision
	/*cubes.push_back(1);
	cubes.push_back(1);

	cubes.push_back(1);
	cubes.push_back(model->pixelDataHeight/2);

	cubes.push_back(model->pixelDataWidth/2);
	cubes.push_back(model->pixelDataHeight/2);

	cubes.push_back(model->pixelDataWidth/2);
	cubes.push_back(1);*/


}


void DicomViewer2DGL::setTolerance(int toleranceReceived){
	tolerance = toleranceReceived;
	//std::cout << "model pixel data"
	if (model->pixelDataWidth == 500 || model->pixelDataWidth==256 || model->pixelDataWidth==512)
		setFrame(0);
	
}