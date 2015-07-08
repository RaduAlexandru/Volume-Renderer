#include "Mesh.h"
#include <stdlib.h>
#include <iostream>
#include <QTextStream>
#include <QFile>


Mesh::Mesh()
{
	width = 0;
	height = 0;
	frames = 0;
}


Mesh::~Mesh()
{
}

Mesh::Mesh(QObject *_parent)
{
	width = 0;
	height = 0;
	frames = 0;
}

void Mesh::writeToOBJ(QString filename){

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