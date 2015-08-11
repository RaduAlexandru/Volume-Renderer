#include "GL/glew.h"
#include "volumerenderer.h"
#include <iostream>


#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"

#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcfilefo.h" 
#include "dcmtk/dcmdata/dcmetinf.h" 
#include "dcmtk/dcmimgle/dcmimage.h" 

#include "dcmtk/dcmjpeg/djdecode.h"     
#include "dcmtk/dcmjpeg/djencode.h"  

#include "dcmtk/dcmdata/dcpxitem.h"


#include <stdlib.h>
#include <cstdlib>
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsDropShadowEffect>


#include <GLFW/glfw3.h>
#include "glwidget.h"
#include "dicomviewer2dgl.h"
#include "Model.h"
#include <math.h> 
#include <tuple>
#include <boost/math/special_functions/round.hpp>
#include <boost/thread.hpp>
#include <queue>



//#include <boost/variant.hpp>
#define PI 3.14159265


extern "C" void launch_kernel(std::vector<int >, std::vector< std::vector<int> >);


using namespace::std;



VolumeRenderer::VolumeRenderer(QWidget *parent)
	: QMainWindow(parent)
{
	mc = NULL;
	amc = NULL;
	ng = NULL;
	reader = NULL;



	ui.setupUi(this);
	model = new Model();
	ui.glwidget->mesh = model->mesh;
	ui.glwidget->mesh2 = model->mesh2;
	ui.glwidget->gradient = &(model->gradient);
	ui.dicomviewer2dgl->pixelData = model->pixelData;
	interactive = true;
	generatingMesh = false;

	//establishConnections();
	connect(this, SIGNAL(generatingStartedSignal()), ui.glwidget, SLOT(generatingStartedSlot()));
	connect(this, SIGNAL(generatingFinishedSignal()), ui.glwidget, SLOT(generatingFinishedSlot()));


	QFile File("stylesheet.qss");
	File.open(QFile::ReadOnly);
	QString StyleSheet = QLatin1String(File.readAll());

	qApp->setStyleSheet(StyleSheet);

	ui.progressBar->setTextVisible(false);
	ui.progressBar->setMaximumHeight(4);
	ui.progressText->setText("<font color='black'></font>");

	ui.progressText->setStyleSheet("QLabel { color: black }");
	
	QPixmap pixmap("./images/move_icon2.png");
	QIcon ButtonIcon(pixmap);
	ui.moveButton->setIcon(ButtonIcon);

	QPixmap pixmap2("./images/scale_icon2.png");
	QIcon ButtonIcon2(pixmap2);
	ui.scaleButton->setIcon(ButtonIcon2);
	this->setCentralWidget(ui.glwidget);
	ui.glwidget->lower();


	QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect(this);
	effect->setBlurRadius(1);
	effect->setColor(QColor("#060409"));
	effect->setOffset(1, 1);

	ui.label->setGraphicsEffect(effect);

	QGraphicsDropShadowEffect* effect2 = new QGraphicsDropShadowEffect(this);
	effect2->setBlurRadius(1);
	effect2->setColor(QColor("#060409"));
	effect2->setOffset(1, 1);
	ui.label_3->setGraphicsEffect(effect2);

	QGraphicsDropShadowEffect* effect3 = new QGraphicsDropShadowEffect(this);
	effect3->setBlurRadius(1);
	effect3->setColor(QColor("#060409"));
	effect3->setOffset(1, 1);
	ui.label_8->setGraphicsEffect(effect3);
	//ui.label_8->setGraphicsEffect(effect);

	QGraphicsDropShadowEffect* effect4 = new QGraphicsDropShadowEffect(this);
	effect4->setBlurRadius(1);
	effect4->setColor(QColor("#060409"));
	effect4->setOffset(1, 1);
	ui.label_9->setGraphicsEffect(effect4);

	QGraphicsDropShadowEffect* effect5 = new QGraphicsDropShadowEffect(this);
	effect5->setBlurRadius(1);
	effect5->setColor(QColor("#060409"));
	effect5->setOffset(1, 1);
	ui.label_7->setGraphicsEffect(effect5);

	QGraphicsDropShadowEffect* effect6 = new QGraphicsDropShadowEffect(this);
	effect6->setBlurRadius(1);
	effect6->setColor(QColor("#060409"));
	effect6->setOffset(1, 1);
	ui.label_14->setGraphicsEffect(effect6);



	ui.moveButton->setToolTip("Left Shift");
	ui.scaleButton->setToolTip("Left Ctrl");

}

VolumeRenderer::~VolumeRenderer()
{
	
}



