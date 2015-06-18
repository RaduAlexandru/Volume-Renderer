#include "glwidget.h"
#include <iostream>
#include <QMouseEvent>
#include <gl/GLU.h>
#define SHIFT 16777248
#define CTRL 16777249
#define PI 3.14159265
/*#define GL_GLEXT_PROTOTYPES
#include <GL/GL.h>
#include <GL/glew.h>*/

//#include <boost/variant.hpp>

GLWidget::GLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{
	this->setFocusPolicy(Qt::StrongFocus);
	frame_to_display = 0;
	/*angle = 0.0f;
	connect(&timer, SIGNAL(timeout()), this, SLOT(rotate()));
	timer.start(16);*/
	xRot = -90;
	yRot = 0;
	mouseSpeed = 0.5;
	xMove = 0;
	yMove = 0;
	zMove = 0;
	shiftPressed = false;
	ctrlPressed = false;
	
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
	
	

	glEnable(GL_LIGHTING);

	//WE ADD THE LIGHTS
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	
	//Light 1 is a point light, an omni light
	float lightColor[4] = { 0.5, 0.5, 0.5, 1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

	//Light 2 a blue keylight
	float lightColor2[4] = { 0.5, 0.5, 0.5, 1 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor2);

	//Light 3 is just above the face
	float lightColor3[4] = { 1.3, 1.3, 1.3, 1 };
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor3);
	




	

	//ambient light
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color(0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);


	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1);
	glFrontFace(GL_CCW);

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);*/
	

	readBackgroundImage();
	glShadeModel(GL_SMOOTH);

	
	
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
		//xRot = -90;
	}

	
	glViewport(0.0, 0.0, width(), height());
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setMatrices();



	//if ((boost::get<unsigned short**>(model->pixelData)) != NULL)
	/*if (model->pixelDataHeight != 0){
		if (model->numberOfBytes==1)
			glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, ((model->pixelData))[frame_to_display]);
		if (model->numberOfBytes==2)
			glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_SHORT, ((model->pixelData))[frame_to_display]);
		if (model->numberOfBytes==3)
			glDrawPixels(model->pixelDataWidth, model->pixelDataHeight, GL_LUMINANCE, GL_UNSIGNED_INT, ((model->pixelData))[frame_to_display]);

	}*/
	
	drawBackground();

	/*glTranslatef((model->pixelDataWidth / 2), (model->pixelDataHeight / 2), -(510.0f));
	glRotatef(-xRot, 1, 0, 0);
	glTranslatef(-(model->pixelDataWidth / 2), -(model->pixelDataHeight / 2), 0.0f);

	glTranslatef((model->pixelDataWidth / 2), (model->pixelDataHeight / 2), 0.0f);
	glRotatef(yRot, 0, 0, 1);	//We put it to minus so that the rotation is reversed
	glTranslatef(-(model->pixelDataWidth / 2), -(model->pixelDataHeight / 2), 0.0f - (model->frames / 2));*/
	
	//THe above one is the one that works for rotation, the below one sorta works but only in some directions, I don't know it'w weird
	/*glTranslatef((model->pixelDataWidth / 2 +  xMoveOld), (model->pixelDataHeight / 2 + yMoveOld), -(510.0f+ zMoveOld));
	glRotatef(-xRot, 1, 0, 0);
	glTranslatef(-(model->pixelDataWidth / 2 + xMove + xMoveOld ), -(model->pixelDataHeight / 2 + yMove + yMoveOld), 0.0f +zMove);

	glTranslatef((model->pixelDataWidth / 2 + xMove), (model->pixelDataHeight / 2 + yMove), 0.0f + zMove);
	glRotatef(yRot, 0, 0, 1);	//We put it to minus so that the rotation is reversed
	glTranslatef(-(model->pixelDataWidth / 2 + xMove), -(model->pixelDataHeight / 2 + yMove), 0.0f - (model->frames/2 + zMove));*/


	//std::cout << "xrot is" << xRot << std::endl;

	glTranslatef((model->pixelData->width / 2), (model->pixelData->height / 2 - yMove), -(510.0f) + zMove);
	//glScalef(scale, scale, scale);
	glRotatef(xRot, 1, 0, 0);
	glTranslatef(-(model->pixelData->width / 2  -xMove), -(model->pixelData->height / 2  ), 0.0f );

	glTranslatef((model->pixelData->width / 2 ), (model->pixelData->height / 2 ), 0.0f  );
	glRotatef(yRot, 0, 0, 1);	//We put it to minus so that the rotation is reversed
	glTranslatef(-(model->pixelData->width / 2 ), -(model->pixelData->height / 2 ), 0.0f - (model->pixelData->frames / 2 ));

	
	
	//glPointSize(1.5f);

	/*FLOAT LightAmbient[4] = { 0, 0, 0, 1 };
	FLOAT LightDiffuse[4] = { 1, 1, 1, 1 };
	FLOAT LightPosition[4] = { 0, 0, 1, 0 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);

	glEnable(GL_LIGHTING);


	glShadeModel(GL_SMOOTH);*/


	
	//If you copy all these position in the lines before the rotations (after draw background), the liths will rotate with the camera (like if you are holding a flashlight). not with the mesh
	//Position of light 1
	float lightPosition[4] = { 0, 0, 0, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	//Position of light 2
	float lightPosition1[4] = { model->pixelData->width, model->pixelData->height/8, 0, 1 };
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);

	//Position of light 3
	float lightPosition2[4] = { model->pixelData->width/2, model->pixelData->height/1.1, 0, 1 };
	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);



	/*glColor3f(1.f, 0.f, 0.f);
	glPointSize(1.0);
	glBegin(GL_POINTS);
	if (!model->gradientPoints.empty())
		for (int i = 0; i < model->gradientPoints.size() - 3; i = i + 3){
		glVertex3f(model->gradientPoints[i], model->gradientPoints[i + 1], model->gradientPoints[i + 2]);
		}
	glEnd();*/

	
	//glDrawArrays(GL_TRIANGLES, 0, model->verts.size());
	glColor3f(0.55f, 0.55f, 0.55f);
	if (model->showMesh)
		drawMesh();

	glColor3f(1.f, 0.f, 0.f);
	if (model->showGradient)
		displayGradient();

	if (model->showCubes)
		drawCubes2();


	if (model->showWireframe){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.5f);
		glColor3f(1.f, 0.f, 0.f);
		drawMesh();
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	//Draw the point for debugging
	glPointSize(6.0f);
	glColor3f(0.f, 0.f, 1.f);
	glBegin(GL_POINTS);
	glVertex3f(model->xPosPoint, model->yPosPoint, model->zPosPoint);
	glEnd();

	//240, 160, 48 of size 16

	//another cube with problems cube
	//x=320-8
	//y=192
	//z=32
	//size=8

	//another cube with problems cube
	//x=256
	//y=168
	//z=56
	//size=8
 
	//Draw the cube for debugging
	int x, y, z, size;
	x = 256;
	y = 168;
	z = 56;
	size = 8;

	glColor3f(0.f, 0.f, 1.f);
	glLineWidth(2.5f);
	glBegin(GL_LINES);

	

		glVertex3f(x, y, z);
		glVertex3f(x + size, y, z);

		glVertex3f(x, y, z);
		glVertex3f(x, y + size, z);

		glVertex3f(x, y, z);
		glVertex3f(x, y, z + size);


		glVertex3f(x + size, y + size, z + size);
		glVertex3f(x, y + size, z +size);

		glVertex3f(x + size, y + size, z + size);
		glVertex3f(x + size, y, z + size);

		glVertex3f(x + size, y + size, z + size);
		glVertex3f(x +size, y + size, z);




		glVertex3f(x, y + size, z);
		glVertex3f(x, y + size, z + size);

		glVertex3f(x, y + size, z + size);
		glVertex3f(x, y, z + size);

		glVertex3f(x + size, y, z);
		glVertex3f(x + size, y, z + size);

		glVertex3f(x + size, y,z + size);
		glVertex3f(x, y, z + size);

		glVertex3f(x + size, y, z);
		glVertex3f(x + size,y + size, z);

		glVertex3f(x, y + size,z);
		glVertex3f(x + size, y + size, z);

	glEnd();





	x = 256;
	y = 168;
	z = 56 - 8;
	size = 8;

	glColor3f(0.f, 1.f, 0.f);
	glLineWidth(2.5f);
	glBegin(GL_LINES);



	glVertex3f(x, y, z);
	glVertex3f(x + size, y, z);

	glVertex3f(x, y, z);
	glVertex3f(x, y + size, z);

	glVertex3f(x, y, z);
	glVertex3f(x, y, z + size);


	glVertex3f(x + size, y + size, z + size);
	glVertex3f(x, y + size, z + size);

	glVertex3f(x + size, y + size, z + size);
	glVertex3f(x + size, y, z + size);

	glVertex3f(x + size, y + size, z + size);
	glVertex3f(x + size, y + size, z);




	glVertex3f(x, y + size, z);
	glVertex3f(x, y + size, z + size);

	glVertex3f(x, y + size, z + size);
	glVertex3f(x, y, z + size);

	glVertex3f(x + size, y, z);
	glVertex3f(x + size, y, z + size);

	glVertex3f(x + size, y, z + size);
	glVertex3f(x, y, z + size);

	glVertex3f(x + size, y, z);
	glVertex3f(x + size, y + size, z);

	glVertex3f(x, y + size, z);
	glVertex3f(x + size, y + size, z);

	glEnd();

	//Draw face for debugging

	/*x = 240;
	y = 160;
	z = 48 ;
	size = 16;

	glColor3f(0.f, 1.f, 0.f);
	glLineWidth(2.5f);
	glBegin(GL_LINES);

	glVertex3f(x, y, z);
	glVertex3f(x + size, y, z);

	glVertex3f(x + size, y, z);
	glVertex3f(x + size, y + size, z);

	glVertex3f(x + size, y + size, z);
	glVertex3f(x, y + size, z);

	glVertex3f(x, y + size, z);
	glVertex3f(x, y, z);

	glEnd();*/
	

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


