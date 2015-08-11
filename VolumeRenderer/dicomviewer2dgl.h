#ifndef DICOMVIEWER2DGL_H
#define DICOMVIEWER2DGL_H

#include <QOpenGLWidget>
#include "PixelData.h"
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
	PixelData* pixelData;	//!< Referencia al volumen de datos de pixeles
	int orientation;	//!< Orientacion (x, y o z) en la que se va a recorrer ese volumen
	void displayFrame();
	void displayBorderLines();
	std::vector<unsigned int> localFrame; //!< Almacena los pixels que se van a representar en la vista 2d. Va a cambiar segun la orientacion elegida
	int frame_to_display;	//!< Número de la capa que se está mostrando

	
public slots:
	void setFrame(int frame);
};

#endif // DICOMVIEWER2DGL_H