/*! \brief Establece las conexiones de signals y slots entre la clase MarchingCubes y el controlador
*/
void VolumeRenderer::establishConnectionsMC(){
	connect(mc, SIGNAL(progressValueChangedSignal(int)), this, SLOT(progressValueChangedSlot(int)));
	connect(mc, SIGNAL(finishedMeshSignal()), this, SLOT(finishedMeshSlot()));
}

/*! \brief Establece las conexiones de signals y slots entre la clase AdaptiveMarchingCubes y el controlador
*/
void VolumeRenderer::establishConnectionsAMC(){
	connect(amc, SIGNAL(progressValueChangedSignal(int)), this, SLOT(progressValueChangedSlot(int)));
	connect(amc, SIGNAL(finishedMeshSignal()), this, SLOT(finishedMeshSlot()));
	connect(amc, SIGNAL(progressTextSignal(QString)), this, SLOT(progressTextSlot(QString)));
}

/*! \brief Establece las conexiones de signals y slots entre la clase NormalsGenerator y el controlador
*/
void VolumeRenderer::establishConnectionsNG(){
	connect(ng, SIGNAL(progressValueChangedSignal(int)), this, SLOT(progressValueChangedSlot(int)));
	connect(ng, SIGNAL(finishedNormalsSignal()), this, SLOT(finishedNormalsSlot()));
}

/*! \brief Establece las conexiones de signals y slots entre la clase Reader y el controlador
*/
void VolumeRenderer::establishConnectionsREADER(){
	connect(reader, SIGNAL(progressValueChangedSignal(int)), this, SLOT(progressValueChangedSlot(int)));
	connect(reader, SIGNAL(dataFinishedReadingSignal()), ui.glwidget, SLOT(dataFinishedReadingSlot()));
	connect(reader, SIGNAL(dataFinishedReadingSignal()), this, SLOT(dataFinishedReadingSlot()));
	connect(reader, SIGNAL(objFinishedReadingSignal()), this, SLOT(finishedNormalsSlot()));		//When you finish reading the obj it is the same as when you finish the normals because you completely change the verts and the normals of the mesh

}






/*! \brief Principio de un intento de implementar el algoritmo de Ball pivot para triangulacion Delaunay
*/
void VolumeRenderer::ballPivot(){
	std::cout << "creating mesh with ball pivot algorithm" << std::endl;

	//grab a point from the gradients. Since gradients is still a map we convert it to two vector of positons and normals
	//Create another vector of sortedPoints which are sorted by the distance from the onw we picked
	//Pick two points from the sortedPoints (whithin a certain distance range) and see if those 3 points are compatible with the R-sphere

	pair <glm::vec3,glm::vec3> referencePoint;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;

	if (positions.empty()){
		for (int k = 0; k < model->pixelData->frames; k++){
			for (auto kv : model->gradient[k]) {
				glm::vec3 positionKey;
				positionKey.x = kv.first.first;
				positionKey.y = kv.first.second;
				positionKey.z = k;
				positions.push_back(positionKey);
				normals.push_back(kv.second);
			}
		}
	}
	std::vector<glm::vec3> sortedPositions;
	std::vector<glm::vec3> sortedNormals;


	for (int i = 0; i < positions.size(); i++){
		referencePoint.first = positions[i];
		referencePoint.second = normals[i];

		//Now that we have the point, we create a sortedPoints list and we sort the rest of them based on the distance from our reference point
		sortedPositions.clear();
		sortedNormals.clear();

		sortedPositions = positions;
		sortedNormals = normals;

		sortedPositions.erase(sortedPositions.begin() + i);
		sortedNormals.erase(sortedNormals.begin() + i);

		//Now we sort the points that remain, which are all the points minus the one grabbed by reference
		bool needsSwapping = true;
		while (needsSwapping){
			needsSwapping = false;
			for (int j = 0; j < sortedPositions.size() - 1; j++){

				double distance1 = sqrt(pow(2, sortedPositions[j].x - referencePoint.first.x) + pow(2, sortedPositions[j].y - referencePoint.first.y) + pow(2, sortedPositions[j].z - referencePoint.first.z));
				double distance2 = sqrt(pow(2, sortedPositions[j + 1].x - referencePoint.first.x) + pow(2, sortedPositions[j + 1].y - referencePoint.first.y) + pow(2, sortedPositions[j + 1].z - referencePoint.first.z));

				if (distance1 > distance2){
					//swap positions and normals
					glm::vec3 temp;
					temp = sortedPositions[j];
					sortedPositions[j] = sortedPositions[j + 1];
					sortedPositions[j + 1] = temp;

					temp = sortedNormals[j];
					sortedNormals[j] = sortedNormals[j + 1];
					sortedNormals[j + 1] = temp;

					needsSwapping = true;
				}
			}
		}
		//Now we have the sorted points and normals so now we have to search in it.

		std::cout << "distance between the reference and the first points is" << sqrt(pow(2, sortedPositions[0].x - referencePoint.first.x) + pow(2, sortedPositions[0].y - referencePoint.first.y) + pow(2, sortedPositions[0].z - referencePoint.first.z)) << std::endl;
		std::cout << "distance between the reference and the first points is" << sqrt(pow(2, sortedPositions[1].x - referencePoint.first.x) + pow(2, sortedPositions[1].y - referencePoint.first.y) + pow(2, sortedPositions[1].z - referencePoint.first.z)) << std::endl;
		std::cout << "distance between the reference and the first points is" << sqrt(pow(2, sortedPositions[2].x - referencePoint.first.x) + pow(2, sortedPositions[2].y - referencePoint.first.y) + pow(2, sortedPositions[2].z - referencePoint.first.z)) << std::endl;
		std::cout << "distance between the reference and the first points is" << sqrt(pow(2, sortedPositions[3].x - referencePoint.first.x) + pow(2, sortedPositions[3].y - referencePoint.first.y) + pow(2, sortedPositions[3].z - referencePoint.first.z)) << std::endl;
		break;	//I just use the break for debuggin so that i stop at the first reference point
		

	}

}