void GLWidget::drawMesh(){
	//glColor3f(1.f, 0.f, 0.f);
	
	//glDrawArrays(GL_TRIANGLES, 0, model->verts.size());
	if (model->verts.empty() || model->normals.empty())
		return;


	//std::cout << "gneralting mesh is " << model->generatingMesh << std::endl;
	if (model->generatingMesh == true)
		return;

	int normalIndex = 0;

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < model->verts.size() - 3; i = i + 3){

		glNormal3f(model->normals[i].x, model->normals[i].y, model->normals[i].z);
		glVertex3f(model->verts[i].x, model->verts[i].y, model->verts[i].z);

		glNormal3f(model->normals[i + 1].x, model->normals[i + 1].y, model->normals[i + 1].z);
		glVertex3f(model->verts[i + 1].x, model->verts[i + 1].y, model->verts[i + 1].z);

		glNormal3f(model->normals[i + 2].x, model->normals[i + 2].y, model->normals[i + 2].z);
		glVertex3f(model->verts[i + 2].x, model->verts[i + 2].y, model->verts[i + 2].z);
	}
	glEnd();



	/*if (model->normalsAlgChosen == 1){
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < model->verts.size() - 9; i = i + 9){

			if (normalIndex >= model->normals.size()){
				break;
			}

			glNormal3f(model->normals[normalIndex], model->normals[normalIndex + 1], model->normals[normalIndex + 2]);
			normalIndex = normalIndex + 3;
			//glNormal3f(0, 0, 1);
			glVertex3f(model->verts[i], model->verts[i + 1], model->verts[i + 2]);
			glVertex3f(model->verts[i + 3], model->verts[i + 4], model->verts[i + 5]);
			glVertex3f(model->verts[i + 6], model->verts[i + 7], model->verts[i + 8]);
		}
		glEnd();
	}

	//Representation with normal for each vertice
	if (model->normalsAlgChosen == 2){
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < model->verts.size() - 9; i = i + 9){
			glNormal3f(model->normals[i], model->normals[i + 1], model->normals[i + 2]);
			glVertex3f(model->verts[i], model->verts[i + 1], model->verts[i + 2]);

			glNormal3f(model->normals[i + 3], model->normals[i + 4], model->normals[i + 5]);
			glVertex3f(model->verts[i + 3], model->verts[i + 4], model->verts[i + 5]);

			glNormal3f(model->normals[i + 6], model->normals[i + 7], model->normals[i + 8]);
			glVertex3f(model->verts[i + 6], model->verts[i + 7], model->verts[i + 8]);
		}
		glEnd();
	}*/

	

}
void GLWidget::drawCubes(){
	//Draw the cubes now The cube configuracion can be found as a comment in the adaptiveMarchingCubes function
	
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	for (int i = 0; i < model->cubes.size(); i = i + 1){

		if (!model->cubes[i].isLeaf){
			continue;
		}
		//240, 160, 48 of size 16

		

		glVertex3f(model->cubes[i].origin.x, model->cubes[i].origin.y, model->cubes[i].origin.z);
		glVertex3f(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y, model->cubes[i].origin.z);

		glVertex3f(model->cubes[i].origin.x, model->cubes[i].origin.y, model->cubes[i].origin.z);
		glVertex3f(model->cubes[i].origin.x, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z);

		glVertex3f(model->cubes[i].origin.x, model->cubes[i].origin.y, model->cubes[i].origin.z);
		glVertex3f(model->cubes[i].origin.x, model->cubes[i].origin.y, model->cubes[i].origin.z + model->cubes[i].sizeZ);


		glVertex3f(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z + model->cubes[i].sizeZ);
		glVertex3f(model->cubes[i].origin.x, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z + model->cubes[i].sizeZ);

		glVertex3f(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z + model->cubes[i].sizeZ);
		glVertex3f(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y, model->cubes[i].origin.z + model->cubes[i].sizeZ);

		glVertex3f(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z + model->cubes[i].sizeZ);
		glVertex3f(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z);




		glVertex3f(model->cubes[i].origin.x, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z);
		glVertex3f(model->cubes[i].origin.x, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z + model->cubes[i].sizeZ);

		glVertex3f(model->cubes[i].origin.x, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z + model->cubes[i].sizeZ);
		glVertex3f(model->cubes[i].origin.x, model->cubes[i].origin.y, model->cubes[i].origin.z + model->cubes[i].sizeZ);

		glVertex3f(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y, model->cubes[i].origin.z);
		glVertex3f(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y, model->cubes[i].origin.z + model->cubes[i].sizeZ);

		glVertex3f(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y, model->cubes[i].origin.z + model->cubes[i].sizeZ);
		glVertex3f(model->cubes[i].origin.x, model->cubes[i].origin.y, model->cubes[i].origin.z + model->cubes[i].sizeZ);

		glVertex3f(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y, model->cubes[i].origin.z);
		glVertex3f(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z);

		glVertex3f(model->cubes[i].origin.x, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z);
		glVertex3f(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z);


	}
	glEnd();
}


