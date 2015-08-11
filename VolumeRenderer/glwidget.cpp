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

	xRot = -90;
	yRot = 0;
	mouseSpeed = 0.5;
	xMove = 0;
	yMove = 0;
	zMove = 0;
	movingFigure = false;
	scalingFigure = false;
	opacity = 1.0;

	showPerspective = true;
	showMesh = true;
	showWireframe = false;
	showCubes = false;
	showGradients = false;
	mesh = NULL;
	mesh2 = NULL;
	
}

GLWidget::~GLWidget()
{

}


void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0.8, 0.8, 0.8, 0);
	glEnable(GL_DEPTH_TEST);
	
	glFrontFace(GL_CW);
	

	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	glEnable(GL_COLOR_MATERIAL);
	
	

	//WE ADD THE LIGHTS
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	
	//glEnable(GL_LIGHT3);

	//Light 1 is a point light, an omni light
	float lightColor[4] = { 0.5, 0.5, 0.5, 1 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);

	//Light 2 a blue keylight
	float lightColor2[4] = { 0.5, 0.5, 0.5, 1 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor2);

	//Light 3 is just above the face
	float lightColor3[4] = { 1.3, 1.3, 1.3, 1 };
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor3);
	

	/*float lightColor4[4] = { 0.3, 0.3, 0.3, 1 };
	float specReflection[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	glMaterialfv(GL_BACK, GL_SPECULAR, specReflection);
	glMateriali(GL_BACK, GL_SHININESS, 56);
	glLightfv(GL_LIGHT3, GL_SPECULAR, lightColor4);*/


	

	//ambient light
	GLfloat ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f }; //Color(0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);


	

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);*/
	

	readBackgroundImage();
	glShadeModel(GL_SMOOTH);

	generatingMesh = false;
	
	//Activar el calculo de transparencias
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0, 0.0, 0);
	
	
}

void GLWidget::paintGL()
{
	
	//si se esta generando un mallado no podemos recorrer o representar ningun mallado ya que algun algoritmo está escribiendo en el vector de pixels
	if (generatingMesh)
		return;

	if (mesh == NULL);
	
	
	glViewport(0.0, 0.0, width(), height());
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	setMatrices();



	
	
	drawBackground();



	
	//Rotar, girar y escalar la figura
	glTranslatef((mesh->width / 2), (mesh->height / 2 - yMove), -(510.0f) + zMove);
	glRotatef(xRot, 1, 0, 0);
	glTranslatef(-(mesh->width / 2 - xMove), -(mesh->height / 2), 0.0f);

	glTranslatef((mesh->width / 2), (mesh->height / 2), 0.0f);
	glRotatef(yRot, 0, 0, 1);	
	glTranslatef(-(mesh->width / 2), -(mesh->height / 2), 0.0f - (mesh->frames / 2));

	
	
	


	
	//If you copy all these position in the lines before the rotations (after draw background), the lights will rotate with the camera (like if you are holding a flashlight). not with the mesh
	//Position of light 1
	float lightPosition[4] = { 0, 0, 0, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

	//Position of light 2
	float lightPosition1[4] = { mesh->width, mesh->height / 8, 0, 1 };
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);

	//Position of light 3
	float lightPosition2[4] = { mesh->width / 2, mesh->height / 1.1, 0, 1 };
	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);



	float lightPosition3[4] = { mesh->width / 2, mesh->height / 1.1, 0, 0 };
	glLightfv(GL_LIGHT3, GL_POSITION, lightPosition3);


	//Dibujar la figura secundaria primero ya que la primaria se tiene que dibujar después para que transparente sobre la otra
	glColor4f(1.0f, 0.9f, 0.65f, 1.0f);
	if (showMesh)
		drawMesh(mesh2);
	
	
	glColor4f(0.55f, 0.55f, 0.55f,float (opacity));
	if (showMesh)
		drawMesh(mesh);

	

	glColor3f(1.f, 0.f, 0.f);
	if (showGradients)
		displayGradient();


	if (showWireframe){
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.5f);
		glColor3f(1.f, 0.f, 0.f);
		drawMesh(mesh);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	else{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	
	
}