/*! \brief Escribe en el modelo el nuevo dato de isoLevel cuando cambia el slider correspondiente
*/
void VolumeRenderer::on_isoLevelSlider_valueChanged(){
	cout << "isolevel changed" << endl;

	if (model->isoLevel == ui.isoLevelSlider->value())
		return;

	model->isoLevel=ui.isoLevelSlider->value();
	//Now we should generate another mesh with the new isolevel, and refresh the openglwidget
	generateMesh();
	ui.glwidget->update();
	
}







/*! \brief Abre una ventana de dialogo donde seleccionar los archivos dicom. La ruta de los archivos se pasan a la clase Reader donde se leen los pixeles de cada archivo
*/
void VolumeRenderer::on_loadDICOMFromFile_clicked(){
	
	QFileDialog dialog(this);
	//dialog.setOption(QFileDialog::DontUseNativeDialog, true);
	//dialog.setDirectory(QDir::currentPath());
	dialog.setDirectory("E:\\Universidad\\Hecho por mi\\Volume Renderer\\Examples\\");
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilter(tr("Files (*.*)"));
	QStringList fileNames;
	if (dialog.exec())
		fileNames = dialog.selectedFiles();
	
	if (fileNames.empty())
		return;
	wipePoints();
	wipePixelData();

	ui.progressText->setText("Reading DICOM Files");
	reader = new FileReader;
	establishConnectionsREADER();
	
	boost::thread workerThread(boost::bind(&FileReader::loadDICOMPixelData, reader,fileNames,model->pixelData));

	ui.frameSlider->setMaximum(fileNames.size() - 1);
	

	

}

/*! \brief Slot asociado al final de la lectura de un archivo Dicom, actualiza el visor 2d y el valor maximo del umbral dependiendo de la profundidad de bits
*/

void VolumeRenderer::dataFinishedReadingSlot(){
	ui.progressText->setText("");
	ui.progressBar->setValue(0);

	
	if (model->pixelData->numberOfBytes == 1)
		ui.isoLevelSlider->setMaximum(255);
	if (model->pixelData->numberOfBytes == 2)
		ui.isoLevelSlider->setMaximum(65536);
	if (model->pixelData->numberOfBytes == 3)
		ui.isoLevelSlider->setMaximum(4294967296);

	generateMesh(1);
	ui.dicomviewer2dgl->setFrame(model->pixelData->frames / 2);

	ui.glwidget->update();
	emit dataFinishedReading();

}

/*! \brief Cambio en el tamaño de los cubos de Marching Cubes
*/
void VolumeRenderer::on_resolutionSlider_valueChanged(){
	cout << "Change the resolution to " << ui.resolutionSlider->value() << endl;
	model->cellSizeX = ui.resolutionSlider->value();
	model->cellSizeY = ui.resolutionSlider->value();
	model->cellSizeZ = ui.resolutionSlider->value();


	generateMesh();
	ui.glwidget->update();
}






void VolumeRenderer::on_toleranceSlider_valueChanged(){
	model->tolerance = ui.toleranceSlider->value();
	cout << "tolerance set to " << model->tolerance;
	//model->cubes.clear();
	generateMesh();
}

