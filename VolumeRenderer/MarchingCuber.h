#pragma once

#include <vector>
#include <list>
#include <glm/glm.hpp>
#include "OctreeCube.h"
#include "PixelData.h"
//#include <boost/variant.hpp>
//#include <boost/any.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <map>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <boost/unordered_map.hpp>
#include <QtWidgets/QApplication>
#include <QObject>
#include "Mesh.h"


class MarchingCuber : public QObject
{
	Q_OBJECT
public:
	MarchingCuber();
	MarchingCuber(QObject *_parent);
	MarchingCuber(PixelData* pixelData, Mesh*, int isoLevel ,  int cellSizeX, int cellSizeY, int cellSizeZ, int interpolateDepth );
	~MarchingCuber();

	typedef struct {
		glm::vec3 position[8];	//!< Posicion de los 8 vertices de una celda
		double val[8];		//!< Valor que tienen los 8 vertices de una celda
	} CELL;

	void run();
	int polygonise(CELL &);
	inline void interpolate(int, glm::vec3 point1, glm::vec3 point2, float, float, glm::vec3&, int depth = 0);


	

	std::vector<int>  edgeTable;
	std::vector< std::vector<int> > triTable;


	PixelData* pixelData;	//!< El volumen de pixeles que se va a polgonizar

	Mesh* mesh;		//!< MEsh donde se van a escribir el mallado creado

	int isoLevel; //!< Representa el valor umbral con el que se segmenta el volumen de datos 
	int frames;
	int pixelDataHeight;
	int pixelDataWidth;
	int cellSizeX;
	int cellSizeY;
	int cellSizeZ;
	int interpolateDepth;//!< El numero de veces que va a iterar el bucle de interpolacion. Cuando mayor, mas se acercarán lso vertices a la superficie

signals:
	void finishedMeshSignal();
	void progressValueChangedSignal(int);//!< Señal que se envia periodicamente a una barra de progreso en la interfaz
	void progressTextSignal(QString); // !< Señal que se envia con un texto mostrando en que fase se encuentra el algoritmo


};