/*! \brief Funcion que muestra el mallado por pantalla. Se recorre el vector de puntos y de normales de 3 en 3, formandose triangulos con sus normales correspondientes
*/
void GLWidget::drawMesh(Mesh* meshtoDraw){
	//glColor3f(1.f, 0.f, 0.f);
	
	if (meshtoDraw == NULL)
		return;

	//glDrawArrays(GL_TRIANGLES, 0, model->verts.size());
	if (meshtoDraw->verts.empty() || meshtoDraw->normals.empty())
		return;


	//std::cout << "gneralting mesh is " << model->generatingMesh << std::endl;
	if (generatingMesh == true)
		return;

	int normalIndex = 0;

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < meshtoDraw->verts.size() - 3; i = i + 3){

		if (i >= meshtoDraw->normals.size())
			break;

		glNormal3f(meshtoDraw->normals[i].x, meshtoDraw->normals[i].y, meshtoDraw->normals[i].z);
		glVertex3f(meshtoDraw->verts[i].x, meshtoDraw->verts[i].y, meshtoDraw->verts[i].z);

		glNormal3f(meshtoDraw->normals[i + 1].x, meshtoDraw->normals[i + 1].y, meshtoDraw->normals[i + 1].z);
		glVertex3f(meshtoDraw->verts[i + 1].x, meshtoDraw->verts[i + 1].y, meshtoDraw->verts[i + 1].z);

		glNormal3f(meshtoDraw->normals[i + 2].x, meshtoDraw->normals[i + 2].y, meshtoDraw->normals[i + 2].z);
		glVertex3f(meshtoDraw->verts[i + 2].x, meshtoDraw->verts[i + 2].y, meshtoDraw->verts[i + 2].z);
	}
	glEnd();


}






//This one get the octreecubes from octreevector which is the one that gets used when adaptivemarchingvubes3
/*void GLWidget::drawCubes2(){
	//Draw the cubes now The cube configuracion can be found as a comment in the adaptiveMarchingCubes function

	glBegin(GL_LINES);
	for (int i = 0; i < model->octreeVector.size(); i = i + 1){

		//if (!model->octreeVector[i]->isLeaf){
		//	continue;
		//}

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
}*/