void VolumeRenderer::on_marchingCubesButton_clicked(){
	if (ui.marchingCubesButton->isChecked()){
		model->algorithmChosen = 1;
		generateMesh();
	}
}


void VolumeRenderer::on_adaptiveMarchingCubes2Button_clicked(){
	if (ui.adaptiveMarchingCubes2Button->isChecked()){
		model->algorithmChosen = 3;
		generateMesh();
	}
}
void VolumeRenderer::on_adaptiveMarchingCubes3Button_clicked(){
	if (ui.adaptiveMarchingCubes3Button->isChecked()){
		model->algorithmChosen = 4;
		generateMesh();
	}
}



void VolumeRenderer::on_normalsPerTriangleButton_clicked(){
	if (ui.normalsPerTriangleButton->isChecked()){
		model->normalsAlgChosen = 1;
	}
	if (!interactive)
		return;
	model->mesh->normals.clear();
	emit generatingStartedSignal();
	generatingMesh = true;
	generateNormals();
	
}
void VolumeRenderer::on_normalsPerVerticeButton_clicked(){
	if (ui.normalsPerVerticeButton->isChecked()){
		model->normalsAlgChosen = 2;
	}
	if (!interactive)
		return;
	model->mesh->normals.clear();
	emit generatingStartedSignal();
	generatingMesh = true;
	generateNormals();
	
}



