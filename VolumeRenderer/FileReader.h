#pragma once
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QFileDialog>
#include "PixelData.h"
#include <QObject>
#include <glm/vec3.hpp>
#include "Mesh.h"

class FileReader : public QObject
{
	Q_OBJECT
public:
	FileReader();
	~FileReader();
	FileReader(QObject *_parent);



	int FileReader::loadDICOMPixelData(QStringList fileNames, PixelData*pixelData);
	int FileReader::getRepresentation(QString fileName, int& bitsAllocated, int& bitsStored, int& hightBit, int& pixelRepresentation, int& frames, int& height, int& width);
	int FileReader::loadOBJFile(QString fileName, PixelData*pixelData,Mesh* mesh);

signals:
	void dataFinishedReadingSignal(); //!< Señal que se envia cuando se han acabado de leer los archivos DICOM. El controlador tiene un slot asociado y va empezar el MarchingCuber
	void objFinishedReadingSignal();	//!< Señal que se envia cuando se ha acabado de leer un obj. Hay un slot asociado en la vista 3D que va a empezar a representarlo
	void progressValueChangedSignal(int);	//!< Señal que se envia periodicamente a una barra de progreso en la interfaz
	void progressTextSignal(QString);	//!< Señal que se envia con un texto mostrando en que fase se encuentra el reader
};

