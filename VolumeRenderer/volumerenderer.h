#ifndef VOLUMERENDERER_H
#define VOLUMERENDERER_H

#include <QtWidgets/QMainWindow>
#include "ui_volumerenderer.h"
#include <stdlib.h>

#include "glwidget.h"
#include "Model.h"
#include <glm/glm.hpp>


class VolumeRenderer : public QMainWindow
{
	Q_OBJECT

public:
	VolumeRenderer(QWidget *parent = 0);
	~VolumeRenderer();

	typedef struct {
		glm::vec3 position;
		glm::vec3 normal;
		double value;
	} Vertex;

	typedef struct {
		glm::vec3 position[8];	//Position of the 8 vertices
		glm::vec3 normal[8];	//normals of those vertices
		double val[8];		//value at those vertices

		Vertex vertices[8];

	} CELL;

	

	
	

private:
	Ui::VolumeRendererClass ui;

	Model* model;

	int readPixelData(QString, void*&, int, int);
	int getRepresentation(QString , int& , int& , int& , int& ,int&, int&, int&);
	//void showPixels(void* pixelData, std::vector<Model::POINTF>);
	void load_image_data(const char*);
	void polygonise(CELL, int, std::vector<float>&);
	inline void interpolate(int, glm::vec3 point1, glm::vec3 point2, float, float, glm::vec3&);
	int loadDICOMPixelData(const char*);
	int loadDICOMPixelData(QStringList);

	int adaptiveMarchingCubes();

	
	void marchingSquares();
	void wipeBitmap();
	void wipePoints();

private slots:
	void on_test_clicked();
	void on_addDICOMFiles_clicked();
	void on_isoLevelSlider_valueChanged();
	void on_loadTestImageButton_clicked();
	void on_loadTestImage2Button_clicked();
	void on_loadDICOMButton_clicked();
	void on_loadMFDICOMButton_clicked();
	void on_loadDICOMFromFile_clicked();
	void on_resolutionSlider_valueChanged();
	void on_linearInterpolationButton_clicked();
	void on_pointFlagButton_clicked();
	void on_borderFlagButton_clicked();
	void on_frameSlider_valueChanged();
	
};

#endif // VOLUMERENDERER_H