/*! \brief Cambia la orientacion en la que se visualzia el volumen de datos
*
*  Cada vez que se cambia la orientacion en la que se observa el mallado hay que modificar el tamaño de los sliders para reflejar el tamaño correspondiente en esa dirrection
*  Por ejemplo visto desde el eje Z un volumen de 512x512x10 tendrá los sliders al maximo. Pero cuando se visualiza desde el X o Y, uno de los sliders se tiene que modificar
*  para quedar mucho mas pequeño (el 10 en el eje correspondiente).
*/
void VolumeRenderer::on_orientationZButton_clicked(){
	if (ui.orientationZButton->isChecked()){
		ui.dicomviewer2dgl->orientation = 1;
		ui.dicomviewer2dgl->frame_to_display = model->pixelData->frames / 2;
		ui.frameSlider->setValue(model->pixelData->frames / 2);
		ui.frameSlider->setMaximum(model->pixelData->frames);
		ui.dicomviewer2dgl->setFrame(model->pixelData->frames / 2);

		//Give values to the slider and maximums

		ui.borderYTopSlider->setValue(model->pixelData->borderYTop);
		ui.borderYTopSlider->setMaximum(model->pixelData->height);

		ui.borderYBottomSlider->setValue(model->pixelData->borderYBottom);
		ui.borderYBottomSlider->setMaximum(model->pixelData->height);
		
		ui.borderXLeftSlider->setValue(model->pixelData->borderXLeft);
		ui.borderXLeftSlider->setMaximum(model->pixelData->width);

		ui.borderXRightSlider->setValue(model->pixelData->borderXRight);
		ui.borderXRightSlider->setMaximum(model->pixelData->width);


		//Leave everything as original because in the z direction there cannot be any distorsion
		QRect rect;

		rect = ui.borderYTopSlider->geometry();
		rect.setBottom(370 + 231);
		rect.setTop(370);
		ui.borderYTopSlider->setGeometry(rect);
		rect = ui.borderYBottomSlider->geometry();
		rect.setBottom(370 + 231);
		rect.setTop(370);
		ui.borderYBottomSlider->setGeometry(rect);

		rect = ui.borderXLeftSlider->geometry();
		rect.setLeft(1100);
		rect.setRight(1100 + 231);
		ui.borderXLeftSlider->setGeometry(rect);
		rect = ui.borderXRightSlider->geometry();
		rect.setLeft(1100);
		rect.setRight(1100 + 231);
		ui.borderXRightSlider->setGeometry(rect);
		


	}
	
}
void VolumeRenderer::on_orientationXButton_clicked(){
	if (ui.orientationXButton->isChecked()){
		ui.dicomviewer2dgl->orientation = 2;
		ui.dicomviewer2dgl->frame_to_display = model->pixelData->width / 2;
		ui.frameSlider->setValue(model->pixelData->width / 2);
		ui.frameSlider->setMaximum(model->pixelData->width);
		ui.dicomviewer2dgl->setFrame(model->pixelData->width / 2);

		ui.borderYTopSlider->setValue(model->pixelData->borderZFurther);
		ui.borderYTopSlider->setMaximum(model->pixelData->frames);

		ui.borderYBottomSlider->setValue(model->pixelData->borderZCloser);
		ui.borderYBottomSlider->setMaximum(model->pixelData->frames);

		ui.borderXLeftSlider->setValue(model->pixelData->borderYBottom);
		ui.borderXLeftSlider->setMaximum(model->pixelData->height);

		ui.borderXRightSlider->setValue(model->pixelData->borderYTop);
		ui.borderXRightSlider->setMaximum(model->pixelData->height);



		if (model->pixelData->frames < model->pixelData->width){	//change Y leave X as original
			double ratio = (float)model->pixelData->frames / (float)model->pixelData->width;

			QRect rect;
	
			rect = ui.borderYTopSlider->geometry();
			rect.setBottom(370 + 231);
			rect.setTop(370 + (231 - 231 * ratio));
			ui.borderYTopSlider->setGeometry(rect);
			rect = ui.borderYBottomSlider->geometry();
			rect.setBottom(370 + 231);
			rect.setTop(370 + (231 - 231 * ratio));
			ui.borderYBottomSlider->setGeometry(rect);


			//Leave X as original
			rect = ui.borderXLeftSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231);
			ui.borderXLeftSlider->setGeometry(rect);
			rect = ui.borderXRightSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231);
			ui.borderXRightSlider->setGeometry(rect);



		}
		else{
			QRect rect;

			rect = ui.borderYTopSlider->geometry();
			rect.setBottom(370 + 231);
			rect.setTop(370);
			ui.borderYTopSlider->setGeometry(rect);

			rect = ui.borderYBottomSlider->geometry();
			rect.setBottom(370 + 231);
			rect.setTop(370);
			ui.borderYBottomSlider->setGeometry(rect);
			//modify X

			double ratio = (float)model->pixelData->width / (float)model->pixelData->frames;

			rect = ui.borderXLeftSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231*ratio);
			ui.borderXLeftSlider->setGeometry(rect);
			rect = ui.borderXRightSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231*ratio);
			ui.borderXRightSlider->setGeometry(rect);

		}



	}
}
void VolumeRenderer::on_orientationYButton_clicked(){
	if (ui.orientationYButton->isChecked()){
		ui.dicomviewer2dgl->orientation = 3;
		ui.dicomviewer2dgl->frame_to_display = model->pixelData->height / 2;
		ui.frameSlider->setValue(model->pixelData->height/2);
		ui.frameSlider->setMaximum(model->pixelData->height);
		ui.dicomviewer2dgl->setFrame(model->pixelData->height / 2);


		ui.borderYTopSlider->setValue(model->pixelData->borderZFurther);
		ui.borderYTopSlider->setMaximum(model->pixelData->frames);

		ui.borderYBottomSlider->setValue(model->pixelData->borderZCloser);
		ui.borderYBottomSlider->setMaximum(model->pixelData->frames);


		ui.borderXLeftSlider->setValue(model->pixelData->borderXLeft);
		ui.borderXLeftSlider->setMaximum(model->pixelData->width);

		ui.borderXRightSlider->setValue(model->pixelData->borderXRight);
		ui.borderXRightSlider->setMaximum(model->pixelData->width);


		if (model->pixelData->frames < model->pixelData->height){ //The image will be squashed down so we need to move the Border Y sliders to coincide
			
			double ratio = (float)model->pixelData->frames / (float)model->pixelData->height;

			QRect rect;
			//360 is the slider y position got from the designer, and the 231 it the slider height
			rect = ui.borderYTopSlider->geometry();
			rect.setBottom(370 + 231);
			rect.setTop(370 +  (231-231 * ratio));
			ui.borderYTopSlider->setGeometry(rect);
			rect = ui.borderYBottomSlider->geometry();
			rect.setBottom(370 + 231);
			rect.setTop(370 + (231 - 231 * ratio));
			ui.borderYBottomSlider->setGeometry(rect);



			//Leave X as original
			rect = ui.borderXLeftSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231);
			ui.borderXLeftSlider->setGeometry(rect);
			rect = ui.borderXRightSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231);
			ui.borderXRightSlider->setGeometry(rect);

		}
		else{ //In this case we need to change the border X slider to coincide and leave the Y to their original values;

			//Leve the y as original
			QRect rect;

			rect = ui.borderYTopSlider->geometry();
			rect.setBottom(370 + 231);
			rect.setTop(370);
			ui.borderYTopSlider->setGeometry(rect);

			rect = ui.borderYBottomSlider->geometry();
			rect.setBottom(370 + 231);
			rect.setTop(370);
			ui.borderYBottomSlider->setGeometry(rect);
			//Modify x

			double ratio = (float)model->pixelData->width / (float)model->pixelData->frames;

			rect = ui.borderXLeftSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231 * ratio);
			ui.borderXLeftSlider->setGeometry(rect);
			rect = ui.borderXRightSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231 * ratio);
			ui.borderXRightSlider->setGeometry(rect);

		}
		
	}
}