//This one get the octreecubes from octreevector which is the one that gets used when adaptivemarchingvubes3
void GLWidget::drawCubes2(){
	//Draw the cubes now The cube configuracion can be found as a comment in the adaptiveMarchingCubes function

	glBegin(GL_LINES);
	for (int i = 0; i < model->octreeVector.size(); i = i + 1){

		/*if (!model->octreeVector[i]->isLeaf){
			continue;
		}*/

		if (!model->octreeVector[i]->containsVerts){
			continue;
		}

		glVertex3f(model->octreeVector[i]->origin.x, model->octreeVector[i]->origin.y, model->octreeVector[i]->origin.z);
		glVertex3f(model->octreeVector[i]->origin.x + model->octreeVector[i]->sizeX, model->octreeVector[i]->origin.y, model->octreeVector[i]->origin.z);

		glVertex3f(model->octreeVector[i]->origin.x, model->octreeVector[i]->origin.y, model->octreeVector[i]->origin.z);
		glVertex3f(model->octreeVector[i]->origin.x, model->octreeVector[i]->origin.y + model->octreeVector[i]->sizeY, model->octreeVector[i]->origin.z);

		glVertex3f(model->octreeVector[i]->origin.x, model->octreeVector[i]->origin.y, model->octreeVector[i]->origin.z);
		glVertex3f(model->octreeVector[i]->origin.x, model->octreeVector[i]->origin.y, model->octreeVector[i]->origin.z + model->octreeVector[i]->sizeZ);


		glVertex3f(model->octreeVector[i]->origin.x + model->octreeVector[i]->sizeX, model->octreeVector[i]->origin.y + model->octreeVector[i]->sizeY, model->octreeVector[i]->origin.z + model->octreeVector[i]->sizeZ);
		glVertex3f(model->octreeVector[i]->origin.x, model->octreeVector[i]->origin.y + model->octreeVector[i]->sizeY, model->octreeVector[i]->origin.z + model->octreeVector[i]->sizeZ);

		glVertex3f(model->octreeVector[i]->origin.x + model->octreeVector[i]->sizeX, model->octreeVector[i]->origin.y + model->octreeVector[i]->sizeY, model->octreeVector[i]->origin.z + model->octreeVector[i]->sizeZ);
		glVertex3f(model->octreeVector[i]->origin.x + model->octreeVector[i]->sizeX, model->octreeVector[i]->origin.y, model->octreeVector[i]->origin.z + model->octreeVector[i]->sizeZ);

		glVertex3f(model->octreeVector[i]->origin.x + model->octreeVector[i]->sizeX, model->octreeVector[i]->origin.y + model->octreeVector[i]->sizeY, model->octreeVector[i]->origin.z + model->octreeVector[i]->sizeZ);
		glVertex3f(model->octreeVector[i]->origin.x + model->octreeVector[i]->sizeX, model->octreeVector[i]->origin.y + model->octreeVector[i]->sizeY, model->octreeVector[i]->origin.z);




		glVertex3f(model->octreeVector[i]->origin.x, model->octreeVector[i]->origin.y + model->octreeVector[i]->sizeY, model->octreeVector[i]->origin.z);
		glVertex3f(model->octreeVector[i]->origin.x, model->octreeVector[i]->origin.y + model->octreeVector[i]->sizeY, model->octreeVector[i]->origin.z + model->octreeVector[i]->sizeZ);

		glVertex3f(model->octreeVector[i]->origin.x, model->octreeVector[i]->origin.y + model->octreeVector[i]->sizeY, model->octreeVector[i]->origin.z + model->octreeVector[i]->sizeZ);
		glVertex3f(model->octreeVector[i]->origin.x, model->octreeVector[i]->origin.y, model->octreeVector[i]->origin.z + model->octreeVector[i]->sizeZ);

		glVertex3f(model->octreeVector[i]->origin.x + model->octreeVector[i]->sizeX, model->octreeVector[i]->origin.y, model->octreeVector[i]->origin.z);
		glVertex3f(model->octreeVector[i]->origin.x + model->octreeVector[i]->sizeX, model->octreeVector[i]->origin.y, model->octreeVector[i]->origin.z + model->octreeVector[i]->sizeZ);

		glVertex3f(model->octreeVector[i]->origin.x + model->octreeVector[i]->sizeX, model->octreeVector[i]->origin.y, model->octreeVector[i]->origin.z + model->octreeVector[i]->sizeZ);
		glVertex3f(model->octreeVector[i]->origin.x, model->octreeVector[i]->origin.y, model->octreeVector[i]->origin.z + model->octreeVector[i]->sizeZ);

		glVertex3f(model->octreeVector[i]->origin.x + model->octreeVector[i]->sizeX, model->octreeVector[i]->origin.y, model->octreeVector[i]->origin.z);
		glVertex3f(model->octreeVector[i]->origin.x + model->octreeVector[i]->sizeX, model->octreeVector[i]->origin.y + model->octreeVector[i]->sizeY, model->octreeVector[i]->origin.z);

		glVertex3f(model->octreeVector[i]->origin.x, model->octreeVector[i]->origin.y + model->octreeVector[i]->sizeY, model->octreeVector[i]->origin.z);
		glVertex3f(model->octreeVector[i]->origin.x + model->octreeVector[i]->sizeX, model->octreeVector[i]->origin.y + model->octreeVector[i]->sizeY, model->octreeVector[i]->origin.z);


	}
	glEnd();
}



