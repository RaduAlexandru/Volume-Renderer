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
    GLWidget *glwidget;
    QLabel *numberOfTrianglesLabel;
    QPushButton *pushButton;
    QSlider *frameSlider;
    QLabel *label_2;
    QLabel *label_6;
    DicomViewer2DGL *dicomviewer2dgl;
    QPushButton *loadDICOMFromFile;
    QCheckBox *showWireframeButton;
    QCheckBox *perspectiveButton;
    QGroupBox *groupBox_2;
    QRadioButton *marchingCubesButton;
    QRadioButton *adaptiveMarchingCubesButton;
    QRadioButton *adaptiveMarchingCubes2Button;
    QRadioButton *ballPivotButton;
    QRadioButton *adaptiveMarchingCubes3Button;
    QGroupBox *groupBox_3;
    QRadioButton *normalsPerTriangleButton;
    QRadioButton *normalsPerVerticeButton;
    QCheckBox *showCubesButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QCheckBox *showMeshButton;
    QCheckBox *showGradientButton;
    QGroupBox *groupBox_4;
    QRadioButton *orientationZButton;
    QRadioButton *orientationXButton;
    QRadioButton *orientationYButton;
    QProgressBar *progressBar;
    QLineEdit *xPosText;
    QLineEdit *yPosText;
    QLineEdit *zPosText;
    QLabel *label_4;
    QSlider *toleranceSlider;
    QLabel *label_9;
    QLabel *label_8;
    QLabel *label_3;
    QSlider *resolutionSlider;
    QSlider *octreeDepthSlider;
    QSlider *isoLevelSlider;
    QLabel *label_5;
    QLabel *label_7;
    QCheckBox *interactiveButton;
    QSlider *linearInterpolationSlider;
    QLabel *label;
    QPushButton *generateMeshButton;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *label_12;
    QSlider *borderYBottomSlider;
    QSlider *borderYTopSlider;
    QSlider *borderXLeftSlider;
    QSlider *borderXRightSlider;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VolumeRendererClass)
    {
        if (VolumeRendererClass->objectName().isEmpty())
            VolumeRendererClass->setObjectName(QStringLiteral("VolumeRendererClass"));
        VolumeRendererClass->resize(1343, 705);
        centralWidget = new QWidget(VolumeRendererClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        glwidget = new GLWidget(centralWidget);
        glwidget->setObjectName(QStringLiteral("glwidget"));
        glwidget->setEnabled(true);
        glwidget->setGeometry(QRect(0, 10, 1361, 641));
        numberOfTrianglesLabel = new QLabel(glwidget);
        numberOfTrianglesLabel->setObjectName(QStringLiteral("numberOfTrianglesLabel"));
        numberOfTrianglesLabel->setGeometry(QRect(560, 560, 131, 20));
        numberOfTrianglesLabel->setTextFormat(Qt::RichText);
        numberOfTrianglesLabel->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        pushButton = new QPushButton(glwidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(660, 460, 75, 23));
        frameSlider = new QSlider(centralWidget);
        frameSlider->setObjectName(QStringLiteral("frameSlider"));
        frameSlider->setGeometry(QRect(1170, 300, 121, 22));
        frameSlider->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(1120, 300, 47, 13));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(1300, 300, 47, 13));
        dicomviewer2dgl = new DicomViewer2DGL(centralWidget);
        dicomviewer2dgl->setObjectName(QStringLiteral("dicomviewer2dgl"));
        dicomviewer2dgl->setGeometry(QRect(1100, 360, 231, 231));
        loadDICOMFromFile = new QPushButton(centralWidget);
        loadDICOMFromFile->setObjectName(QStringLiteral("loadDICOMFromFile"));
        loadDICOMFromFile->setGeometry(QRect(160, 470, 111, 23));
        showWireframeButton = new QCheckBox(centralWidget);
        showWireframeButton->setObjectName(QStringLiteral("showWireframeButton"));
        showWireframeButton->setGeometry(QRect(1180, 160, 101, 17));
        perspectiveButton = new QCheckBox(centralWidget);
        perspectiveButton->setObjectName(QStringLiteral("perspectiveButton"));
        perspectiveButton->setGeometry(QRect(1180, 100, 81, 17));
        perspectiveButton->setChecked(true);
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(310, 450, 171, 141));
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
        ballPivotButton->setGeometry(QRect(10, 100, 82, 17));
        adaptiveMarchingCubes3Button = new QRadioButton(groupBox_2);
        adaptiveMarchingCubes3Button->setObjectName(QStringLiteral("adaptiveMarchingCubes3Button"));
        adaptiveMarchingCubes3Button->setGeometry(QRect(10, 80, 161, 17));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(500, 450, 141, 80));
        normalsPerTriangleButton = new QRadioButton(groupBox_3);
        normalsPerTriangleButton->setObjectName(QStringLiteral("normalsPerTriangleButton"));
        normalsPerTriangleButton->setGeometry(QRect(10, 20, 131, 17));
        normalsPerTriangleButton->setChecked(true);
        normalsPerVerticeButton = new QRadioButton(groupBox_3);
        normalsPerVerticeButton->setObjectName(QStringLiteral("normalsPerVerticeButton"));
        normalsPerVerticeButton->setGeometry(QRect(10, 40, 131, 17));
        showCubesButton = new QCheckBox(centralWidget);
        showCubesButton->setObjectName(QStringLiteral("showCubesButton"));
        showCubesButton->setGeometry(QRect(1180, 190, 81, 17));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(160, 510, 111, 23));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(1180, 70, 111, 23));
        showMeshButton = new QCheckBox(centralWidget);
        showMeshButton->setObjectName(QStringLiteral("showMeshButton"));
        showMeshButton->setGeometry(QRect(1180, 130, 70, 17));
        showMeshButton->setChecked(true);
        showGradientButton = new QCheckBox(centralWidget);
        showGradientButton->setObjectName(QStringLiteral("showGradientButton"));
        showGradientButton->setGeometry(QRect(1180, 220, 91, 17));
        groupBox_4 = new QGroupBox(centralWidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(1100, 320, 231, 41));
        orientationZButton = new QRadioButton(groupBox_4);
        orientationZButton->setObjectName(QStringLiteral("orientationZButton"));
        orientationZButton->setGeometry(QRect(80, 20, 31, 17));
        orientationZButton->setChecked(true);
        orientationXButton = new QRadioButton(groupBox_4);
        orientationXButton->setObjectName(QStringLiteral("orientationXButton"));
        orientationXButton->setGeometry(QRect(10, 20, 31, 17));
        orientationYButton = new QRadioButton(groupBox_4);
        orientationYButton->setObjectName(QStringLiteral("orientationYButton"));
        orientationYButton->setGeometry(QRect(50, 20, 31, 17));
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(340, 10, 301, 16));
        progressBar->setValue(30);
        xPosText = new QLineEdit(centralWidget);
        xPosText->setObjectName(QStringLiteral("xPosText"));
        xPosText->setGeometry(QRect(20, 390, 61, 20));
        yPosText = new QLineEdit(centralWidget);
        yPosText->setObjectName(QStringLiteral("yPosText"));
        yPosText->setGeometry(QRect(90, 390, 61, 20));
        zPosText = new QLineEdit(centralWidget);
        zPosText->setObjectName(QStringLiteral("zPosText"));
        zPosText->setGeometry(QRect(160, 390, 71, 20));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(240, 240, 31, 16));
        toleranceSlider = new QSlider(centralWidget);
        toleranceSlider->setObjectName(QStringLiteral("toleranceSlider"));
        toleranceSlider->setGeometry(QRect(110, 270, 121, 22));
        toleranceSlider->setMaximum(360);
        toleranceSlider->setValue(60);
        toleranceSlider->setOrientation(Qt::Horizontal);
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(7, 300, 91, 20));
        label_9->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(26, 270, 71, 20));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(17, 240, 81, 20));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        resolutionSlider = new QSlider(centralWidget);
        resolutionSlider->setObjectName(QStringLiteral("resolutionSlider"));
        resolutionSlider->setGeometry(QRect(110, 240, 121, 22));
        resolutionSlider->setMinimum(1);
        resolutionSlider->setMaximum(20);
        resolutionSlider->setValue(6);
        resolutionSlider->setTracking(true);
        resolutionSlider->setOrientation(Qt::Horizontal);
        resolutionSlider->setInvertedAppearance(true);
        resolutionSlider->setTickPosition(QSlider::TicksBelow);
        octreeDepthSlider = new QSlider(centralWidget);
        octreeDepthSlider->setObjectName(QStringLiteral("octreeDepthSlider"));
        octreeDepthSlider->setGeometry(QRect(110, 300, 121, 22));
        octreeDepthSlider->setMaximum(10);
        octreeDepthSlider->setValue(8);
        octreeDepthSlider->setOrientation(Qt::Horizontal);
        isoLevelSlider = new QSlider(centralWidget);
        isoLevelSlider->setObjectName(QStringLiteral("isoLevelSlider"));
        isoLevelSlider->setGeometry(QRect(110, 210, 121, 22));
        isoLevelSlider->setMaximum(255);
        isoLevelSlider->setValue(100);
        isoLevelSlider->setOrientation(Qt::Horizontal);
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(240, 210, 47, 13));
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(7, 330, 91, 20));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        interactiveButton = new QCheckBox(centralWidget);
        interactiveButton->setObjectName(QStringLiteral("interactiveButton"));
        interactiveButton->setGeometry(QRect(30, 170, 91, 20));
        interactiveButton->setLayoutDirection(Qt::RightToLeft);
        interactiveButton->setChecked(true);
        linearInterpolationSlider = new QSlider(centralWidget);
        linearInterpolationSlider->setObjectName(QStringLiteral("linearInterpolationSlider"));
        linearInterpolationSlider->setGeometry(QRect(109, 330, 121, 22));
        linearInterpolationSlider->setMinimum(0);
        linearInterpolationSlider->setMaximum(10);
        linearInterpolationSlider->setValue(10);
        linearInterpolationSlider->setOrientation(Qt::Horizontal);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(26, 210, 71, 20));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        generateMeshButton = new QPushButton(centralWidget);
        generateMeshButton->setObjectName(QStringLiteral("generateMeshButton"));
        generateMeshButton->setGeometry(QRect(150, 170, 81, 23));
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(240, 270, 31, 16));
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(240, 300, 47, 13));
        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(240, 330, 47, 13));
        borderYBottomSlider = new QSlider(centralWidget);
        borderYBottomSlider->setObjectName(QStringLiteral("borderYBottomSlider"));
        borderYBottomSlider->setGeometry(QRect(1070, 360, 22, 231));
        borderYBottomSlider->setMaximum(512);
        borderYBottomSlider->setValue(0);
        borderYBottomSlider->setOrientation(Qt::Vertical);
        borderYBottomSlider->setInvertedAppearance(false);
        borderYTopSlider = new QSlider(centralWidget);
        borderYTopSlider->setObjectName(QStringLiteral("borderYTopSlider"));
        borderYTopSlider->setGeometry(QRect(1050, 360, 20, 231));
        borderYTopSlider->setLayoutDirection(Qt::LeftToRight);
        borderYTopSlider->setMaximum(512);
        borderYTopSlider->setValue(0);
        borderYTopSlider->setOrientation(Qt::Vertical);
        borderYTopSlider->setInvertedAppearance(true);
        borderYTopSlider->setInvertedControls(false);
        borderXLeftSlider = new QSlider(centralWidget);
        borderXLeftSlider->setObjectName(QStringLiteral("borderXLeftSlider"));
        borderXLeftSlider->setGeometry(QRect(1100, 600, 231, 22));
        borderXLeftSlider->setMaximum(512);
        borderXLeftSlider->setOrientation(Qt::Horizontal);
        borderXRightSlider = new QSlider(centralWidget);
        borderXRightSlider->setObjectName(QStringLiteral("borderXRightSlider"));
        borderXRightSlider->setGeometry(QRect(1100, 620, 231, 22));
        borderXRightSlider->setMaximum(512);
        borderXRightSlider->setOrientation(Qt::Horizontal);
        borderXRightSlider->setInvertedAppearance(true);
        VolumeRendererClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(VolumeRendererClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1343, 21));
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
        QObject::connect(toleranceSlider, SIGNAL(valueChanged(int)), label_10, SLOT(setNum(int)));
        QObject::connect(octreeDepthSlider, SIGNAL(valueChanged(int)), label_11, SLOT(setNum(int)));
        QObject::connect(linearInterpolationSlider, SIGNAL(valueChanged(int)), label_12, SLOT(setNum(int)));

        QMetaObject::connectSlotsByName(VolumeRendererClass);
    } // setupUi

    void retranslateUi(QMainWindow *VolumeRendererClass)
    {
        VolumeRendererClass->setWindowTitle(QApplication::translate("VolumeRendererClass", "VolumeRenderer", 0));
        numberOfTrianglesLabel->setText(QApplication::translate("VolumeRendererClass", "<html><head/><body><p><br/></p></body></html>", 0));
        pushButton->setText(QApplication::translate("VolumeRendererClass", "writeToObj", 0));
        label_2->setText(QApplication::translate("VolumeRendererClass", "Frame", 0));
        label_6->setText(QApplication::translate("VolumeRendererClass", "0", 0));
        loadDICOMFromFile->setText(QApplication::translate("VolumeRendererClass", "Load DICOM file", 0));
        showWireframeButton->setText(QApplication::translate("VolumeRendererClass", "showWireframe", 0));
        perspectiveButton->setText(QApplication::translate("VolumeRendererClass", "Perspective", 0));
        groupBox_2->setTitle(QApplication::translate("VolumeRendererClass", "Algorithm", 0));
        marchingCubesButton->setText(QApplication::translate("VolumeRendererClass", "Marching Cubes", 0));
        adaptiveMarchingCubesButton->setText(QApplication::translate("VolumeRendererClass", "Adaptive Marching Cubes", 0));
        adaptiveMarchingCubes2Button->setText(QApplication::translate("VolumeRendererClass", "Adaptive Marching Cubes 2", 0));
        ballPivotButton->setText(QApplication::translate("VolumeRendererClass", "Ball Pivot", 0));
        adaptiveMarchingCubes3Button->setText(QApplication::translate("VolumeRendererClass", "Adaptive Marching Cubes 3", 0));
        groupBox_3->setTitle(QApplication::translate("VolumeRendererClass", "Normals", 0));
        normalsPerTriangleButton->setText(QApplication::translate("VolumeRendererClass", "Normals Per Triangle", 0));
        normalsPerVerticeButton->setText(QApplication::translate("VolumeRendererClass", "Normals Per Vertice ", 0));
        showCubesButton->setText(QApplication::translate("VolumeRendererClass", "showCubes", 0));
        pushButton_2->setText(QApplication::translate("VolumeRendererClass", "Load OBJ File", 0));
        pushButton_3->setText(QApplication::translate("VolumeRendererClass", "ResetModelPosition", 0));
        showMeshButton->setText(QApplication::translate("VolumeRendererClass", "showMesh", 0));
        showGradientButton->setText(QApplication::translate("VolumeRendererClass", "showGradient", 0));
        groupBox_4->setTitle(QApplication::translate("VolumeRendererClass", "Orientation", 0));
        orientationZButton->setText(QApplication::translate("VolumeRendererClass", "Z", 0));
        orientationXButton->setText(QApplication::translate("VolumeRendererClass", "X", 0));
        orientationYButton->setText(QApplication::translate("VolumeRendererClass", "Y", 0));
        label_4->setText(QApplication::translate("VolumeRendererClass", "6", 0));
        label_9->setText(QApplication::translate("VolumeRendererClass", "OctreeDepth", 0));
        label_8->setText(QApplication::translate("VolumeRendererClass", "Tolerance", 0));
        label_3->setText(QApplication::translate("VolumeRendererClass", "Resolution", 0));
        label_5->setText(QApplication::translate("VolumeRendererClass", "100", 0));
        label_7->setText(QApplication::translate("VolumeRendererClass", "Interpolation", 0));
        interactiveButton->setText(QApplication::translate("VolumeRendererClass", "Interactive", 0));
        label->setText(QApplication::translate("VolumeRendererClass", "IsoLevel", 0));
        generateMeshButton->setText(QApplication::translate("VolumeRendererClass", "Generate Mesh", 0));
        label_10->setText(QApplication::translate("VolumeRendererClass", "60", 0));
        label_11->setText(QApplication::translate("VolumeRendererClass", "8", 0));
        label_12->setText(QApplication::translate("VolumeRendererClass", "10", 0));
    } // retranslateUi

};

namespace Ui {
    class VolumeRendererClass: public Ui_VolumeRendererClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOLUMERENDERER_H