/*! \brief Funcion encargada de crear los objetos y los hilos necesarios para la creacion del mallado.
*
*  Si el sistema esta en modo interactivo esta funcion no se ejecuta, a no ser que el usuario fuerze la creacion de un mallado mediante el boton Generate Mesh.
*  La funcion se encarga de crear los objetos MarchingCuber o AdaptiveMarchingCuber y de crear un hilo donde se va a ejecutar la tarea principal de cada uno.
*/
void VolumeRenderer::generateMesh(int force){


	if (!force){			
		if (!interactive)
			return;
	}

	if (generatingMesh == true)
		return;

	wipePoints();
	timeStart = boost::posix_time::microsec_clock::universal_time();

	std::cout << "generating mesh with" << model->algorithmChosen << std::endl;

	emit generatingStartedSignal();
	generatingMesh = true;
	if (model->algorithmChosen == 1){
	
		ui.progressText->setText("<font color='black'>Generating Mesh</font>");
		mc = new MarchingCuber((model->pixelData), model->mesh, model->isoLevel, model->cellSizeX, model->cellSizeY, model->cellSizeZ, model->interpolateDepth);
		establishConnectionsMC();

		boost::thread workerThread(boost::bind(&MarchingCuber::run, mc));
		

	}
		
	if (model->algorithmChosen == 2){

		launch_kernel(model->edgeTable,model->triTable);
	}
	if (model->algorithmChosen == 3){
		amc = new AdaptiveCuber((model->pixelData), model->mesh, model->isoLevel, model->cellSizeX, model->cellSizeY, model->cellSizeZ, model->interpolateDepth, model->octreeMaxDepth, &(model->gradient), model->tolerance);
		establishConnectionsAMC();
		boost::thread workerThread(boost::bind(&AdaptiveCuber::runWithCracks, amc));
	}
	if (model->algorithmChosen == 4){
	
		amc = new AdaptiveCuber((model->pixelData), model->mesh, model->isoLevel, model->cellSizeX, model->cellSizeY, model->cellSizeZ, model->interpolateDepth, model->octreeMaxDepth, &(model->gradient), model->tolerance);
		establishConnectionsAMC();
		boost::thread workerThread(boost::bind(&AdaptiveCuber::run, amc));
	}

	
	QPalette plt;
	plt.setColor(QPalette::WindowText, Qt::white);
	ui.numberOfTrianglesLabel->setPalette(plt);
	
}

/*! \brief Slot que se ejecuta cuando el MarchingCuber o AdaptiveMarching cuber han acabado indicando que el mallado esta creado. Ejecuta la funcion de generateNormals para crear las normales.
*/
void VolumeRenderer::finishedMeshSlot(){

	ui.progressText->setText("<font color='black'>Generating Normals</font>");
	generateNormals();
}

/*! \brief Slot indicando que NormalsGenerator ha acabado por lo tanto el mallado esta listo para representarse asi que se emite la señal correspondiente al visor en 3D.
*/
void VolumeRenderer::finishedNormalsSlot(){

	model->mesh->height = model->pixelData->height;
	model->mesh->width = model->pixelData->width;
	model->mesh->frames = model->pixelData->frames;
	emit generatingFinishedSignal();
	ui.progressText->setText("<font color='black'></font>");
	ui.progressBar->setValue(0);
	generatingMesh = false;
	ui.glwidget->update();

	//Write the number of triangles
	QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
	QLocale aEnglish;
	QString string;
	string = aEnglish.toString(model->mesh->verts.size() / 3);
	string.append(" triangles");
	ui.numberOfTrianglesLabel->setText(string);

	//Write the time 
	timeFinish = boost::posix_time::microsec_clock::universal_time();
	boost::posix_time::time_duration duration = timeFinish - timeStart;
	duration.seconds();
	std::stringstream durationString;
	durationString << duration;
	std::string stringStd;
	stringStd = durationString.str();
	ui.timeLabel->setText(QString(stringStd.c_str()));

}