/*! \brief Lee un bitmap para representarlo como fondo del vison en 3D
*/
void GLWidget::readBackgroundImage(){
	FILE *f = fopen("./images/test9_background3.bmp", "rb");
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




/*void GLWidget::rotate(){
	angle += 2.0f;
	if (angle > 360) {
		angle -= 360;
	}
	update();
}*/


/*! \brief Recorre el mapa de los gradientes y por cada direccion de gradiente se asigna un color en RGB para mostrarlo por pantalla
*/
void GLWidget::displayGradient(){


	if (gradient->empty())
		return;

	
	std::vector<glm::vec3> keys;	//The position
	std::vector<glm::vec3> vals;	//the gradient


	//We read values only if keys is emty
	if (keys.empty())
	for (int k = 0; k < mesh->frames; k++){
		for (auto kv : (*gradient)[k]) {
			glm::vec3 positionKey;
			positionKey.x = kv.first.first;
			positionKey.y = kv.first.second;
			positionKey.z = k;
			keys.push_back(positionKey);
			vals.push_back(kv.second);
		}
	}

	
	

	int skip=0;

	glDisable(GL_LIGHTING);
	glPointSize(2.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < keys.size(); i++){
		glColor3f((sin(vals[i].x  *PI / 180) + 1) / 2, (sin(vals[i].y *PI / 180) + 1) / 2, (sin(vals[i].z *PI / 180) + 1) / 2);
		
		glVertex3f(keys[i].x, keys[i].y, keys[i].z);

	}
	glEnd();
	glEnable(GL_LIGHTING);

	

}

/*! \brief Asigna las matriz de perspectiva o ortogonal dependiendo de las opciones seleccionadas por el usuario.
*/
void GLWidget::setMatrices(){

	if (showPerspective){
		//glFrustum(0.0f, 1, 0.0f,1, 1.0f, 1024.0f);	//Also works but it would need some tweaking and also probably commenting the GlulookAt
		gluPerspective(60.0, (float)width() / height(), 1.0, -1024.0); // Set perspective
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//Now we position the camera in the middle of the screen (width/2 and heidth /2 ) and looking at the negative z axis
		gluLookAt(mesh->width / 2, mesh->height / 2, mesh->frames,
			mesh->width / 2, mesh->height / 2, -mesh->frames,
			0.0, 1.0, 0.0);
	}
	else{
	
		float aspectRatio = (GLfloat)width() / (GLfloat)height();
		if (width() <= height())
			glOrtho(0.0f, mesh->width, 0.0f - (mesh->height* (aspectRatio - 1)), mesh->height / aspectRatio, -1024.0f, 1024.0f);
		if (width() > height())
			glOrtho(0.0f - (mesh->width* (aspectRatio - 1)), mesh->width*aspectRatio, 0.0f, mesh->height, -1024.0f, 1024.0f);
		else
			glOrtho(0.0f, mesh->width, 0.0f, mesh->height, -1024.0f, 1024.0f);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();


	}
}


/*! \brief Recoge los eventos del raton
*
*	Cada vez que se mueve el raton, se resta la posicion actual de la anterior. Esa diferencia nos indica lo que se ha movido en cada direccion.
*	En el modo normal mover el raton gira la figura pero se puede modificar ese comportamiento usando SHIFT o CTRL a la vez que se mueve el raton.
*/
void GLWidget::mouseMoveEvent(QMouseEvent * event){
	int xpos=0, ypos=0;
	
	xpos = event->x();
	ypos = event->y();
	int incX=1, incY=1;
	
	//si estamos en el modo de mover la figura, se actualizan los las posicones de la figura
	if (movingFigure){
		xMove += (xpos - mouseXPosEntered);
		yMove += (ypos - mouseYPosEntered);
		mouseXPosEntered = xpos;
		mouseYPosEntered = ypos;
		update();
		return;
	}

	//si estamoe en modo de escalado se actualzia el zMove, es decir lo cerca o lo lejos que esta la figura
	if (scalingFigure){
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
	

	mouseXPosEntered = xpos;
	mouseYPosEntered = ypos;
	update();
}


/*! \brief Recoge la posicion en la que se ha hecho click con el raton
*/
void GLWidget::mousePressEvent(QMouseEvent *event){
	mouseXPosEntered = event->x();
	mouseYPosEntered = event->y();
}


/*! \brief REcoe la tecla que se esta pulsando en el momento. Si mantenemos presionado Ctrl=escalado o Shift=movimiento
*/
void GLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
	std::cout << "Pressed key: " << keyEvent->key() << std::endl;
	//keyEvent->ignore();
	if (keyEvent->key() == SHIFT)
		movingFigure = true;
	if (keyEvent->key() == CTRL)
		scalingFigure = true;
		
}

/*! \brief Cuando dejamos de presionar la tecla se vuele al modo por defecto, es decir cuando se arrastra el raton, la figura gira
*/
void GLWidget::keyReleaseEvent(QKeyEvent *keyEvent)
{
	std::cout << "Released key: " << keyEvent->key() << std::endl;
	if (keyEvent->key() == SHIFT)
		movingFigure = false;
	if (keyEvent->key() == CTRL)
		scalingFigure = false;
}

/*! \brief Cuando una figura se ha acabado de leer de dicom o de obj, se avisa para poner la posicion por defecto
*/
void  GLWidget::dataFinishedReadingSlot(){
	std::cout << "received the signal that all the data was read so we set the rotation to xrot-90" << std::endl;
	xRot = -90;
}


/*! \brief Indica que los algoritmos de generacion de mallado han terminado su trabajo asi que podemos representarlo por pantalla
*/
void GLWidget::generatingFinishedSlot(){
	//std::cout << "generating finished slot in the gl window" << std::endl;
	generatingMesh = false;
}

/*! \brief Indica que Los algoritmos de generacion de mallado han empezado a trabajar asi que el visor en 3D deja de actualizarse hasta que el mallado se acaba.
*/
void GLWidget::generatingStartedSlot(){
	generatingMesh = true;
}



/*! \brief Se ha cambiado la opacidad de la figura principal
*/
void GLWidget::opacityChangedSlot(int value){
	opacity = float(value / 100.0);
}





/*! \brief Slot: Cuando se pulsa el boton en la interfaz, se avisa a este slot para representar los triangulso del mallao
*/
void GLWidget::showWireframeSlot(bool value){
	showWireframe = value;
	this->update();
}

/*! \brief Slot mostrar perspectiva o ortografia
*/
void GLWidget::showPerspectiveSlot(bool value){
	showPerspective = value;
	this->update();
}

/*! \brief Slot mostrar mallado
*/
void GLWidget::showMeshSlot(bool value){
	showMesh = value;
	this->update();
}

/*! \brief Slot mostrar cubos octree
*/
void GLWidget::showCubesSlot(bool value){
	showCubes = value;
	this->update();
}

/*! \brief Slot mostrar gradientes
*/
void GLWidget::showGradientSlot(bool value){
	showGradients = value;
	this->update();
}

/*! \brief DEja la figura en el punto inicial y mirando de cara hacia la cámara
*/
void GLWidget::resetFigureSlot(){
	xRot = -90;
	yRot = 0;
	xMove = 0;
	yMove = 0;
	zMove = 0;
	this->update();
}