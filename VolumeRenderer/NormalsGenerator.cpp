#include "NormalsGenerator.h"
#include <boost/math/special_functions/round.hpp>
#include <stdlib.h>
#include <iostream>
#define PI 3.14159265


NormalsGenerator::NormalsGenerator()
{
}


NormalsGenerator::~NormalsGenerator()
{
}

void NormalsGenerator::normalsPerTriangle(PixelData* pixelData, Mesh* mesh){

	mesh->normals.clear();

	if (mesh->verts.empty()){
		emit finishedNormalsSignal();
		return;
	}

	glm::vec3 u, v, n;


	for (int i = 0; i < mesh->verts.size(); i = i + 3){

		if (i % (mesh->verts.size() / 10) == 0) //Update 10 times in total
			emit progressValueChangedSignal(i * 100 / mesh->verts.size());
		//emit progressValueChangedSignal(i * 100 / model->verts.size());
		u.x = mesh->verts[i + 1].x - mesh->verts[i].x;
		u.y = mesh->verts[i + 1].y - mesh->verts[i].y;
		u.z = mesh->verts[i + 1].z - mesh->verts[i].z;

		v.x = mesh->verts[i + 2].x - mesh->verts[i].x;
		v.y = mesh->verts[i + 2].y - mesh->verts[i].y;
		v.z = mesh->verts[i + 2].z - mesh->verts[i].z;

		n.x = u.y*v.z - u.z*v.y;
		n.y = u.z*v.x - u.x*v.z;
		n.z = u.x*v.y - u.y*v.x;

		//we now normalize it
		double length = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);

		n.x = n.x / length;
		n.y = n.y / length;
		n.z = n.z / length;

		//WE push three time once. Because we have 3 normal per triangle, one for each vetice
		mesh->normals.push_back(n);
		mesh->normals.push_back(n);
		mesh->normals.push_back(n);
	}
	std::cout << "finished normals, emiting signal" << std::endl;
	emit finishedNormalsSignal();

}