/*! \brief Funcion que crea el objeto NormalsGenerator y genera un hilo para ejecutar su funcion (normales por triangulos o por vertices dependiendo del algoritmo seleccionado)
*/
void VolumeRenderer::generateNormals(){

	ui.progressText->setText("Generating Normals");

	model->mesh->normals.clear();
	if (model->normalsAlgChosen == 1){

		ng = new NormalsGenerator();
		establishConnectionsNG();
		//ng->normalsPerTriangle(model->pixelData, model->verts, model->normals);
		boost::thread workerThread(boost::bind(&NormalsGenerator::normalsPerTriangle, ng, model->pixelData, model->mesh));
	}

	if (model->normalsAlgChosen == 2){

		ng = new NormalsGenerator();
		establishConnectionsNG();
		//ng->normalsPerVertex(model->pixelData, model->verts, model->normals);
		boost::thread workerThread(boost::bind(&NormalsGenerator::normalsPerVertex, ng, model->pixelData, model->mesh));
	}

}





/*! \brief Borra todos los puntos del mallado, sus normales y los cubos que se han creado, dejando listo el mesh para un nuevo mallado
*/
void VolumeRenderer::wipePoints(){
	
	//model->cubes.clear();
	model->mesh->verts.clear();
	model->mesh->normals.clear();
	//model->octreeVector.clear();
}

/*! \brief Borra todos los datos de los pixels, dejando listo al modelo para la carga de otros archivos DICOM
*/
void VolumeRenderer::wipePixelData(){
	int numberOfFrees=0;

	if (model->pixelData->data == NULL)
		return;

	for (int i = 0; i < model->pixelData->frames; i++){
		free(model->pixelData->data[i]);
		numberOfFrees++;
	}

	model->gradient.clear();
	free(model->pixelData->data);
	model->pixelData->data = NULL;
	model->pixelData->numberOfBytes = 0;;
	
	ui.glwidget->xMove = 0; //Now we reset the position so that the new model will be centered
	ui.glwidget->yMove = 0;
	ui.glwidget->zMove = 0;
	ui.glwidget->xRot = 0;
	ui.glwidget->yRot = 0;

}

/*! \brief Actualiza el valor del ProgressBar con los valores que se envian a traves de las señales de los diferentes hilos
*/
void VolumeRenderer::progressValueChangedSlot(int newValue){
	cout << "new value of progress" << newValue << endl;
	ui.progressBar->setValue(newValue);
}

/*! \brief Slots que recibirá las señales con el texto indicando la fase en la que se encuentra cada modulo. Escribe el texto en la UI
*/
void VolumeRenderer::progressTextSlot(QString text){

	ui.progressText->setText(text);
}




void VolumeRenderer::on_linearInterpolationSlider_valueChanged(){
	model->interpolateDepth = ui.linearInterpolationSlider->value();
	cout << "interpolate set to " << model->interpolateDepth;
	//model->cubes.clear();
	generateMesh();
}

/*! \brief Fuerza la generacion de un mallado a partir de las opciones seleccionadas. Se creará un mallado aunque el sistema no este en el modo interactivo
*/
void VolumeRenderer::on_generateMeshButton_clicked(){
	//std::cout << "clicked on generate mesh" << std::endl;
	generateMesh(1);	//We force the generation of a mesh by passing it 1
}

/*! \brief Establece el modo interactivo, es decir se creara un mallado automaticamente al cambiar cualquier opcion.
*/
void VolumeRenderer::on_interactiveButton_clicked(){
	if (ui.interactiveButton->isChecked()){
		interactive = true;
	}
	else{
		interactive = false;
	}
}

void  VolumeRenderer::on_octreeDepthSlider_valueChanged(){
	model->octreeMaxDepth = ui.octreeDepthSlider->value();
	cout << "octree depth set to " << model->octreeMaxDepth;
	generateMesh();
}



/*! \brief Cada vez que se mueve uno de los sliders, el pixel data actualiza sus planos de corte en la dirreccion indicada
*/
void VolumeRenderer::on_borderYBottomSlider_valueChanged(){

	if (ui.dicomviewer2dgl->orientation == 1){
		model->pixelData->borderYBottom = ui.borderYBottomSlider->value();
	}
	if (ui.dicomviewer2dgl->orientation == 2){
		model->pixelData->borderZCloser = ui.borderYBottomSlider->value();
	}
	if (ui.dicomviewer2dgl->orientation == 3){
		model->pixelData->borderZCloser = ui.borderYBottomSlider->value();
	}
	generateMesh();
}
void VolumeRenderer::on_borderYTopSlider_valueChanged(){
	if (ui.dicomviewer2dgl->orientation == 1){
		model->pixelData->borderYTop = ui.borderYTopSlider->value();
	}
	if (ui.dicomviewer2dgl->orientation == 2){
		model->pixelData->borderZFurther = ui.borderYTopSlider->value();
	}
	if (ui.dicomviewer2dgl->orientation == 3){
		model->pixelData->borderZFurther = ui.borderYTopSlider->value();
	}
	
	generateMesh();

}




