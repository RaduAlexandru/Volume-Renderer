#ifndef VOLUMERENDERER_H
#define VOLUMERENDERER_H

#include <QtWidgets/QMainWindow>
#include "ui_volumerenderer.h"
#include <stdlib.h>

#include "glwidget.h"
#include "Model.h"
#include "MarchingCuber.h"
#include "AdaptiveCuber.h"
#include "FileReader.h"
#include "NormalsGenerator.h"
#include "Exporter.h"
#include <glm/glm.hpp>



class VolumeRenderer : public QMainWindow
{
	Q_OBJECT

public:
	VolumeRenderer(QWidget *parent = 0);
	~VolumeRenderer();

	

	
	

	
	

public:
	Ui::VolumeRendererClass ui;

	Model* model;
	MarchingCuber* mc;
	AdaptiveCuber* amc;
	FileReader* reader;
	NormalsGenerator* ng;



	void establishConnectionsMC();
	void establishConnectionsAMC();
	void establishConnectionsNG();
	void establishConnectionsREADER();

	int readPixelData(QString, void*&, int, int);
	int getRepresentation(QString , int& , int& , int& , int& ,int&, int&, int&);
	//void showPixels(void* pixelData, std::vector<Model::POINTF>);
	void load_image_data(const char*);
	inline void interpolate(int, glm::vec3 point1, glm::vec3 point2, float, float, glm::vec3&, int depth=0);
	int loadDICOMPixelData(const char*);
	int loadDICOMPixelData(QStringList);

	int adaptiveMarchingCubes();
	void calculateGradient();
	inline bool cubeNeedsSubdivision(OctreeCube&);
	int generateOctree(OctreeCube,int=0);	//The int=0 is the default value of the depth. We obviously start at 0
	int generateOctree_tree_version(OctreeCube&, int = 0);
	int adaptiveMarchingCubes2();
	int adaptiveMarchingCubes3();
	int polygoniseOctree(OctreeCube*,int=0);
	int polygoniseOctree2(OctreeCube* );
	int polygoniseAssignToCube(OctreeCube* );
	int readPointsFromOctree(OctreeCube*);
	//glm::vec3 pointOnLine(glm::vec3 , glm::vec3 , glm::vec3& );

	void octree2CellPolygonise(OctreeCube&);
	void generateHypercube();
	void correctlyAssignLeafs(OctreeCube*);

	void generateMesh(int force=0);
	void generateNormals();
	OctreeCube createInitialCube();
	void ballPivot();
	//void crackPatch(OctreeCube*);
	//void crackPatch2(OctreeCube*);
	
	void marchingSquares();
	void wipePoints();
	void wipePixelData();

	bool interactive;
	bool generatingMesh; //We need this to know if there the mesh is being generated, that means that there is already a thread working, in which case the generateMesh function doesnt do anything


signals:
	void progressValueChangedSignal(int newValue);
	void generatingFinishedSignal();
	void generatingStartedSignal();
	void dataFinishedReading();

private slots:
	//void on_addDICOMFiles_clicked();
	void on_isoLevelSlider_valueChanged();
	//void on_loadTestImageButton_clicked();
	//void on_loadTestImage2Button_clicked();
	//void on_loadDICOMButton_clicked();
	//void on_loadMFDICOMButton_clicked();
	void on_loadDICOMFromFile_clicked();
	void on_resolutionSlider_valueChanged();
	void on_linearInterpolationSlider_valueChanged();
	void on_frameSlider_valueChanged();
	void on_showWireframeButton_clicked();
	void on_toleranceSlider_valueChanged();
	void on_marchingCubesButton_clicked();
	void on_adaptiveMarchingCubesButton_clicked();
	void on_adaptiveMarchingCubes2Button_clicked();
	void on_adaptiveMarchingCubes3Button_clicked();
	void on_ballPivotButton_clicked();
	void on_perspectiveButton_clicked();
	void on_normalsPerTriangleButton_clicked();
	void on_normalsPerVerticeButton_clicked();
	void on_showMeshButton_clicked();
	void on_showGradientButton_clicked();
	void on_orientationZButton_clicked();
	void on_orientationXButton_clicked();
	void on_orientationYButton_clicked();
	void on_showCubesButton_clicked();
	void on_xPosText_editingFinished();
	void on_yPosText_editingFinished();
	void on_zPosText_editingFinished();
	void on_generateMeshButton_clicked();
	void on_interactiveButton_clicked();
	void on_octreeDepthSlider_valueChanged();
	void on_borderYBottomSlider_valueChanged();
	void on_borderYTopSlider_valueChanged();
	void on_borderXLeftSlider_valueChanged();
	void on_borderXRightSlider_valueChanged();
	void on_loadObjButton_clicked();
	void on_writeObjButton_clicked();

	void progressValueChangedSlot(int newValue);
	void progressTextSlot(QString);
	void finishedMeshSlot();
	void finishedNormalsSlot();
	void dataFinishedReadingSlot();

	
};

#endif // VOLUMERENDERER_H
