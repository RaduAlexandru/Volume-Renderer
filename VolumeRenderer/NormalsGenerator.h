#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include "PixelData.h"
#include <QtWidgets/QApplication>
#include <QObject>


class NormalsGenerator : public  QObject
{
	Q_OBJECT
public:
	NormalsGenerator();
	~NormalsGenerator();

	void normalsPerTriangle(PixelData* pixeldata, std::vector<glm::vec3>&verts, std::vector<glm::vec3>&normals);
	void normalsPerVertex(PixelData* pixeldata, std::vector<glm::vec3>&verts, std::vector<glm::vec3>&normals);

signals:
	void finishedNormalsSignal();
	void progressValueChangedSignal(int);
};

