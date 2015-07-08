#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "PixelData.h"
#include <QtWidgets/QApplication>
#include <QObject>
#include "Mesh.h"


class NormalsGenerator : public  QObject
{
	Q_OBJECT
public:
	NormalsGenerator();
	~NormalsGenerator();

	void normalsPerTriangle(PixelData* pixeldata, Mesh* mesh);
	void normalsPerVertex(PixelData* pixeldata, Mesh* mesh);
private:
	

signals:
	void finishedNormalsSignal();
	void progressValueChangedSignal(int);
};

