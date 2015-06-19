#pragma once
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QFileDialog>
#include "PixelData.h"
#include <QObject>

class FileReader : public QObject
{
	Q_OBJECT
public:
	FileReader();
	~FileReader();
	FileReader(QObject *_parent);



	int FileReader::loadDICOMPixelData(QStringList fileNames, PixelData*pixelData);
	int FileReader::getRepresentation(QString fileName, int& bitsAllocated, int& bitsStored, int& hightBit, int& pixelRepresentation, int& frames, int& height, int& width);

signals:
	void dataFinishedReadingSignal();
};

