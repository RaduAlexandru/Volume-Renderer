/********************************************************************************
** Form generated from reading UI file 'volumerenderer.ui'
**
** Created by: Qt User Interface Compiler version 5.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VOLUMERENDERER_H
#define UI_VOLUMERENDERER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "dicomviewer2dgl.h"
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_VolumeRendererClass
{
public:
    QWidget *centralWidget;
    QPushButton *test;
    GLWidget *glwidget;
    QLabel *numberOfTrianglesLabel;
    QSlider *frameSlider;
    QPushButton *addDICOMFiles;
    QSlider *isoLevelSlider;
    QPushButton *loadTestImageButton;
    QPushButton *loadDICOMButton;
    QPushButton *loadTestImage2Button;
    QLabel *label;
    QLabel *label_2;
    QSlider *resolutionSlider;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QRadioButton *linearInterpolationButton;
    QGroupBox *groupBox;
    QCheckBox *pointFlagButton;
    QCheckBox *borderFlagButton;
    QPushButton *loadMFDICOMButton;
    DicomViewer2DGL *dicomviewer2dgl;
    QSlider *toleranceSlider;
    QPushButton *loadDICOMFromFile;
    QCheckBox *showWireframeButton;
    QCheckBox *perspectiveButton;
    QLineEdit *interpolateDepthText;
    QLabel *label_7;
    QGroupBox *groupBox_2;
    QRadioButton *marchingCubesButton;
    QRadioButton *adaptiveMarchingCubesButton;
    QRadioButton *adaptiveMarchingCubes2Button;
    QRadioButton *ballPivotButton;
    QRadioButton *adaptiveMarchingCubes3Button;
    QGroupBox *groupBox_3;
    QRadioButton *normalsPerTriangleButton;
    QRadioButton *normalsPerVerticeButton;
    QLabel *label_8;
    QCheckBox *showCubesButton;
    QSlider *horizontalSlider;
    QLabel *label_9;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QCheckBox *showMeshButton;
    QCheckBox *showGradientButton;
    QGroupBox *groupBox_4;
    QRadioButton *orientationZButton;
    QRadioButton *orientationXButton;
    QRadioButton *orientationYButton;
    QProgressBar *progressBar;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VolumeRendererClass)
    {
        if (VolumeRendererClass->objectName().isEmpty())
            VolumeRendererClass->setObjectName(QStringLiteral("VolumeRendererClass"));
        VolumeRendererClass->resize(1346, 692);
        centralWidget = new QWidget(VolumeRendererClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        test = new QPushButton(centralWidget);
        test->setObjectName(QStringLiteral("test"));
        test->setGeometry(QRect(1070, 40, 75, 23));
        glwidget = new GLWidget(centralWidget);
        glwidget->setObjectName(QStringLiteral("glwidget"));
        glwidget->setEnabled(true);
        glwidget->setGeometry(QRect(250, 10, 701, 591));
        numberOfTrianglesLabel = new QLabel(glwidget);
        numberOfTrianglesLabel->setObjectName(QStringLiteral("numberOfTrianglesLabel"));
        numberOfTrianglesLabel->setGeometry(QRect(560, 560, 131, 20));
        numberOfTrianglesLabel->setTextFormat(Qt::RichText);
        numberOfTrianglesLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        frameSlider = new QSlider(centralWidget);
        frameSlider->setObjectName(QStringLiteral("frameSlider"));
        frameSlider->setGeometry(QRect(1020, 600, 121, 22));
        frameSlider->setOrientation(Qt::Horizontal);
        addDICOMFiles = new QPushButton(centralWidget);
        addDICOMFiles->setObjectName(QStringLiteral("addDICOMFiles"));
        addDICOMFiles->setGeometry(QRect(1070, 90, 91, 23));
        isoLevelSlider = new QSlider(centralWidget);
        isoLevelSlider->setObjectName(QStringLiteral("isoLevelSlider"));
        isoLevelSlider->setGeometry(QRect(70, 360, 121, 22));
        isoLevelSlider->setMaximum(255);
        isoLevelSlider->setValue(100);
        isoLevelSlider->setOrientation(Qt::Horizontal);
        loadTestImageButton = new QPushButton(centralWidget);
        loadTestImageButton->setObjectName(QStringLiteral("loadTestImageButton"));
        loadTestImageButton->setGeometry(QRect(1220, 10, 81, 23));
        loadDICOMButton = new QPushButton(centralWidget);
        loadDICOMButton->setObjectName(QStringLiteral("loadDICOMButton"));
        loadDICOMButton->setGeometry(QRect(1220, 70, 75, 23));
        loadTestImage2Button = new QPushButton(centralWidget);
        loadTestImage2Button->setObjectName(QStringLiteral("loadTestImage2Button"));
        loadTestImage2Button->setGeometry(QRect(1210, 40, 91, 23));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 360, 47, 13));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(970, 600, 47, 13));
        resolutionSlider = new QSlider(centralWidget);
        resolutionSlider->setObjectName(QStringLiteral("resolutionSlider"));
        resolutionSlider->setGeometry(QRect(70, 390, 121, 22));
        resolutionSlider->setMinimum(1);
        resolutionSlider->setMaximum(20);
        resolutionSlider->setValue(6);
        resolutionSlider->setOrientation(Qt::Horizontal);
        resolutionSlider->setInvertedAppearance(true);
        resolutionSlider->setTickPosition(QSlider::TicksBelow);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 390, 51, 16));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(210, 390, 47, 13));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(210, 360, 47, 13));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(1150, 610, 47, 13));
        linearInterpolationButton = new QRadioButton(centralWidget);
        linearInterpolationButton->setObjectName(QStringLiteral("linearInterpolationButton"));
        linearInterpolationButton->setGeometry(QRect(10, 450, 111, 17));
        linearInterpolationButton->setChecked(true);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(990, 130, 141, 80));
        pointFlagButton = new QCheckBox(groupBox);
        pointFlagButton->setObjectName(QStringLiteral("pointFlagButton"));
        pointFlagButton->setGeometry(QRect(10, 20, 111, 17));
        borderFlagButton = new QCheckBox(groupBox);
        borderFlagButton->setObjectName(QStringLiteral("borderFlagButton"));
        borderFlagButton->setGeometry(QRect(10, 50, 111, 17));
        loadMFDICOMButton = new QPushButton(centralWidget);
        loadMFDICOMButton->setObjectName(QStringLiteral("loadMFDICOMButton"));
        loadMFDICOMButton->setGeometry(QRect(1200, 100, 121, 23));
        dicomviewer2dgl = new DicomViewer2DGL(centralWidget);
        dicomviewer2dgl->setObjectName(QStringLiteral("dicomviewer2dgl"));
        dicomviewer2dgl->setGeometry(QRect(960, 250, 371, 341));
        toleranceSlider = new QSlider(centralWidget);
        toleranceSlider->setObjectName(QStringLiteral("toleranceSlider"));
        toleranceSlider->setGeometry(QRect(70, 410, 121, 22));
        toleranceSlider->setMaximum(360);
        toleranceSlider->setOrientation(Qt::Horizontal);
        loadDICOMFromFile = new QPushButton(centralWidget);
        loadDICOMFromFile->setObjectName(QStringLiteral("loadDICOMFromFile"));
        loadDICOMFromFile->setGeometry(QRect(30, 20, 111, 23));
        showWireframeButton = new QCheckBox(centralWidget);
        showWireframeButton->setObjectName(QStringLiteral("showWireframeButton"));
        showWireframeButton->setGeometry(QRect(120, 520, 101, 17));
        perspectiveButton = new QCheckBox(centralWidget);
        perspectiveButton->setObjectName(QStringLiteral("perspectiveButton"));
        perspectiveButton->setGeometry(QRect(20, 520, 81, 17));
        perspectiveButton->setChecked(true);
        interpolateDepthText = new QLineEdit(centralWidget);
        interpolateDepthText->setObjectName(QStringLiteral("interpolateDepthText"));
        interpolateDepthText->setGeometry(QRect(190, 450, 51, 20));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(130, 450, 61, 16));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 120, 171, 141));
        marchingCubesButton = new QRadioButton(groupBox_2);
        marchingCubesButton->setObjectName(QStringLiteral("marchingCubesButton"));
        marchingCubesButton->setGeometry(QRect(10, 20, 111, 17));
        marchingCubesButton->setChecked(true);
        adaptiveMarchingCubesButton = new QRadioButton(groupBox_2);
        adaptiveMarchingCubesButton->setObjectName(QStringLiteral("adaptiveMarchingCubesButton"));
        adaptiveMarchingCubesButton->setGeometry(QRect(10, 40, 161, 17));
        adaptiveMarchingCubes2Button = new QRadioButton(groupBox_2);
        adaptiveMarchingCubes2Button->setObjectName(QStringLiteral("adaptiveMarchingCubes2Button"));
        adaptiveMarchingCubes2Button->setGeometry(QRect(10, 60, 161, 17));
        ballPivotButton = new QRadioButton(groupBox_2);
        ballPivotButton->setObjectName(QStringLiteral("ballPivotButton"));
        ballPivotButton->setGeometry(QRect(10, 110, 82, 17));
        adaptiveMarchingCubes3Button = new QRadioButton(groupBox_2);
        adaptiveMarchingCubes3Button->setObjectName(QStringLiteral("adaptiveMarchingCubes3Button"));
        adaptiveMarchingCubes3Button->setGeometry(QRect(10, 80, 161, 17));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 270, 141, 80));
        normalsPerTriangleButton = new QRadioButton(groupBox_3);
        normalsPerTriangleButton->setObjectName(QStringLiteral("normalsPerTriangleButton"));
        normalsPerTriangleButton->setGeometry(QRect(10, 20, 131, 17));
        normalsPerTriangleButton->setChecked(true);
        normalsPerVerticeButton = new QRadioButton(groupBox_3);
        normalsPerVerticeButton->setObjectName(QStringLiteral("normalsPerVerticeButton"));
        normalsPerVerticeButton->setGeometry(QRect(10, 40, 131, 17));
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 410, 47, 13));
        showCubesButton = new QCheckBox(centralWidget);
        showCubesButton->setObjectName(QStringLiteral("showCubesButton"));
        showCubesButton->setGeometry(QRect(20, 550, 81, 17));
        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(100, 490, 101, 22));
        horizontalSlider->setOrientation(Qt::Horizontal);
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(20, 490, 71, 16));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(30, 60, 75, 23));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(30, 90, 75, 23));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(130, 60, 111, 23));
        showMeshButton = new QCheckBox(centralWidget);
        showMeshButton->setObjectName(QStringLiteral("showMeshButton"));
        showMeshButton->setGeometry(QRect(120, 550, 70, 17));
        showMeshButton->setChecked(true);
        showGradientButton = new QCheckBox(centralWidget);
        showGradientButton->setObjectName(QStringLiteral("showGradientButton"));
        showGradientButton->setGeometry(QRect(120, 580, 91, 17));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(1160, 130, 120, 101));
        orientationZButton = new QRadioButton(groupBox_4);
        orientationZButton->setObjectName(QStringLiteral("orientationZButton"));
        orientationZButton->setGeometry(QRect(20, 20, 82, 17));
        orientationZButton->setChecked(true);
        orientationXButton = new QRadioButton(groupBox_4);
        orientationXButton->setObjectName(QStringLiteral("orientationXButton"));
        orientationXButton->setGeometry(QRect(20, 40, 82, 17));
        orientationYButton = new QRadioButton(groupBox_4);
        orientationYButton->setObjectName(QStringLiteral("orientationYButton"));
        orientationYButton->setGeometry(QRect(20, 60, 82, 17));
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(280, 610, 301, 23));
        progressBar->setValue(0);
        VolumeRendererClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(VolumeRendererClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1346, 21));
        VolumeRendererClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(VolumeRendererClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        VolumeRendererClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(VolumeRendererClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        VolumeRendererClass->setStatusBar(statusBar);

        retranslateUi(VolumeRendererClass);
        QObject::connect(resolutionSlider, SIGNAL(valueChanged(int)), label_4, SLOT(setNum(int)));
        QObject::connect(frameSlider, SIGNAL(valueChanged(int)), label_6, SLOT(setNum(int)));
        QObject::connect(isoLevelSlider, SIGNAL(valueChanged(int)), label_5, SLOT(setNum(int)));
        QObject::connect(toleranceSlider, SIGNAL(valueChanged(int)), dicomviewer2dgl, SLOT(setTolerance(int)));

        QMetaObject::connectSlotsByName(VolumeRendererClass);
    } // setupUi

    void retranslateUi(QMainWindow *VolumeRendererClass)
    {
        VolumeRendererClass->setWindowTitle(QApplication::translate("VolumeRendererClass", "VolumeRenderer", 0));
        test->setText(QApplication::translate("VolumeRendererClass", "test", 0));
        numberOfTrianglesLabel->setText(QApplication::translate("VolumeRendererClass", "<html><head/><body><p><br/></p></body></html>", 0));
        addDICOMFiles->setText(QApplication::translate("VolumeRendererClass", "AddDICOMFiles", 0));
        loadTestImageButton->setText(QApplication::translate("VolumeRendererClass", "loadTestImage", 0));
        loadDICOMButton->setText(QApplication::translate("VolumeRendererClass", "loadDICOM", 0));
        loadTestImage2Button->setText(QApplication::translate("VolumeRendererClass", "loadTestImage2", 0));
        label->setText(QApplication::translate("VolumeRendererClass", "IsoLevel", 0));
        label_2->setText(QApplication::translate("VolumeRendererClass", "Frame", 0));
        label_3->setText(QApplication::translate("VolumeRendererClass", "Resolution", 0));
        label_4->setText(QApplication::translate("VolumeRendererClass", "6", 0));
        label_5->setText(QApplication::translate("VolumeRendererClass", "100", 0));
        label_6->setText(QApplication::translate("VolumeRendererClass", "0", 0));
        linearInterpolationButton->setText(QApplication::translate("VolumeRendererClass", "linearInterpolation", 0));
        groupBox->setTitle(QApplication::translate("VolumeRendererClass", "PointRepresentation", 0));
        pointFlagButton->setText(QApplication::translate("VolumeRendererClass", "showPointCloud", 0));
        borderFlagButton->setText(QApplication::translate("VolumeRendererClass", "showPointInBorder", 0));
        loadMFDICOMButton->setText(QApplication::translate("VolumeRendererClass", "loadMultiFrameDicom", 0));
        loadDICOMFromFile->setText(QApplication::translate("VolumeRendererClass", "loadDICOMFromFile", 0));
        showWireframeButton->setText(QApplication::translate("VolumeRendererClass", "showWireframe", 0));
        perspectiveButton->setText(QApplication::translate("VolumeRendererClass", "Perspective", 0));
        interpolateDepthText->setText(QApplication::translate("VolumeRendererClass", "10", 0));
        label_7->setText(QApplication::translate("VolumeRendererClass", "InterpDepth", 0));
        groupBox_2->setTitle(QApplication::translate("VolumeRendererClass", "Algorithm", 0));
        marchingCubesButton->setText(QApplication::translate("VolumeRendererClass", "Marching Cubes", 0));
        adaptiveMarchingCubesButton->setText(QApplication::translate("VolumeRendererClass", "Adaptive Marching Cubes", 0));
        adaptiveMarchingCubes2Button->setText(QApplication::translate("VolumeRendererClass", "Adaptive Marching Cubes 2", 0));
        ballPivotButton->setText(QApplication::translate("VolumeRendererClass", "Ball Pivot", 0));
        adaptiveMarchingCubes3Button->setText(QApplication::translate("VolumeRendererClass", "Adaptive Marching Cubes 3", 0));
        groupBox_3->setTitle(QApplication::translate("VolumeRendererClass", "Normals", 0));
        normalsPerTriangleButton->setText(QApplication::translate("VolumeRendererClass", "Normals Per Triangle", 0));
        normalsPerVerticeButton->setText(QApplication::translate("VolumeRendererClass", "Normals Per Vertice ", 0));
        label_8->setText(QApplication::translate("VolumeRendererClass", "Tolerance", 0));
        showCubesButton->setText(QApplication::translate("VolumeRendererClass", "showCubes", 0));
        label_9->setText(QApplication::translate("VolumeRendererClass", "OctreeDepth", 0));
        pushButton->setText(QApplication::translate("VolumeRendererClass", "writeToObj", 0));
        pushButton_2->setText(QApplication::translate("VolumeRendererClass", "readObj", 0));
        pushButton_3->setText(QApplication::translate("VolumeRendererClass", "ResetModelPosition", 0));
        showMeshButton->setText(QApplication::translate("VolumeRendererClass", "showMesh", 0));
        showGradientButton->setText(QApplication::translate("VolumeRendererClass", "showGradient", 0));
        groupBox_4->setTitle(QApplication::translate("VolumeRendererClass", "Orientation", 0));
        orientationZButton->setText(QApplication::translate("VolumeRendererClass", "Z", 0));
        orientationXButton->setText(QApplication::translate("VolumeRendererClass", "X", 0));
        orientationYButton->setText(QApplication::translate("VolumeRendererClass", "Y", 0));
    } // retranslateUi

};

namespace Ui {
    class VolumeRendererClass: public Ui_VolumeRendererClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOLUMERENDERER_H
