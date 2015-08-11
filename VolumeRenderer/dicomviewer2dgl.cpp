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
	orientation = 1;
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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	
	if (orientation == 1)
		glOrtho(0.0f, pixelData->width, 0.0f, pixelData->height,  0.0f, 1.0f);
	if (orientation == 2)
		glOrtho(0.0f, pixelData->height, 0.0f, pixelData->frames, 0.0f, 1.0f);
	if (orientation == 3)
		glOrtho(0.0f, pixelData->width, 0.0f, pixelData->frames, 0.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	
	glPixelZoom((float)this->width() / (float)pixelData->width, (float)this->height() / (float)pixelData->height);  //used to scale the image down if the image is to big to fit in the window. 


	displayFrame();
	displayBorderLines();
	


}

void DicomViewer2DGL::resizeGL(int w, int h)
{

}



/*! \brief Muestra en el visor 2D los pixels data almacenados en localFrame
*/
void DicomViewer2DGL::displayFrame(){
	if (pixelData->height != 0){

	

		if (!localFrame.empty()){
			if (orientation == 1)	//Normal z orientation
				glDrawPixels(pixelData->width, pixelData->height, GL_LUMINANCE, GL_UNSIGNED_INT, localFrame.data());
			if (orientation == 2)	//X direction
				glDrawPixels(pixelData->height, pixelData->frames, GL_LUMINANCE, GL_UNSIGNED_INT, localFrame.data());
			if (orientation == 3)	//y direction
				glDrawPixels(pixelData->width, pixelData->frames, GL_LUMINANCE, GL_UNSIGNED_INT, localFrame.data());
		}

	}
}



/*! \brief Muestra los planos de corte que se van a usar para la region de interes. Dibuja lineas en rojo para ello
*/
void DicomViewer2DGL::displayBorderLines(){
	if (orientation == 1){
		glColor3f(1.0f, 0.0, 0.0);

		//Border XLeft
		glBegin(GL_LINES);
			glVertex2f(pixelData->borderXLeft, 0);
			glVertex2f(pixelData->borderXLeft,pixelData->height);
		glEnd();

		//Border XRight
		glBegin(GL_LINES);
			glVertex2f(pixelData->width- pixelData->borderXRight, 0);
			glVertex2f(pixelData->width - pixelData->borderXRight, pixelData->height);
		glEnd();

		//Border Ytop
		glBegin(GL_LINES);
			glVertex2f(0, pixelData->height- pixelData->borderYTop);
			glVertex2f(pixelData->width, pixelData->height - pixelData->borderYTop);
		glEnd();

		//Border YBottom
		glBegin(GL_LINES);
			glVertex2f(0, pixelData->borderYBottom);
			glVertex2f(pixelData->width,pixelData->borderYBottom);
		glEnd();

	}

	if (orientation == 2){
		//Border Ytop
		glBegin(GL_LINES);
		glVertex2f(pixelData->height - pixelData->borderYTop, 0);
		glVertex2f(pixelData->height - pixelData->borderYTop, pixelData->height);
		glEnd();

		//Border YBottom
		glBegin(GL_LINES);
		glVertex2f(  pixelData->borderYBottom, 0);
		glVertex2f (pixelData->borderYBottom, pixelData->height);
		glEnd();

		//Border Zcloser
		glBegin(GL_LINES);
		glVertex2f(0, pixelData->borderZCloser* ((float)pixelData->frames / (float)pixelData->width));
		glVertex2f(pixelData->height, pixelData->borderZCloser* ((float)pixelData->frames / (float)pixelData->width));
		glEnd();

		//Border Zfurther
		glBegin(GL_LINES);
		glVertex2f(0, (pixelData->frames - pixelData->borderZFurther)* ((float)pixelData->frames/(float)pixelData->width));	//WE need to multply it by this ratio of change so that it shows correctly
		glVertex2f(pixelData->height, (pixelData->frames - pixelData->borderZFurther) * ((float)pixelData->frames / (float)pixelData->width));
		glEnd();

	}

	if (orientation == 3){
		//Border Zcloser
		glBegin(GL_LINES);
		glVertex2f(0, pixelData->borderZCloser* ((float)pixelData->frames / (float)pixelData->width));
		glVertex2f(pixelData->height, pixelData->borderZCloser* ((float)pixelData->frames / (float)pixelData->width));
		glEnd();

		//Border Zfurther
		glBegin(GL_LINES);
		glVertex2f(0, (pixelData->frames - pixelData->borderZFurther)* ((float)pixelData->frames / (float)pixelData->width));	//WE need to multply it by this ratio of change so that it shows correctly
		glVertex2f(pixelData->height, (pixelData->frames - pixelData->borderZFurther) * ((float)pixelData->frames / (float)pixelData->width));
		glEnd();

		//Border XLeft
		glBegin(GL_LINES);
		glVertex2f(pixelData->borderXLeft, 0);
		glVertex2f(pixelData->borderXLeft,pixelData->height);
		glEnd();

		//Border XRight
		glBegin(GL_LINES);
		glVertex2f(pixelData->width-pixelData->borderXRight, 0);
		glVertex2f(pixelData->width - pixelData->borderXRight, pixelData->height);
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

	//The value we have goes from o to pow(2,numberOfBytes*8). So in the case of the VHF which has 2 bytes per pixel. it goes from 0 to pow(2,16)=65536
	//WE have to scale it up to 4 bytes (unsigned int) Which max value would be pow(2,4*8). The multiplier would thus be 
	int multiplier = pow(2, 4 * 8) / pow(2,pixelData->numberOfBytes * 8);	
	if (orientation == 1){
		for (int i = 0; i < pixelData->height; i++){
			for (int j = 0; j < pixelData->width; j++){


				localFrame.push_back(pixelData->getPixelValue(j, i, frame)*multiplier);
			}
		}
	}

	if (orientation == 2){
		for (int i = 0; i < pixelData->height; i++){
			for (int j = 0; j < pixelData->width; j++){
				localFrame.push_back(pixelData->getPixelValue(frame, j, i)*multiplier);
			}
		}
	}


	if (orientation == 3){
		for (int i = 0; i < pixelData->height; i++){
			for (int j = 0; j < pixelData->width; j++){
				localFrame.push_back(pixelData->getPixelValue(j, frame, i)*multiplier);
			}
		}
	}

	this->update();

}




