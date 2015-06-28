#pragma once
#include <glm/vec3.hpp>
#include <QString>
#include <vector>
#include <QFile>
#include <QObject>
#include <QtWidgets/QApplication>
class Exporter : public QObject
{

	Q_OBJECT
public:
	Exporter();
	~Exporter();
	Exporter(QObject *_parent);

	void writeToOBJ(QString filename, std::vector<glm::vec3> verts, std::vector<glm::vec3> normals, int width, int height, int frames);

signals:
	void progressValueChangedSignal(int);
	void finishedWritingToFileSignal();
};

