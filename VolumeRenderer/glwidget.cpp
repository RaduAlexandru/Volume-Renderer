#include "glwidget.h"
#include <iostream>
#include <QMouseEvent>
/*#define GL_GLEXT_PROTOTYPES
#include <GL/GL.h>
#include <GL/glew.h>*/

//#include <boost/variant.hpp>

GLWidget::GLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	frame_to_display = 0;
	/*angle = 0.0f;
	connect(&timer, SIGNAL(timeout()), this, SLOT(rotate()));
	timer.start(16);*/
	xRot = 0;
	yRot = 0;
	mouseSpeed = 0.5;
	
}

GLWidget::~GLWidget()
{

}

void GLWidget::sendDataToGL(){
	/*GLfloat verts[] = {
		100.0f, 0.0f, 0.0f,
		100.0f, 100.0f, 0.0f,
		0.0f, 100.0f, 0.0f,

		100.0f, 0.0f, 0.0f,
		100.0f, 100.0f, 0.0f,
		150.0f, 50.0f, 600.0f,
	};*/
	std::cout << "creating data and sending to gl" << std::endl;

	/*model->verts.push_back(100.0f);
	model->verts.push_back(0.0f);
	model->verts.push_back(0.0f);

	model->verts.push_back(100.0f);
	model->verts.push_back(100.0f);
	model->verts.push_back(0.0f);

	model->verts.push_back(0.0f);
	model->verts.push_back(100.0f);
	model->verts.push_back(0.0f);

	model->verts.push_back(100.0f);
	model->verts.push_back(0.0f);
	model->verts.push_back(0.0f);

	model->verts.push_back(100.0f);
	model->verts.push_back(100.0f);
	model->verts.push_back(0.0f);

	model->verts.push_back(150.0f);
	model->verts.push_back(50.0f);
	model->verts.push_back(600.0f);*/

	std::cout << "the size of the vector is" << model->verts.size() << std::endl;
	/*for (int i = 0; i < model->verts.size();i++){
		std::cout << "value is" << model->verts[i] <<std::endl;
	}*/

	GLuint vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, model->verts.size()*sizeof(float), model->verts.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);//0 is the positional attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.8, 0.8, 0.8, 0);
	glEnable(GL_DEPTH_TEST);
	dataSended = 0;
	//sendDataToGL();
}

void GLWidget::paintGL()
{
	if (!dataSended &&!model->verts.empty()){
		GLuint vertexBufferID;
		glGenBuffers(1, &vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, model->verts.size()*sizeof(float), model->verts.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);//0 is the positional attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		dataSended = 1;
	}

	/*if (model->verts.empty()){
		std::cout << "no verts" << std::endl;
		return;
	}
	std::cout << "PaintGL" << frame_to_display << std::endl;*/
	//std::cout << "dysplaying frame number" << frame_to_display;
	//glClear(GL_COLOR_BUFFER_BIT);

	//std::cout << "the size of the vector is" << model->verts.size() << std::endl;
	/*for (int i = 0; i < model->verts.size(); i++){
		std::cout << "value is" << model->verts[i] << std::endl;
	}*/


	glViewport(0.0, 0.0, width(), height());
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	glOrtho(0.0f, model->pixelDataWidth, model->pixelDataHeight, 0.0f, 1024.0f,0.0f);	//When you do the 3d one you will need to change the last parameter to be the depth of the image (number of frames)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//if ((boost::get<unsigned short**>(model->pixelData)) != NULL)
	/*if (model->pixelDataHeight != 0){
		if (model->numberOfBytes==1)
			glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, ((model->pixelData))[frame_to_display]);
		if (model->numberOfBytes==2)
			glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_SHORT, ((model->pixelData))[frame_to_display]);
		if (model->numberOfBytes==3)
			glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_INT, ((model->pixelData))[frame_to_display]);

	}*/
	

	glTranslatef((model->pixelDataWidth / 2), (model->pixelDataHeight / 2), -510.0f);
	glRotatef(-xRot, 1, 0, 0);
	glTranslatef(-(model->pixelDataWidth / 2), -(model->pixelDataHeight / 2), 0.0f);

	glTranslatef((model->pixelDataWidth / 2), (model->pixelDataHeight / 2), 0.0f);
	glRotatef(-yRot, 0, 0, 1);	//We put it to minus so that the rotation is reversed
	glTranslatef(-(model->pixelDataWidth / 2), -(model->pixelDataHeight / 2), 0.0f - model->frames/2);
	
	//glPointSize(1.5f);
	glColor3f(1.f, 0.f, 0.f);
	glDrawArrays(GL_TRIANGLES, 0, model->verts.size());

	/*glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(model->pixelDataWidth/2, 0.0f, 0.0f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(model->pixelDataWidth, model->pixelDataHeight, 0.0f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.0f, model->pixelDataHeight, 0.0f);

	glVertex3f(model->pixelDataWidth / 2, 0.0f, 0.0f);
	glVertex3f(model->pixelDataWidth / 2+50, 50.0f, -50.0f);
	glVertex3f(model->pixelDataWidth, model->pixelDataHeight, 0.0f);

	glEnd();*/

	/*glColor3f(0.f, 1.f, 0.f);
	glBegin(GL_LINES);
	for (int i = 0; i < model->normals.size(); i=i+6){
		glVertex3f(model->normals[i], model->normals[i+1], model->normals[i+2]);
		glVertex3f(model->normals[i+3], model->normals[i + 4], model->normals[i + 5]);
	}
	glEnd();*/
	
}

void GLWidget::resizeGL(int w, int h)
{

}

void GLWidget::setFrame(int frame)
{
	//std::cout << "we set the frame to " << frame << std::endl;
	model->frame_to_display = frame;
	frame_to_display = frame; //We actually have to make something like frameToDisplay=frame% model-> numberOfFrameInDicomFile
	//std::cout << "the first few pixels have value" << (int)model->pixelData[frame_to_display][1] << std::endl;
	
}


/*void GLWidget::rotate(){
	angle += 2.0f;
	if (angle > 360) {
		angle -= 360;
	}
	update();
}*/


//Whenever the mouse is dragged over the glwidget, we calculate the position in that widget and we 
//make the difference between where you dragged to and where originally clicked. Multiply it by the 
//mouseSpeed that controls the speed of rotations and then we update the point where we originally clicked (mouseEntered)
void GLWidget::mouseMoveEvent(QMouseEvent * event){
	int xpos=0, ypos=0;
	
	xpos = event->x();
	ypos = event->y();
	
	yRot += (xpos - mouseXPosEntered)*mouseSpeed;
	xRot += (ypos - mouseYPosEntered)*mouseSpeed;

	if (yRot > 360) {
		yRot -= 360;
	}
	if (xRot > 360) {
		xRot -= 360;
	}
	
	mouseXPosEntered = xpos;
	mouseYPosEntered = ypos;
	update();
}

void GLWidget::mousePressEvent(QMouseEvent *event){
	mouseXPosEntered = event->x();
	mouseYPosEntered = event->y();
}