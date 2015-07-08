#pragma once
#include <glm/vec3.hpp>
#include <QString>
#include <vector>
#include <QFile>
#include <QObject>
#include <QtWidgets/QApplication>
class Mesh : public QObject
{
	Q_OBJECT
public:
	Mesh();
	~Mesh();
	Mesh(QObject *_parent);

	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> normals;
	int height;
	int width;
	int frames;

	void writeToOBJ(QString filename);

signals:
	void progressValueChangedSignal(int);
	void finishedWritingToFileSignal();
};