void NormalsGenerator::normalsPerVertex(PixelData* pixelData, Mesh* mesh){


	mesh->normals.clear();

	if (mesh->verts.empty()){
		emit finishedNormalsSignal();
		return;
	}

	glm::vec3 u, v, n;

	n.x = 0.0;
	n.y = 0.0;
	n.z = 0.0;
	//I will now try to do the same normal but with the sobel operator///
	///////////////////////////////////////////////////////////////////

	double dx = 0.0, dy = 0.0, dz = 0.0;
	int j=0, y=0, k=0;
	double length=0.0;

	int steps = mesh->verts.size() / 10 - 1;

	for (int i = 0; i < mesh->verts.size(); i = i + 1){


		//normals.push_back(n);
		//continue;

		if (i % (mesh->verts.size() / 10) == 0) //Update 10 times in total
			emit progressValueChangedSignal(i * 100 / mesh->verts.size());
		/*j = boost::math::iround(verts[i].x);
		y = boost::math::iround(verts[i].y);
		k = boost::math::iround(verts[i].z);*/

		j = round(mesh->verts[i].x);
		y = round(mesh->verts[i].y);
		k = round(mesh->verts[i].z);

		/*if (y>pixelData->height-200-50){
			n.x = 0.0;
			n.y = 0.0;
			n.z = 0.0;
			normals.push_back(n);
			continue;
		}*/

		//getSmoothPixelValue
		/*
		dx = -1 * (pixelData->getPixelValue(j - 1, y + 1, k - 1)) + 1 * (pixelData->getPixelValue(j + 1, y + 1, k - 1)) -
			2 * (pixelData->getPixelValue(j - 1, y, k - 1)) + 2 * (pixelData->getPixelValue(j + 1, y, k - 1)) -
			1 * (pixelData->getPixelValue(j - 1, y - 1, k - 1)) + 1 * (pixelData->getPixelValue(j + 1, y - 1, k - 1)) -

			2 * (pixelData->getPixelValue(j - 1, y + 1, k)) + 2 * (pixelData->getPixelValue(j + 1, y + 1, k)) -
			4 * (pixelData->getPixelValue(j - 1, y, k)) + 2 * (pixelData->getPixelValue(j + 1, y, k)) -
			2 * (pixelData->getPixelValue(j - 1, y - 1, k)) + 2 * (pixelData->getPixelValue(j + 1, y - 1, k)) -

			1 * (pixelData->getPixelValue(j - 1, y + 1, k + 1)) + 1 * (pixelData->getPixelValue(j + 1, y + 1, k + 1)) -
			2 * (pixelData->getPixelValue(j - 1, y, k + 1)) + 2 * (pixelData->getPixelValue(j + 1, y, k + 1)) -
			1 * (pixelData->getPixelValue(j - 1, y - 1, k + 1)) + 1 * (pixelData->getPixelValue(j + 1, y - 1, k + 1));

		dy = 1 * (pixelData->getPixelValue(j - 1, y + 1, k - 1)) + 2 * (pixelData->getPixelValue(j, y + 1, k - 1)) + 1 * (pixelData->getPixelValue(j + 1, y + 1, k - 1)) -
			1 * (pixelData->getPixelValue(j - 1, y - 1, k - 1)) - 2 * (pixelData->getPixelValue(j, y - 1, k - 1)) - 1 * (pixelData->getPixelValue(j + 1, y - 1, k - 1)) +

			2 * (pixelData->getPixelValue(j - 1, y + 1, k)) + 4 * (pixelData->getPixelValue(j, y + 1, k)) + 2 * (pixelData->getPixelValue(j + 1, y + 1, k)) -
			2 * (pixelData->getPixelValue(j - 1, y - 1, k)) - 4 * (pixelData->getPixelValue(j, y - 1, k)) - 2 * (pixelData->getPixelValue(j + 1, y - 1, k)) +

			1 * (pixelData->getPixelValue(j - 1, y + 1, k + 1)) + 2 * (pixelData->getPixelValue(j, y + 1, k + 1)) + 1 * (pixelData->getPixelValue(j + 1, y + 1, k + 1)) -
			1 * (pixelData->getPixelValue(j - 1, y - 1, k + 1)) - 2 * (pixelData->getPixelValue(j, y - 1, k + 1)) - 1 * (pixelData->getPixelValue(j + 1, y - 1, k + 1));

		dz = -1 * (pixelData->getPixelValue(j - 1, y + 1, k - 1)) - 2 * (pixelData->getPixelValue(j, y + 1, k - 1)) - 1 * (pixelData->getPixelValue(j + 1, y + 1, k - 1)) -
			2 * (pixelData->getPixelValue(j - 1, y, k - 1)) - 4 * (pixelData->getPixelValue(j, y, k - 1)) - 2 * (pixelData->getPixelValue(j + 1, y, k - 1)) -
			1 * (pixelData->getPixelValue(j - 1, y - 1, k - 1)) - 2 * (pixelData->getPixelValue(j, y - 1, k - 1)) - 1 * (pixelData->getPixelValue(j + 1, y - 1, k - 1)) +

			1 * (pixelData->getPixelValue(j - 1, y + 1, k + 1)) + 2 * (pixelData->getPixelValue(j, y + 1, k + 1)) + 1 * (pixelData->getPixelValue(j + 1, y + 1, k + 1)) -
			2 * (pixelData->getPixelValue(j - 1, y, k + 1)) + 4 * (pixelData->getPixelValue(j, y, k + 1)) + 2 * (pixelData->getPixelValue(j + 1, y, k + 1)) -
			1 * (pixelData->getPixelValue(j - 1, y - 1, k + 1)) + 2 * (pixelData->getPixelValue(j, y - 1, k + 1)) + 1 * (pixelData->getPixelValue(j + 1, y - 1, k + 1));*/

		///Smooth one

		/*dx = -1 * (model->getSmoothPixelValue(j - 1, y + 1, k - 1)) + 1 * (model->getSmoothPixelValue(j + 1, y + 1, k - 1)) -
		2 * (model->getSmoothPixelValue(j - 1, y, k - 1)) + 2 * (model->getSmoothPixelValue(j + 1, y, k - 1)) -
		1 * (model->getSmoothPixelValue(j - 1, y - 1, k - 1)) + 1 * (model->getSmoothPixelValue(j + 1, y - 1, k - 1)) -

		2 * (model->getSmoothPixelValue(j - 1, y + 1, k)) + 2 * (model->getSmoothPixelValue(j + 1, y + 1, k)) -
		4 * (model->getSmoothPixelValue(j - 1, y, k)) + 2 * (model->getSmoothPixelValue(j + 1, y, k)) -
		2 * (model->getSmoothPixelValue(j - 1, y - 1, k)) + 2 * (model->getSmoothPixelValue(j + 1, y - 1, k)) -

		1 * (model->getSmoothPixelValue(j - 1, y + 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, y + 1, k + 1)) -
		2 * (model->getSmoothPixelValue(j - 1, y, k + 1)) + 2 * (model->getSmoothPixelValue(j + 1, y, k + 1)) -
		1 * (model->getSmoothPixelValue(j - 1, y - 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, y - 1, k + 1));

		dy = 1 * (model->getSmoothPixelValue(j - 1, y + 1, k - 1)) + 2 * (model->getSmoothPixelValue(j, y + 1, k - 1)) + 1 * (model->getSmoothPixelValue(j + 1, y + 1, k - 1)) -
		1 * (model->getSmoothPixelValue(j - 1, y - 1, k - 1)) - 2 * (model->getSmoothPixelValue(j, y - 1, k - 1)) - 1 * (model->getSmoothPixelValue(j + 1, y - 1, k - 1)) +

		2 * (model->getSmoothPixelValue(j - 1, y + 1, k)) + 4 * (model->getSmoothPixelValue(j, y + 1, k)) + 2 * (model->getSmoothPixelValue(j + 1, y + 1, k)) -
		2 * (model->getSmoothPixelValue(j - 1, y - 1, k)) - 4 * (model->getSmoothPixelValue(j, y - 1, k)) - 2 * (model->getSmoothPixelValue(j + 1, y - 1, k)) +

		1 * (model->getSmoothPixelValue(j - 1, y + 1, k + 1)) + 2 * (model->getSmoothPixelValue(j, y + 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, y + 1, k + 1)) -
		1 * (model->getSmoothPixelValue(j - 1, y - 1, k + 1)) - 2 * (model->getSmoothPixelValue(j, y - 1, k + 1)) - 1 * (model->getSmoothPixelValue(j + 1, y - 1, k + 1));

		dz = -1 * (model->getSmoothPixelValue(j - 1, y + 1, k - 1)) - 2 * (model->getSmoothPixelValue(j, y + 1, k - 1)) - 1 * (model->getSmoothPixelValue(j + 1, y + 1, k - 1)) -
		2 * (model->getSmoothPixelValue(j - 1, y, k - 1)) - 4 * (model->getSmoothPixelValue(j, y, k - 1)) - 2 * (model->getSmoothPixelValue(j + 1, y, k - 1)) -
		1 * (model->getSmoothPixelValue(j - 1, y - 1, k - 1)) - 2 * (model->getSmoothPixelValue(j, y - 1, k - 1)) - 1 * (model->getSmoothPixelValue(j + 1, y - 1, k - 1)) +

		1 * (model->getSmoothPixelValue(j - 1, y + 1, k + 1)) + 2 * (model->getSmoothPixelValue(j, y + 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, y + 1, k + 1)) -
		2 * (model->getSmoothPixelValue(j - 1, y, k + 1)) + 4 * (model->getSmoothPixelValue(j, y, k + 1)) + 2 * (model->getSmoothPixelValue(j + 1, y, k + 1)) -
		1 * (model->getSmoothPixelValue(j - 1, y - 1, k + 1)) + 2 * (model->getSmoothPixelValue(j, y - 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, y - 1, k + 1));*/


		dx = pixelData->convolveX( j, y, k);
		dy = pixelData->convolveY( j, y, k);
		dz = pixelData->convolveZ( j, y, k);





		if (dx == 0)
			dx = 1;
		if (dy == 0)
			dy = 1;
		if (dz == 0)
			dz = 1;

		/*THe 3 possible axis
		dx-dy
		dx-dz
		dy-dz
		*/


		//we now normalize it


		length = sqrt(dx*dx + dy*dy + dz*dz);

			dx = dx / length;
			dy = dy / length;
			dz = dz / length;
		

		n.x = dx;
		n.y = dy;
		n.z = dz;

		mesh->normals.push_back(n);
		//model->normals.push_back(dy);
		//model->normals.push_back(dz);
		
	}
	emit finishedNormalsSignal();

}


