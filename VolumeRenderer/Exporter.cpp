#include "Exporter.h"
#include <stdlib.h>
#include <iostream>
#include <QTextStream>

Exporter::Exporter()
{
}


Exporter::~Exporter()
{
}


Exporter::Exporter(QObject *_parent)
{

}


void Exporter::writeToOBJ(QString filename, std::vector<glm::vec3> verts, std::vector<glm::vec3> normals, int width, int height, int frames){


	QFile f(filename);

	if (!f.open(QIODevice::WriteOnly | QIODevice::Text)){
		std::cout << "Error opening the file for writing" << std::endl;
		return;
	}

	QTextStream out(&f);

	out << "representation " << width << " " << height << " " << frames << "\n";


	for (int i = 0; i < verts.size(); i = i + 1){
		if (i % (verts.size() / 10) == 0)
			emit progressValueChangedSignal(i * 30 / verts.size());
		out << "v " << verts[i].x << " " << verts[i].y << " " << verts[i].z << "\n";
	}
	for (int i = 0; i < normals.size(); i = i + 1){
		if (i % (normals.size() / 10) == 0)
			emit progressValueChangedSignal(i * 30 / verts.size() + 30);
		out << "vn " << normals[i].x << " " << normals[i].y << " " << normals[i].z << "\n";
	}
	for (int i = 0; i < verts.size() - 3; i = i + 3){
		if (i % (verts.size() / 10) == 0)
			emit progressValueChangedSignal(i * 40 / verts.size() + 60);
		out << "f " << i + 1 << "//" << i + 1 << " " << i + 2 << "//" << i + 2 << " " << i + 3 << "//" << i + 3 << "\n";
	}

	f.close();
	emit finishedWritingToFileSignal();
}