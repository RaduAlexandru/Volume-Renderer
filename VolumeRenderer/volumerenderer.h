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
	} Vertex;

	typedef struct {
		glm::vec3 position[8];	//Position of the 8 vertices
		glm::vec3 normal[8];	//normals of those vertices I dont know if I'm going to use it
		double val[8];		//value at those vertices
	} CELL;

	

	
	

private:
	Ui::VolumeRendererClass ui;

	Model* model;

	int readPixelData(QString, void*&, int, int);
	int getRepresentation(QString , int& , int& , int& , int& ,int&, int&, int&);
	//void showPixels(void* pixelData, std::vector<Model::POINTF>);
	void load_image_data(const char*);
	void polygonise(CELL&, std::vector<double>&);
	inline void interpolate(int, glm::vec3 point1, glm::vec3 point2, float, float, glm::vec3&, int depth=0);
	int loadDICOMPixelData(const char*);
	int loadDICOMPixelData(QStringList);

	int adaptiveMarchingCubes();
	void calculateGradient();
	inline bool cubeNeedsSubdivision(OctreeCube&);
	int generateOctree(OctreeCube,int=0);	//The int=0 is the default value of the depth. We obviously start at 0
	int adaptiveMarchingCubes2();
	void generateMesh();
	void generateNormals();
	void createInitialCube();
	void ballPivot();
	
	void marchingSquares();
	void wipeBitmap();
	void wipePoints();
	void wipePixelData();

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
	void on_showWireframeButton_clicked();
	void on_toleranceSlider_valueChanged();
	void on_marchingCubesButton_clicked();
	void on_adaptiveMarchingCubesButton_clicked();
	void on_adaptiveMarchingCubes2Button_clicked();
	void on_ballPivotButton_clicked();
	void on_perspectiveButton_clicked();
	void on_normalsPerTriangleButton_clicked();
	void on_normalsPerVerticeButton_clicked();
	void on_interpolateDepthText_editingFinished();
	void on_showMeshButton_clicked();
	void on_showGradientButton_clicked();
	void on_orientationZButton_clicked();
	void on_orientationXButton_clicked();
	void on_orientationYButton_clicked();
	void on_showCubesButton_clicked();

	
};

#endif // VOLUMERENDERER_H
