
//QT CODE/////////////


#include "volumerenderer.h"
#include <QtWidgets/QApplication>
#include <iostream>

using namespace::std;

int main(int argc, char *argv[])
{
	std::cout << "..." << std::endl;
	std::cout << "..." << std::endl;
	
	QApplication a(argc, argv);
	std::cout << "..." << std::endl;
	std::cout << "..." << std::endl;
	VolumeRenderer w;
	w.show();
	std::cout << "..." << std::endl;
	return a.exec();
	
}


//DCMTK CODE
/*
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"

#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcfilefo.h" 
#include "dcmtk/dcmdata/dcmetinf.h" 
#include "dcmtk/dcmimgle/dcmimage.h" 

#include "dcmtk/dcmjpeg/djdecode.h"     
#include "dcmtk/dcmjpeg/djencode.h"      

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include "volumerenderer.h"
#include <QtWidgets/QApplication>
#include <QDebug>

using namespace::std;

int main(int argc, char *argv[])
{
	QApplication a (argc, argv);
	string testString = "cosascosascosas";
	QString str = "Hello World\n";
	QTextStream qout(stdout, QIODevice::WriteOnly); //I connect the stout to my qout textstream

	qout << "1. Starting the application\n";
	qout << str;
	std::cout << "2. Some normal iostream output before using qDebug\n";
	qDebug() << "3. Some output with qDebug after the iostream output\n";
	

	VolumeRenderer w;
	w.show();
	std::cout << "hola" << std::endl;
	



	DcmFileFormat fileformat;
	OFCondition status = fileformat.loadFile("E:\\Universidad\\Hecho por mi\\Volume Renderer\\cuda1\\Release\\IM-0001-0001.dcm");
	if (!status.good())
	{
		cerr << "Error: cannot read DICOM file (" << status.text() << ")" << endl;
		return 1;
	}


	OFString patientsName;
	fileformat.getDataset()->findAndGetOFString(DCM_PatientName, patientsName);
	cout << "Patient's Name: " << patientsName << endl;
	qDebug("message %s", patientsName);

	OFString rows;
	fileformat.getDataset()->findAndGetOFString(DCM_Rows, rows);
	cout << rows << endl;

	OFString columns;
	fileformat.getDataset()->findAndGetOFString(DCM_Columns, columns);
	cout << columns << endl;

	OFString Bits_stored;
	fileformat.getDataset()->findAndGetOFString(DCM_BitsStored, Bits_stored);
	cout << Bits_stored << endl;




	//Sleep(100000);
	//system("PAUSE");
	return a.exec();
	return 0;
}

*/

//OPENGL CODE///////////////
/*
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		//printf("%d, %d\n",width,height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
		glBegin(GL_TRIANGLES);
		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-0.6f, -0.4f, 0.f);
		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(0.6f, -0.4f, 0.f);
		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(0.f, 0.6f, 0.f);
		glEnd();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}*/


//CUDA CODE///////////////
/*

#include "cuda_runtime.h";
#include <stdlib.h>;

extern "C" void launch_kernel();

int main(void){
	launch_kernel();
	system("PAUSE");
}
*/