void VolumeRenderer::on_borderXLeftSlider_valueChanged(){

	if (ui.dicomviewer2dgl->orientation == 1){
		model->pixelData->borderXLeft = ui.borderXLeftSlider->value();
	}
	if (ui.dicomviewer2dgl->orientation == 2){
		model->pixelData->borderYBottom = ui.borderXLeftSlider->value();
	}
	if (ui.dicomviewer2dgl->orientation == 3){
		model->pixelData->borderXLeft = ui.borderXLeftSlider->value();
	}
	generateMesh();
}
void VolumeRenderer::on_borderXRightSlider_valueChanged(){
	if (ui.dicomviewer2dgl->orientation == 1){
		model->pixelData->borderXRight = ui.borderXRightSlider->value();
	}
	if (ui.dicomviewer2dgl->orientation == 2){
		model->pixelData->borderYTop = ui.borderXRightSlider->value();
	}
	if (ui.dicomviewer2dgl->orientation == 3){
		model->pixelData->borderXRight = ui.borderXRightSlider->value();
	}

	generateMesh();

}


/*! \brief Recoge el fichero a obj leer y lo envia al modulo FileReader para que se lean los vertices y normales en el nuevo malaldo
*/
void VolumeRenderer::on_loadObjButton_clicked(){
	std::cout << "loading obj file" << std::endl;
	

	QString fileName=NULL;
	fileName = QFileDialog::getOpenFileName(this,
		tr("Open OBJ File"), "/home", tr("OBJ File (*.obj)"));
	
	if (fileName == NULL)
		return;

	//sends a signal to stop rendering the mesh because we are updating its points.
	emit generatingStartedSignal();
	wipePoints();

	ui.progressText->setText("Reading OBJ File");
	reader = new FileReader;
	establishConnectionsREADER();
	boost::thread workerThread(boost::bind(&FileReader::loadOBJFile, reader, fileName, model->pixelData, model->mesh));


}

/*! \brief Se recoge el fichero donde se quiere guardar y se lanza en un nuevo hilo la funcion de exportacion del mallado.
*/
void VolumeRenderer::on_writeObjButton_clicked(){
	std::cout << "writing obj file" << std::endl;
	
	
	QString filters("OBJ files (*.obj);;Text files (*.txt);;All files (*.*)");
	QString defaultFilter("OBJ files (*.obj)");

	/* Static method approach */
	QString filename; 
	filename = QFileDialog::getSaveFileName(0, "Save file", QDir::currentPath(),
		filters, &defaultFilter);
	
	if (filename.isEmpty())
		return;

	ui.progressText->setText("Exporting to OBJ");


	connect(model->mesh, SIGNAL(progressValueChangedSignal(int)), this, SLOT(progressValueChangedSlot(int)));
	connect(model->mesh, SIGNAL(finishedWritingToFileSignal()), this, SLOT(finishedNormalsSlot()));
	
	boost::thread workerThread(boost::bind(&Mesh::writeToOBJ, model->mesh, filename));

}


/*! \brief Copia el mesh principal en el secundario
*/
void VolumeRenderer::on_saveToSecondaryButton_clicked(){
	std::cout << "Copying from mesh 1 to secondary" << std::endl;

	model->mesh2->verts.clear();
	model->mesh2->normals.clear();

	for (int i = 0; i < model->mesh->verts.size(); i++){
		model->mesh2->verts.push_back(model->mesh->verts[i]);
		model->mesh2->normals.push_back(model->mesh->normals[i]);
	}
}

/*! \brief Borra el mesh secundario
*/
void VolumeRenderer::on_clearPinButton_clicked(){
	std::cout << "Clearing pined mesh" << std::endl;
	model->mesh2->verts.clear();
	model->mesh2->normals.clear();
}



/*! \brief Toggle para el movimiento de la figura. Usado como alternativa a los atajos de Ctrl y Shift
*/
void VolumeRenderer::on_moveButton_clicked(){
	if (ui.moveButton->isChecked()){
		ui.scaleButton->setChecked(false);
		ui.glwidget->scalingFigure = false;
		ui.glwidget->movingFigure = true;
	}
	else{
		ui.glwidget->movingFigure = false;
	}
}
void VolumeRenderer::on_scaleButton_clicked(){
	if (ui.scaleButton->isChecked()){
		ui.moveButton->setChecked(false);
		ui.glwidget->movingFigure = false;
		ui.glwidget->scalingFigure = true;
	}
	else{
		ui.glwidget->scalingFigure = false;
	}
}