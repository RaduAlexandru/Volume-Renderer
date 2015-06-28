#pragma once
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QFileDialog>
#include "PixelData.h"
#include <QObject>
#include <glm/vec3.hpp>

class FileReader : public QObject
{
	Q_OBJECT
public:
	FileReader();
	~FileReader();
	FileReader(QObject *_parent);



	int FileReader::loadDICOMPixelData(QStringList fileNames, PixelData*pixelData);
	int FileReader::getRepresentation(QString fileName, int& bitsAllocated, int& bitsStored, int& hightBit, int& pixelRepresentation, int& frames, int& height, int& width);
	int FileReader::loadOBJFile(QString fileName, PixelData*pixelData,std::vector<glm::vec3>& verts,std::vector<glm::vec3>& normals);

signals:
	void dataFinishedReadingSignal();
	void objFinishedReadingSignal();	//When you finish reading the obj it is the same as when you finish the normals because you completely change the verts and the normals of the mesh
	void progressValueChangedSignal(int);
	void progressTextSignal(QString);
};

