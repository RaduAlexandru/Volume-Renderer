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
	glClearColor(0.0, 0.0, 0.0, 0);
	//glEnable(GL_DEPTH_TEST);
	//sendDataToGL();
}

void DicomViewer2DGL::paintGL()
{

	glClearColor(0.0, 0.0, 0.0, 0);
	glViewport(0.0, 0.0, width(), height());
	glClear(GL_COLOR_BUFFER_BIT);
	//glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	//glOrtho(0.0f, model->pixelDataWidth, model->pixelDataHeight, 0.0f, 1.0f, 0.0f);	//When you do the 3d one you will need to change the last parameter to be the depth of the image (number of frames)
	
	int orientation = model->orientation;
	
	if (orientation == 1)
		glOrtho(0.0f, model->pixelData->width, 0.0f, model->pixelData->height,  0.0f, 1.0f);
	if (orientation == 2)
		glOrtho(0.0f, model->pixelData->height, 0.0f, model->pixelData->frames, 0.0f, 1.0f);
	if (orientation == 3)
		glOrtho(0.0f, model->pixelData->width, 0.0f, model->pixelData->frames, 0.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	
	glPixelZoom((float)this->width() / (float)model->pixelData->width, (float)this->height() / (float)model->pixelData->height);  //used to scale the image down if the image is to big to fit in the window. 
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 2); //In case the size of the image is not a power of 2

	displayFrame();
	/*if (model->showGradient)
		displayGradient();*/

	
	displayBorderLines();
	


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



/*! \brief Muestra en el visor 2D los pixels data almacenados en localFrame
*/
void DicomViewer2DGL::displayFrame(){
	if (model->pixelData->height != 0){

		int orientation = model->orientation;
		//Copy the data fo that frame in a new local frame. Depending on the orientation, z, x or y. We copy on orientation of the frame or another Z=1, x=2;y=3

		


		/*if (model->numberOfBytes == 1)
			glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, ((model->pixelData))[frame_to_display]);
			//if (!gradient.empty())
				//glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_INT, gradient.data());
		if (model->numberOfBytes == 2)
			glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_SHORT, ((model->pixelData))[frame_to_display]);
			//if (!gradient.empty())
				//glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_INT, gradient.data());
		if (model->numberOfBytes == 3)
			glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_INT, ((model->pixelData))[frame_to_display]);*/

		if (!localFrame.empty()){
			if (orientation == 1)	//Normal z orientation
				glDrawPixels(model->pixelData->width, model->pixelData->height, GL_LUMINANCE, GL_UNSIGNED_INT, localFrame.data());
			//if (!gradient.empty())
			//glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_INT, gradient.data());
			if (orientation == 2)	//X direction
				glDrawPixels(model->pixelData->height, model->pixelData->frames, GL_LUMINANCE, GL_UNSIGNED_INT, localFrame.data());
			//if (!gradient.empty())
			//glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_INT, gradient.data());
			if (orientation == 3)	//y direction
				glDrawPixels(model->pixelData->width, model->pixelData->frames, GL_LUMINANCE, GL_UNSIGNED_INT, localFrame.data());
		}

	}
}

/*! \brief Muestra los gradientes en el visor 2D. Solo valido si antes se han calculado los gradientes mediante el AdaptiveMarchingCuber
*/
void DicomViewer2DGL::displayGradient(){

	boost::unordered_map< std::pair<int, int>, glm::vec3>::iterator it;
	glm::vec3 actualValue(-1, -1, -1);

	if (model->gradient.empty())
		return;

	int orientation = model->orientation;

	if (orientation == 1){
		for (int i = 0; i < model->pixelData->height; i++){
			for (int j = 0; j < model->pixelData->width; j++){

				//Look for a gradient with values (j,i) If we found it display the line from (j,i) to the x,y of that gradient
				
				it = model->gradient[frame_to_display].find(std::make_pair(j, i));
				if (it != model->gradient[frame_to_display].end())
				{
					//element found;
					actualValue = it->second;
					//The input for sin is in radians so we do *PI / 180. The output os sin is from -1 to 1. So we add 1 and then divide by 2 so we get a value from 0 to 1
					glColor3f((sin(actualValue.x *PI / 180) + 1) / 2, (sin(actualValue.y *PI / 180) + 1) / 2, (sin(actualValue.z *PI / 180) + 1) / 2);
					glBegin(GL_POINTS);
						glVertex2f(j, i);
					glEnd();
				}

			
			}
		}
	}

}


/*! \brief Muestra los planos de corte que se van a usar para la region de interes
*/
void DicomViewer2DGL::displayBorderLines(){
	int orientation = model->orientation;
	if (orientation == 1){
		glColor3f(1.0f, 0.0, 0.0);

		//Border XLeft
		glBegin(GL_LINES);
			glVertex2f(model->pixelData->borderXLeft, 0);
			glVertex2f(model->pixelData->borderXLeft, model->pixelData->height);
		glEnd();

		//Border XRight
		glBegin(GL_LINES);
			glVertex2f(model->pixelData->width- model->pixelData->borderXRight, 0);
			glVertex2f(model->pixelData->width - model->pixelData->borderXRight, model->pixelData->height);
		glEnd();

		//Border Ytop
		glBegin(GL_LINES);
			glVertex2f(0, model->pixelData->height- model->pixelData->borderYTop);
			glVertex2f(model->pixelData->width, model->pixelData->height - model->pixelData->borderYTop);
		glEnd();

		//Border YBottom
		glBegin(GL_LINES);
			glVertex2f(0, model->pixelData->borderYBottom);
			glVertex2f(model->pixelData->width, model->pixelData->borderYBottom);
		glEnd();

	}

	if (orientation == 2){
		//Border Ytop
		glBegin(GL_LINES);
		glVertex2f(model->pixelData->height - model->pixelData->borderYTop, 0);
		glVertex2f(model->pixelData->height - model->pixelData->borderYTop, model->pixelData->height);
		glEnd();

		//Border YBottom
		glBegin(GL_LINES);
		glVertex2f(  model->pixelData->borderYBottom, 0);
		glVertex2f (model->pixelData->borderYBottom, model->pixelData->height);
		glEnd();

		//Border Zcloser
		glBegin(GL_LINES);
		glVertex2f(0, model->pixelData->borderZCloser* ((float)model->pixelData->frames / (float)model->pixelData->width));
		glVertex2f(model->pixelData->height, model->pixelData->borderZCloser* ((float)model->pixelData->frames / (float)model->pixelData->width));
		glEnd();

		//Border Zfurther
		glBegin(GL_LINES);
		glVertex2f(0, (model->pixelData->frames - model->pixelData->borderZFurther)* ((float)model->pixelData->frames/(float)model->pixelData->width));	//WE need to multply it by this ratio of change so that it shows correctly
		glVertex2f(model->pixelData->height, (model->pixelData->frames - model->pixelData->borderZFurther) * ((float)model->pixelData->frames / (float)model->pixelData->width));
		glEnd();

	}

	if (orientation == 3){
		//Border Zcloser
		glBegin(GL_LINES);
		glVertex2f(0, model->pixelData->borderZCloser* ((float)model->pixelData->frames / (float)model->pixelData->width));
		glVertex2f(model->pixelData->height, model->pixelData->borderZCloser* ((float)model->pixelData->frames / (float)model->pixelData->width));
		glEnd();

		//Border Zfurther
		glBegin(GL_LINES);
		glVertex2f(0, (model->pixelData->frames - model->pixelData->borderZFurther)* ((float)model->pixelData->frames / (float)model->pixelData->width));	//WE need to multply it by this ratio of change so that it shows correctly
		glVertex2f(model->pixelData->height, (model->pixelData->frames - model->pixelData->borderZFurther) * ((float)model->pixelData->frames / (float)model->pixelData->width));
		glEnd();

		//Border XLeft
		glBegin(GL_LINES);
		glVertex2f(model->pixelData->borderXLeft, 0);
		glVertex2f(model->pixelData->borderXLeft, model->pixelData->height);
		glEnd();

		//Border XRight
		glBegin(GL_LINES);
		glVertex2f(model->pixelData->width-model->pixelData->borderXRight, 0);
		glVertex2f(model->pixelData->width - model->pixelData->borderXRight, model->pixelData->height);
		glEnd();
	}
}


/*! \brief Slot que se ejecuta cada vez que se cambia el frame. Dependiendo de la orientacion, los datos en esa direccion del volumen se copiaran en el localFrame
*/
void DicomViewer2DGL::setFrame(int frame)
{
	frame_to_display = frame;
	localFrame.clear();
	
	unsigned char* dataPointer;
	int value = 0;

	int orientation = model->orientation;

	//The value we have goes from o to pow(2,numberOfBytes*8). So in the case of the VHF which has 2 bytes per pixel. it goes from 0 to pow(2,16)=65536
	//WE have to scale it up to 4 bytes (unsigned int) Which max value would be pow(2,4*8). The multiplier would thus be 
	int multiplier = pow(2, 4 * 8) / pow(2,model->pixelData->numberOfBytes * 8);	
	if (orientation == 1){
		//localFrame.reserve(model->pixelDataWidth*model->pixelDataHeight);
		for (int i = 0; i < model->pixelData->height; i++){
			for (int j = 0; j < model->pixelData->width; j++){


				/*dataPointer = &(model->pixelData->data[frame][0]);
				dataPointer = dataPointer + (j + i*model->pixelData->width)*model->pixelData->numberOfBytes;
				memcpy(&value, dataPointer, model->pixelData->numberOfBytes);


				localFrame.push_back(value*multiplier);*/

				localFrame.push_back(model->pixelData->getPixelValue(j, i, frame)*multiplier);
			}
		}
	}

	if (orientation == 2){
		for (int i = 0; i < model->pixelData->height; i++){
			for (int j = 0; j < model->pixelData->width; j++){

				/*dataPointer = &(model->pixelData[i][0]);
				dataPointer = dataPointer + (frame + j*model->pixelDataWidth)*model->numberOfBytes;
				memcpy(&value, dataPointer, model->numberOfBytes);*/


				localFrame.push_back(model->pixelData->getPixelValue(frame, j, i)*multiplier);
			}
		}
	}


	if (orientation == 3){
		for (int i = 0; i < model->pixelData->height; i++){
			for (int j = 0; j < model->pixelData->width; j++){
				localFrame.push_back(model->pixelData->getPixelValue(j, frame, i)*multiplier);
			}
		}
	}




	//ALl of the below is for calculating gradient////////////
	/*

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
	
	multiplier_angle = 11930464; // 2^32 /360
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
	



	//ALL THE CODE BENETH HERE IS TO MAKE OCTREE IN 2D, BUT IT NEEDS TO BE CHANGED SO THAT IT WORKS WITH THE NEW OCTREECUBE CLASS WHICH USES 0,1,2,3 FOR VERTICES INSTED OF TOPRIGHT, TOPlEFT ETC
	
	/*

	cubes.clear();
	//Create initial cube
	//Look inside of it to see if the angle changes enough. If it does subdivide it and add the new cubes in 
	OctreeCube octreeCube;

	

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

				
				//-------------------
				//|   1    !    2   !
				//!        !        !
				//!-----------------
				//!   3    !    4   !
				//!        !        !
				//-------------------
				
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


	

	*/

	

}




