#ifndef DICOMVIEWER2DGL_H
#define DICOMVIEWER2DGL_H

#include <QOpenGLWidget>
#include "Model.h"
#include <QOpenGLFunctions_4_3_Core>
#include <vector>
#include "OctreeCube.h"

class DicomViewer2DGL : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
	Q_OBJECT

public:
	DicomViewer2DGL(QWidget *parent);
	~DicomViewer2DGL();
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	Model* model;
	std::vector<unsigned int>  gradient;
	std::vector<OctreeCube> cubes;
	OctreeCube selectedCube;
	int tolerance;
	//unsigned char * gradient;
	void displayFrame();
	void displayGradient();
	void displayBorderLines();
	std::vector<unsigned int> localFrame; //!< Almacena los pixels data que se van a representar en la vista 2d. Va a cambiar segun la orientacion elegida

private:
	int frame_to_display;
public slots:
	void setFrame(int frame);
	
};

#endif // DICOMVIEWER2DGL_H
