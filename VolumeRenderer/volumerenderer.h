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
#include <glm/glm.hpp>
#include <boost/date_time.hpp>



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

	void generateMesh(int force=0);
	void generateNormals();
	void ballPivot();

	void wipePoints();
	void wipePixelData();

	bool interactive;	
	bool generatingMesh; //We need this to know if there the mesh is being generated, that means that there is already a thread working, in which case the generateMesh function doesnt do anything

	boost::posix_time::ptime timeStart;
	boost::posix_time::ptime timeFinish;


signals:
	void progressValueChangedSignal(int newValue);
	void generatingFinishedSignal();
	void generatingStartedSignal();
	void dataFinishedReading();

private slots:
	void on_isoLevelSlider_valueChanged();
	void on_loadDICOMFromFile_clicked();
	void on_resolutionSlider_valueChanged();
	void on_linearInterpolationSlider_valueChanged();
	void on_toleranceSlider_valueChanged();
	void on_marchingCubesButton_clicked();
	void on_adaptiveMarchingCubes2Button_clicked();
	void on_adaptiveMarchingCubes3Button_clicked();
	void on_normalsPerTriangleButton_clicked();
	void on_normalsPerVerticeButton_clicked();
	void on_orientationZButton_clicked();
	void on_orientationXButton_clicked();
	void on_orientationYButton_clicked();
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


	void on_saveToSecondaryButton_clicked();
	void on_clearPinButton_clicked();

	void on_moveButton_clicked();
	void on_scaleButton_clicked();
	
};

#endif // VOLUMERENDERER_H
