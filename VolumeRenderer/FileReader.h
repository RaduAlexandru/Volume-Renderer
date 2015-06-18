#pragma once
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QFileDialog>
#include "PixelData.h"

class FileReader
{
public:
	FileReader();
	~FileReader();



	int FileReader::loadDICOMPixelData(QStringList fileNames, PixelData*pixelData);
	int FileReader::getRepresentation(QString fileName, int& bitsAllocated, int& bitsStored, int& hightBit, int& pixelRepresentation, int& frames, int& height, int& width);
};