void GLWidget::readBackgroundImage(){
	FILE *f = fopen("test9_background3.bmp", "rb");
	if (!f) {
		printf("failed to open file\n");
		exit(0);
	}

	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	//fread(signature, 1, 2, f);
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, f);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, f);
	fseek(f, bfh.bfOffBits, SEEK_SET);	//The offbits is the byte where the pixels start so we move the file pointer there and we read from it

	
	backgroundWidth = bih.biWidth;
	backgroundHeight = bih.biHeight;
	int bitmapNumberOfPixels = bih.biWidth * bih.biHeight;
	int bitmapImageSize = sizeof(char)* bitmapNumberOfPixels;
	

	
	background = (unsigned char *)malloc(bitmapImageSize);

	fread(background, 1, bitmapImageSize, f);
}

void GLWidget::drawBackground(){


	glPixelZoom((float)this->width() / (float)backgroundWidth, (float)this->height() / (float)backgroundHeight);  //used to scale the image down if the image is to big to fit in the window. 
	
	glDrawPixels(backgroundWidth, backgroundHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, background);
			
				

	glClear(GL_DEPTH_BUFFER_BIT);
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


void GLWidget::displayGradient(){


	if (model->gradient.empty())
		return;

	
	std::vector<glm::vec3> keys;	//The position
	std::vector<glm::vec3> vals;	//the gradient


	//We read values only if keys is emty
	if (keys.empty())
	for (int k = 0; k < model->pixelData->frames; k++){
		for (auto kv : model->gradient[k]) {
			glm::vec3 positionKey;
			positionKey.x = kv.first.first;
			positionKey.y = kv.first.second;
			positionKey.z = k;
			keys.push_back(positionKey);
			vals.push_back(kv.second);
		}
	}

	
	/*glDisable(GL_LIGHTING);
	int valsIndex = 0;
	for (int i = 0; i < keys.size(); i=i+3){
		glColor3f((sin(vals[valsIndex].x *PI / 180) + 1) / 2, (sin(vals[valsIndex].y *PI / 180) + 1) / 2, (sin(vals[valsIndex].z *PI / 180) + 1) / 2);	//The input for sin is in radians so we do *PI / 180. The output os sin is from -1 to 1. So we add 1 and then divide by 2 so we get a value from 0 to 1
		glPointSize(3.0f);
		glBegin(GL_POINTS);
		glVertex3f(keys[i], keys[i+1], keys[i+2]);
		glEnd();
		valsIndex++;
	}
	glEnable(GL_LIGHTING);
	return;*/

	int skip=0;

	glDisable(GL_LIGHTING);
	glPointSize(2.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < keys.size(); i++){
		/*if (skip != 5){
			skip++;
			continue;
		}*/
		glColor3f((sin(vals[i].x  *PI / 180) + 1) / 2, (sin(vals[i].y *PI / 180) + 1) / 2, (sin(vals[i].z *PI / 180) + 1) / 2);
		
		glVertex3f(keys[i].x, keys[i].y, keys[i].z);
		
		//skip = 0;
	}
	glEnd();
	glEnable(GL_LIGHTING);

	///////////////

	
	

}

void GLWidget::setMatrices(){

	if (model->perspectiveActivated){
		//glFrustum(0.0f, 1, 0.0f,1, 1.0f, 1024.0f);	//Also works but it would need some tweaking and also probably commenting the GlulookAt
		gluPerspective(60.0, (float)width() / height(), 1.0, -1024.0); // Set perspective
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//Now we position the camera in the middle of the screen (width/2 and heidth /2 ) and looking at the negative z axis
		gluLookAt(model->pixelData->width / 2, model->pixelData->height / 2, model->pixelData->frames,
			model->pixelData->width / 2, model->pixelData->height / 2, -model->pixelData->frames,
			0.0, 1.0, 0.0);
	}
	else{
		//glOrtho(0.0f, model->pixelDataWidth, 0.0f,  model->pixelDataHeight, -1024.0f, 1024.0f);	//When you do the 3d one you will need to change the last parameter to be the depth of the image (number of frames)
		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();



		float aspectRatio = (GLfloat)width() / (GLfloat)height();
		if (width() <= height())
			glOrtho(0.0f, model->pixelData->width, 0.0f - (model->pixelData->height* (aspectRatio - 1)), model->pixelData->height / aspectRatio, -1024.0f, 1024.0f);
		if (width() > height())
			glOrtho(0.0f - (model->pixelData->width* (aspectRatio-1)), model->pixelData->width*aspectRatio, 0.0f, model->pixelData->height, -1024.0f, 1024.0f);
		else
			glOrtho(0.0f, model->pixelData->width, 0.0f, model->pixelData->height, -1024.0f, 1024.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();


	}
}

//Whenever the mouse is dragged over the glwidget, we calculate the position in that widget and we 
//make the difference between where you dragged to and where originally clicked. Multiply it by the 
//mouseSpeed that controls the speed of rotations and then we update the point where we originally clicked (mouseEntered)
void GLWidget::mouseMoveEvent(QMouseEvent * event){
	int xpos=0, ypos=0;
	
	xpos = event->x();
	ypos = event->y();
	int incX=1, incY=1;
	
	//std::cout << "Moving mouse and value of CTRL is "  << ctrlPressed << std::endl;
	if (shiftPressed){
		//std::cout << "Moving mouse while shift is pressed" << std::endl;
		xMove += (xpos - mouseXPosEntered);
		yMove += (ypos - mouseYPosEntered);
		mouseXPosEntered = xpos;
		mouseYPosEntered = ypos;
		update();
		return;
	}

	if (ctrlPressed){
		//std::cout << "Moving mouse while ctrl is pressed" << std::endl;
		//incX += (xpos - mouseXPosEntered);
		//incY += (ypos - mouseYPosEntered);
		//scale = sqrt(incX*incX + incY*incY)*0.1;
		//scale = incX*0.1;
		zMove += (xpos - mouseXPosEntered);
		mouseXPosEntered = xpos;
		mouseYPosEntered = ypos;
		update();
		return;
	}


	yRot += (xpos - mouseXPosEntered)*mouseSpeed;
	xRot += (ypos - mouseYPosEntered)*mouseSpeed;

	if (yRot > 360) {
		yRot -= 360;
	}
	if (xRot > 360) {
		xRot -= 360;
	}
	
	//std::cout << "xrot is " << xRot << " yrot is " << yRot << std::endl;

	mouseXPosEntered = xpos;
	mouseYPosEntered = ypos;
	update();
}

void GLWidget::mousePressEvent(QMouseEvent *event){
	mouseXPosEntered = event->x();
	mouseYPosEntered = event->y();
}



void GLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
	std::cout << "Pressed key: " << keyEvent->key() << std::endl;
	//keyEvent->ignore();
	if (keyEvent->key() == SHIFT)
		shiftPressed = true;
	if (keyEvent->key() == CTRL)
		ctrlPressed = true;
		
}

void GLWidget::keyReleaseEvent(QKeyEvent *keyEvent)
{
	std::cout << "Released key: " << keyEvent->key() << std::endl;
	if (keyEvent->key() == SHIFT)
		shiftPressed = false;
	if (keyEvent->key() == CTRL)
		ctrlPressed = false;
}

void  GLWidget::dataFinishedReading(){
	std::cout << "received the signal that all the data was read" << std::endl;
	xRot = -90;
}