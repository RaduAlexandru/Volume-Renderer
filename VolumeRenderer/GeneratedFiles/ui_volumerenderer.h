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
    QPushButton *saveToSecondaryButton;
    QGroupBox *groupBox_3;
    QRadioButton *normalsPerTriangleButton;
    QRadioButton *normalsPerVerticeButton;
    QPushButton *writeObjButton;
    QGroupBox *groupBox_2;
    QRadioButton *marchingCubesButton;
    QRadioButton *adaptiveMarchingCubes2Button;
    QRadioButton *adaptiveMarchingCubes3Button;
    QLabel *label_13;
    QSlider *linearInterpolationSlider_2;
    QLabel *label_14;
    QPushButton *clearPinButton;
    QPushButton *moveButton;
    QPushButton *scaleButton;
    QSlider *borderXRightSlider;
    QSlider *frameSlider;
    QSlider *borderXLeftSlider;
    QSlider *borderYTopSlider;
    QGroupBox *groupBox_4;
    QRadioButton *orientationZButton;
    QRadioButton *orientationXButton;
    QRadioButton *orientationYButton;
    QLabel *label_2;
    QSlider *borderYBottomSlider;
    QLabel *label_6;
    DicomViewer2DGL *dicomviewer2dgl;
    QLabel *label_11;
    QSlider *toleranceSlider;
    QLabel *label_4;
    QLabel *label_8;
    QLabel *label_7;
    QLabel *label_3;
    QLabel *label;
    QPushButton *loadObjButton;
    QSlider *octreeDepthSlider;
    QLabel *label_5;
    QPushButton *loadDICOMFromFile;
    QPushButton *generateMeshButton;
    QLabel *label_10;
    QSlider *isoLevelSlider;
    QLabel *label_12;
    QSlider *resolutionSlider;
    QLabel *label_9;
    QSlider *linearInterpolationSlider;
    QProgressBar *progressBar;
    QLabel *progressText;
    QCheckBox *showGradientButton;
    QCheckBox *showWireframeButton;
    QCheckBox *showMeshButton;
    QPushButton *pushButton_3;
    QCheckBox *showCubesButton;
    QCheckBox *perspectiveButton;
    QPushButton *interactiveButton;
    QLabel *timeLabel;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VolumeRendererClass)
    {
        if (VolumeRendererClass->objectName().isEmpty())
            VolumeRendererClass->setObjectName(QStringLiteral("VolumeRendererClass"));
        VolumeRendererClass->resize(1380, 725);
        centralWidget = new QWidget(VolumeRendererClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        glwidget = new GLWidget(centralWidget);
        glwidget->setObjectName(QStringLiteral("glwidget"));
        glwidget->setEnabled(true);
        glwidget->setGeometry(QRect(0, -10, 1381, 701));
        numberOfTrianglesLabel = new QLabel(glwidget);
        numberOfTrianglesLabel->setObjectName(QStringLiteral("numberOfTrianglesLabel"));
        numberOfTrianglesLabel->setGeometry(QRect(40, 620, 181, 20));
        numberOfTrianglesLabel->setTextFormat(Qt::RichText);
        numberOfTrianglesLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        saveToSecondaryButton = new QPushButton(glwidget);
        saveToSecondaryButton->setObjectName(QStringLiteral("saveToSecondaryButton"));
        saveToSecondaryButton->setGeometry(QRect(40, 550, 81, 23));
        saveToSecondaryButton->setCheckable(false);
        groupBox_3 = new QGroupBox(glwidget);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(40, 430, 191, 80));
        normalsPerTriangleButton = new QRadioButton(groupBox_3);
        normalsPerTriangleButton->setObjectName(QStringLiteral("normalsPerTriangleButton"));
        normalsPerTriangleButton->setGeometry(QRect(10, 20, 131, 17));
        normalsPerTriangleButton->setChecked(true);
        normalsPerVerticeButton = new QRadioButton(groupBox_3);
        normalsPerVerticeButton->setObjectName(QStringLiteral("normalsPerVerticeButton"));
        normalsPerVerticeButton->setGeometry(QRect(10, 40, 131, 17));
        writeObjButton = new QPushButton(glwidget);
        writeObjButton->setObjectName(QStringLiteral("writeObjButton"));
        writeObjButton->setGeometry(QRect(160, 550, 75, 23));
        groupBox_2 = new QGroupBox(glwidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(40, 330, 191, 91));
        marchingCubesButton = new QRadioButton(groupBox_2);
        marchingCubesButton->setObjectName(QStringLiteral("marchingCubesButton"));
        marchingCubesButton->setGeometry(QRect(10, 20, 111, 17));
        marchingCubesButton->setChecked(true);
        adaptiveMarchingCubes2Button = new QRadioButton(groupBox_2);
        adaptiveMarchingCubes2Button->setObjectName(QStringLiteral("adaptiveMarchingCubes2Button"));
        adaptiveMarchingCubes2Button->setGeometry(QRect(10, 40, 161, 17));
        adaptiveMarchingCubes3Button = new QRadioButton(groupBox_2);
        adaptiveMarchingCubes3Button->setObjectName(QStringLiteral("adaptiveMarchingCubes3Button"));
        adaptiveMarchingCubes3Button->setGeometry(QRect(10, 60, 161, 17));
        label_13 = new QLabel(glwidget);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setGeometry(QRect(241, 300, 47, 13));
        linearInterpolationSlider_2 = new QSlider(glwidget);
        linearInterpolationSlider_2->setObjectName(QStringLiteral("linearInterpolationSlider_2"));
        linearInterpolationSlider_2->setGeometry(QRect(140, 300, 91, 22));
        linearInterpolationSlider_2->setMinimum(0);
        linearInterpolationSlider_2->setMaximum(100);
        linearInterpolationSlider_2->setValue(100);
        linearInterpolationSlider_2->setOrientation(Qt::Horizontal);
        label_14 = new QLabel(glwidget);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setGeometry(QRect(40, 300, 81, 20));
        label_14->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        clearPinButton = new QPushButton(glwidget);
        clearPinButton->setObjectName(QStringLiteral("clearPinButton"));
        clearPinButton->setGeometry(QRect(40, 580, 81, 23));
        moveButton = new QPushButton(glwidget);
        moveButton->setObjectName(QStringLiteral("moveButton"));
        moveButton->setGeometry(QRect(840, 570, 41, 41));
        moveButton->setCheckable(true);
        moveButton->setChecked(false);
        moveButton->setFlat(false);
        scaleButton = new QPushButton(glwidget);
        scaleButton->setObjectName(QStringLiteral("scaleButton"));
        scaleButton->setGeometry(QRect(910, 570, 41, 41));
        scaleButton->setCheckable(true);
        borderXRightSlider = new QSlider(glwidget);
        borderXRightSlider->setObjectName(QStringLiteral("borderXRightSlider"));
        borderXRightSlider->setGeometry(QRect(1100, 630, 231, 22));
        borderXRightSlider->setMaximum(512);
        borderXRightSlider->setOrientation(Qt::Horizontal);
        borderXRightSlider->setInvertedAppearance(true);
        frameSlider = new QSlider(glwidget);
        frameSlider->setObjectName(QStringLiteral("frameSlider"));
        frameSlider->setGeometry(QRect(1170, 310, 121, 22));
        frameSlider->setOrientation(Qt::Horizontal);
        borderXLeftSlider = new QSlider(glwidget);
        borderXLeftSlider->setObjectName(QStringLiteral("borderXLeftSlider"));
        borderXLeftSlider->setGeometry(QRect(1100, 610, 231, 22));
        borderXLeftSlider->setMaximum(512);
        borderXLeftSlider->setOrientation(Qt::Horizontal);
        borderYTopSlider = new QSlider(glwidget);
        borderYTopSlider->setObjectName(QStringLiteral("borderYTopSlider"));
        borderYTopSlider->setGeometry(QRect(1050, 370, 20, 231));
        borderYTopSlider->setLayoutDirection(Qt::LeftToRight);
        borderYTopSlider->setMaximum(512);
        borderYTopSlider->setValue(0);
        borderYTopSlider->setOrientation(Qt::Vertical);
        borderYTopSlider->setInvertedAppearance(true);
        borderYTopSlider->setInvertedControls(false);
        groupBox_4 = new QGroupBox(glwidget);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        groupBox_4->setGeometry(QRect(1100, 330, 231, 41));
        orientationZButton = new QRadioButton(groupBox_4);
        orientationZButton->setObjectName(QStringLiteral("orientationZButton"));
        orientationZButton->setGeometry(QRect(90, 20, 31, 17));
        orientationZButton->setChecked(true);
        orientationXButton = new QRadioButton(groupBox_4);
        orientationXButton->setObjectName(QStringLiteral("orientationXButton"));
        orientationXButton->setGeometry(QRect(10, 20, 31, 17));
        orientationYButton = new QRadioButton(groupBox_4);
        orientationYButton->setObjectName(QStringLiteral("orientationYButton"));
        orientationYButton->setGeometry(QRect(50, 20, 31, 17));
        label_2 = new QLabel(glwidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(1120, 310, 47, 13));
        borderYBottomSlider = new QSlider(glwidget);
        borderYBottomSlider->setObjectName(QStringLiteral("borderYBottomSlider"));
        borderYBottomSlider->setGeometry(QRect(1070, 370, 22, 231));
        borderYBottomSlider->setMaximum(512);
        borderYBottomSlider->setValue(0);
        borderYBottomSlider->setOrientation(Qt::Vertical);
        borderYBottomSlider->setInvertedAppearance(false);
        label_6 = new QLabel(glwidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(1300, 310, 47, 13));
        dicomviewer2dgl = new DicomViewer2DGL(glwidget);
        dicomviewer2dgl->setObjectName(QStringLiteral("dicomviewer2dgl"));
        dicomviewer2dgl->setGeometry(QRect(1100, 370, 231, 231));
        label_11 = new QLabel(glwidget);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(241, 240, 47, 13));
        toleranceSlider = new QSlider(glwidget);
        toleranceSlider->setObjectName(QStringLiteral("toleranceSlider"));
        toleranceSlider->setGeometry(QRect(141, 210, 91, 22));
        toleranceSlider->setMaximum(360);
        toleranceSlider->setValue(60);
        toleranceSlider->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(glwidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(241, 180, 31, 16));
        label_8 = new QLabel(glwidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(40, 210, 91, 20));
        label_8->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_7 = new QLabel(glwidget);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(40, 270, 91, 20));
        label_7->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label_3 = new QLabel(glwidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(40, 180, 81, 20));
        label_3->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        label = new QLabel(glwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 150, 51, 20));
        label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        loadObjButton = new QPushButton(glwidget);
        loadObjButton->setObjectName(QStringLiteral("loadObjButton"));
        loadObjButton->setGeometry(QRect(141, 50, 91, 23));
        octreeDepthSlider = new QSlider(glwidget);
        octreeDepthSlider->setObjectName(QStringLiteral("octreeDepthSlider"));
        octreeDepthSlider->setGeometry(QRect(141, 240, 91, 22));
        octreeDepthSlider->setMaximum(10);
        octreeDepthSlider->setValue(8);
        octreeDepthSlider->setOrientation(Qt::Horizontal);
        label_5 = new QLabel(glwidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(241, 150, 47, 13));
        loadDICOMFromFile = new QPushButton(glwidget);
        loadDICOMFromFile->setObjectName(QStringLiteral("loadDICOMFromFile"));
        loadDICOMFromFile->setGeometry(QRect(31, 50, 91, 23));
        generateMeshButton = new QPushButton(glwidget);
        generateMeshButton->setObjectName(QStringLiteral("generateMeshButton"));
        generateMeshButton->setGeometry(QRect(141, 110, 91, 23));
        label_10 = new QLabel(glwidget);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(241, 210, 31, 16));
        isoLevelSlider = new QSlider(glwidget);
        isoLevelSlider->setObjectName(QStringLiteral("isoLevelSlider"));
        isoLevelSlider->setGeometry(QRect(141, 150, 91, 22));
        isoLevelSlider->setMaximum(255);
        isoLevelSlider->setValue(100);
        isoLevelSlider->setOrientation(Qt::Horizontal);
        label_12 = new QLabel(glwidget);
        label_12->setObjectName(QStringLiteral("label_12"));
        label_12->setGeometry(QRect(241, 270, 47, 13));
        resolutionSlider = new QSlider(glwidget);
        resolutionSlider->setObjectName(QStringLiteral("resolutionSlider"));
        resolutionSlider->setGeometry(QRect(141, 180, 91, 22));
        resolutionSlider->setMinimum(1);
        resolutionSlider->setMaximum(20);
        resolutionSlider->setValue(6);
        resolutionSlider->setTracking(true);
        resolutionSlider->setOrientation(Qt::Horizontal);
        resolutionSlider->setInvertedAppearance(true);
        resolutionSlider->setTickPosition(QSlider::TicksBelow);
        label_9 = new QLabel(glwidget);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(40, 240, 91, 20));
        label_9->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        linearInterpolationSlider = new QSlider(glwidget);
        linearInterpolationSlider->setObjectName(QStringLiteral("linearInterpolationSlider"));
        linearInterpolationSlider->setGeometry(QRect(140, 270, 91, 22));
        linearInterpolationSlider->setMinimum(0);
        linearInterpolationSlider->setMaximum(10);
        linearInterpolationSlider->setValue(10);
        linearInterpolationSlider->setOrientation(Qt::Horizontal);
        progressBar = new QProgressBar(glwidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(0, 10, 1381, 20));
        progressBar->setValue(0);
        progressBar->setTextVisible(false);
        progressText = new QLabel(glwidget);
        progressText->setObjectName(QStringLiteral("progressText"));
        progressText->setGeometry(QRect(600, 30, 161, 20));
        progressText->setAlignment(Qt::AlignCenter);
        showGradientButton = new QCheckBox(glwidget);
        showGradientButton->setObjectName(QStringLiteral("showGradientButton"));
        showGradientButton->setGeometry(QRect(1180, 230, 91, 17));
        showWireframeButton = new QCheckBox(glwidget);
        showWireframeButton->setObjectName(QStringLiteral("showWireframeButton"));
        showWireframeButton->setGeometry(QRect(1180, 170, 101, 17));
        showMeshButton = new QCheckBox(glwidget);
        showMeshButton->setObjectName(QStringLiteral("showMeshButton"));
        showMeshButton->setGeometry(QRect(1180, 140, 70, 17));
        showMeshButton->setChecked(true);
        pushButton_3 = new QPushButton(glwidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setGeometry(QRect(1180, 70, 111, 23));
        showCubesButton = new QCheckBox(glwidget);
        showCubesButton->setObjectName(QStringLiteral("showCubesButton"));
        showCubesButton->setGeometry(QRect(1180, 200, 81, 17));
        perspectiveButton = new QCheckBox(glwidget);
        perspectiveButton->setObjectName(QStringLiteral("perspectiveButton"));
        perspectiveButton->setGeometry(QRect(1180, 110, 81, 17));
        perspectiveButton->setChecked(true);
        interactiveButton = new QPushButton(glwidget);
        interactiveButton->setObjectName(QStringLiteral("interactiveButton"));
        interactiveButton->setGeometry(QRect(34, 110, 91, 23));
        interactiveButton->setCheckable(true);
        interactiveButton->setChecked(true);
        timeLabel = new QLabel(glwidget);
        timeLabel->setObjectName(QStringLiteral("timeLabel"));
        timeLabel->setGeometry(QRect(40, 640, 181, 21));
        numberOfTrianglesLabel->raise();
        saveToSecondaryButton->raise();
        groupBox_3->raise();
        writeObjButton->raise();
        groupBox_2->raise();
        label_13->raise();
        linearInterpolationSlider_2->raise();
        label_14->raise();
        clearPinButton->raise();
        moveButton->raise();
        scaleButton->raise();
        toleranceSlider->raise();
        label_4->raise();
        label_8->raise();
        label_7->raise();
        label_3->raise();
        label->raise();
        label_11->raise();
        loadObjButton->raise();
        octreeDepthSlider->raise();
        label_5->raise();
        loadDICOMFromFile->raise();
        linearInterpolationSlider->raise();
        generateMeshButton->raise();
        label_10->raise();
        isoLevelSlider->raise();
        label_12->raise();
        resolutionSlider->raise();
        label_9->raise();
        progressText->raise();
        progressBar->raise();
        borderXRightSlider->raise();
        frameSlider->raise();
        borderXLeftSlider->raise();
        borderYTopSlider->raise();
        groupBox_4->raise();
        label_2->raise();
        borderYBottomSlider->raise();
        label_6->raise();
        dicomviewer2dgl->raise();
        dicomviewer2dgl->raise();
        borderXRightSlider->raise();
        frameSlider->raise();
        borderXLeftSlider->raise();
        borderYTopSlider->raise();
        groupBox_4->raise();
        label_2->raise();
        borderYBottomSlider->raise();
        label_6->raise();
        label_6->raise();
        borderXRightSlider->raise();
        frameSlider->raise();
        borderXLeftSlider->raise();
        borderYTopSlider->raise();
        groupBox_4->raise();
        label_2->raise();
        borderYBottomSlider->raise();
        dicomviewer2dgl->raise();
        dicomviewer2dgl->raise();
        borderXRightSlider->raise();
        frameSlider->raise();
        borderXLeftSlider->raise();
        borderYTopSlider->raise();
        groupBox_4->raise();
        label_2->raise();
        borderYBottomSlider->raise();
        label_6->raise();
        label_6->raise();
        borderXRightSlider->raise();
        frameSlider->raise();
        borderXLeftSlider->raise();
        borderYTopSlider->raise();
        groupBox_4->raise();
        label_2->raise();
        borderYBottomSlider->raise();
        dicomviewer2dgl->raise();
        dicomviewer2dgl->raise();
        borderXRightSlider->raise();
        frameSlider->raise();
        borderXLeftSlider->raise();
        borderYTopSlider->raise();
        groupBox_4->raise();
        label_2->raise();
        borderYBottomSlider->raise();
        label_6->raise();
        label_6->raise();
        borderXRightSlider->raise();
        frameSlider->raise();
        borderXLeftSlider->raise();
        borderYTopSlider->raise();
        label_2->raise();
        groupBox_4->raise();
        borderYBottomSlider->raise();
        dicomviewer2dgl->raise();
        borderXRightSlider->raise();
        frameSlider->raise();
        borderXLeftSlider->raise();
        borderYTopSlider->raise();
        groupBox_4->raise();
        label_2->raise();
        borderYBottomSlider->raise();
        label_6->raise();
        dicomviewer2dgl->raise();
        borderXRightSlider->raise();
        frameSlider->raise();
        borderXLeftSlider->raise();
        borderYTopSlider->raise();
        groupBox_4->raise();
        label_2->raise();
        borderYBottomSlider->raise();
        label_6->raise();
        dicomviewer2dgl->raise();
        label_11->raise();
        toleranceSlider->raise();
        label_4->raise();
        label_8->raise();
        label_7->raise();
        label_3->raise();
        label->raise();
        loadObjButton->raise();
        octreeDepthSlider->raise();
        label_5->raise();
        loadDICOMFromFile->raise();
        generateMeshButton->raise();
        label_10->raise();
        isoLevelSlider->raise();
        label_12->raise();
        resolutionSlider->raise();
        label_9->raise();
        linearInterpolationSlider->raise();
        progressBar->raise();
        progressText->raise();
        showGradientButton->raise();
        showWireframeButton->raise();
        showMeshButton->raise();
        pushButton_3->raise();
        showCubesButton->raise();
        perspectiveButton->raise();
        interactiveButton->raise();
        timeLabel->raise();
        VolumeRendererClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(VolumeRendererClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1380, 21));
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
        QObject::connect(toleranceSlider, SIGNAL(valueChanged(int)), label_10, SLOT(setNum(int)));
        QObject::connect(octreeDepthSlider, SIGNAL(valueChanged(int)), label_11, SLOT(setNum(int)));
        QObject::connect(linearInterpolationSlider, SIGNAL(valueChanged(int)), label_12, SLOT(setNum(int)));
        QObject::connect(linearInterpolationSlider_2, SIGNAL(valueChanged(int)), label_13, SLOT(setNum(int)));
        QObject::connect(linearInterpolationSlider_2, SIGNAL(valueChanged(int)), glwidget, SLOT(opacityChangedSlot(int)));
        QObject::connect(showWireframeButton, SIGNAL(clicked(bool)), glwidget, SLOT(showWireframeSlot(bool)));
        QObject::connect(perspectiveButton, SIGNAL(clicked(bool)), glwidget, SLOT(showPerspectiveSlot(bool)));
        QObject::connect(showMeshButton, SIGNAL(clicked(bool)), glwidget, SLOT(showMeshSlot(bool)));
        QObject::connect(showCubesButton, SIGNAL(clicked(bool)), glwidget, SLOT(showCubesSlot(bool)));
        QObject::connect(showGradientButton, SIGNAL(clicked(bool)), glwidget, SLOT(showGradientSlot(bool)));
        QObject::connect(pushButton_3, SIGNAL(clicked()), glwidget, SLOT(resetFigureSlot()));
        QObject::connect(frameSlider, SIGNAL(valueChanged(int)), dicomviewer2dgl, SLOT(setFrame(int)));

        QMetaObject::connectSlotsByName(VolumeRendererClass);
    } // setupUi

    void retranslateUi(QMainWindow *VolumeRendererClass)
    {
        VolumeRendererClass->setWindowTitle(QApplication::translate("VolumeRendererClass", "VolumeRenderer", 0));
        numberOfTrianglesLabel->setText(QApplication::translate("VolumeRendererClass", "<html><head/><body><p><br/></p></body></html>", 0));
        saveToSecondaryButton->setText(QApplication::translate("VolumeRendererClass", "Pin Mesh", 0));
        groupBox_3->setTitle(QApplication::translate("VolumeRendererClass", "Normals", 0));
        normalsPerTriangleButton->setText(QApplication::translate("VolumeRendererClass", "Normals Per Triangle", 0));
        normalsPerVerticeButton->setText(QApplication::translate("VolumeRendererClass", "Normals Per Vertice ", 0));
        writeObjButton->setText(QApplication::translate("VolumeRendererClass", "Export OBJ", 0));
        groupBox_2->setTitle(QApplication::translate("VolumeRendererClass", "Algorithm", 0));
        marchingCubesButton->setText(QApplication::translate("VolumeRendererClass", "Marching Cubes", 0));
        adaptiveMarchingCubes2Button->setText(QApplication::translate("VolumeRendererClass", "Adaptive Marching Cubes", 0));
        adaptiveMarchingCubes3Button->setText(QApplication::translate("VolumeRendererClass", "Adaptive Marching Cubes 2", 0));
        label_13->setText(QApplication::translate("VolumeRendererClass", "100", 0));
        label_14->setText(QApplication::translate("VolumeRendererClass", "Opacity", 0));
        clearPinButton->setText(QApplication::translate("VolumeRendererClass", "Clear Pin", 0));
#ifndef QT_NO_TOOLTIP
        moveButton->setToolTip(QApplication::translate("VolumeRendererClass", "Left Shift", 0));
#endif // QT_NO_TOOLTIP
        moveButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        scaleButton->setToolTip(QApplication::translate("VolumeRendererClass", "Left Ctrl", 0));
#endif // QT_NO_TOOLTIP
        scaleButton->setText(QString());
        groupBox_4->setTitle(QApplication::translate("VolumeRendererClass", "Orientation", 0));
        orientationZButton->setText(QApplication::translate("VolumeRendererClass", "Z", 0));
        orientationXButton->setText(QApplication::translate("VolumeRendererClass", "X", 0));
        orientationYButton->setText(QApplication::translate("VolumeRendererClass", "Y", 0));
        label_2->setText(QApplication::translate("VolumeRendererClass", "Frame", 0));
        label_6->setText(QApplication::translate("VolumeRendererClass", "0", 0));
        label_11->setText(QApplication::translate("VolumeRendererClass", "8", 0));
        label_4->setText(QApplication::translate("VolumeRendererClass", "6", 0));
        label_8->setText(QApplication::translate("VolumeRendererClass", "Tolerance", 0));
        label_7->setText(QApplication::translate("VolumeRendererClass", "Interpolation", 0));
        label_3->setText(QApplication::translate("VolumeRendererClass", "Resolution", 0));
        label->setText(QApplication::translate("VolumeRendererClass", "IsoLevel", 0));
        loadObjButton->setText(QApplication::translate("VolumeRendererClass", "Load OBJ File", 0));
        label_5->setText(QApplication::translate("VolumeRendererClass", "14000", 0));
        loadDICOMFromFile->setText(QApplication::translate("VolumeRendererClass", "Load DICOM file", 0));
        generateMeshButton->setText(QApplication::translate("VolumeRendererClass", "Generate Mesh", 0));
        label_10->setText(QApplication::translate("VolumeRendererClass", "60", 0));
        label_12->setText(QApplication::translate("VolumeRendererClass", "10", 0));
        label_9->setText(QApplication::translate("VolumeRendererClass", "OctreeDepth", 0));
        progressBar->setFormat(QApplication::translate("VolumeRendererClass", "%p%", 0));
        progressText->setText(QApplication::translate("VolumeRendererClass", "ProgressText", 0));
        showGradientButton->setText(QApplication::translate("VolumeRendererClass", "showGradient", 0));
        showWireframeButton->setText(QApplication::translate("VolumeRendererClass", "showWireframe", 0));
        showMeshButton->setText(QApplication::translate("VolumeRendererClass", "showMesh", 0));
        pushButton_3->setText(QApplication::translate("VolumeRendererClass", "ResetModelPosition", 0));
        showCubesButton->setText(QApplication::translate("VolumeRendererClass", "showCubes", 0));
        perspectiveButton->setText(QApplication::translate("VolumeRendererClass", "Perspective", 0));
        interactiveButton->setText(QApplication::translate("VolumeRendererClass", "Interactive", 0));
        timeLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class VolumeRendererClass: public Ui_VolumeRendererClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOLUMERENDERER_H
