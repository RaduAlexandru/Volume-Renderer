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
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *VolumeRendererClass)
    {
        if (VolumeRendererClass->objectName().isEmpty())
            VolumeRendererClass->setObjectName(QStringLiteral("VolumeRendererClass"));
        VolumeRendererClass->resize(890, 713);
        centralWidget = new QWidget(VolumeRendererClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        test = new QPushButton(centralWidget);
        test->setObjectName(QStringLiteral("test"));
        test->setGeometry(QRect(20, 50, 75, 23));
        glwidget = new GLWidget(centralWidget);
        glwidget->setObjectName(QStringLiteral("glwidget"));
        glwidget->setEnabled(true);
        glwidget->setGeometry(QRect(170, 10, 601, 491));
        frameSlider = new QSlider(centralWidget);
        frameSlider->setObjectName(QStringLiteral("frameSlider"));
        frameSlider->setGeometry(QRect(140, 610, 160, 22));
        frameSlider->setOrientation(Qt::Horizontal);
        addDICOMFiles = new QPushButton(centralWidget);
        addDICOMFiles->setObjectName(QStringLiteral("addDICOMFiles"));
        addDICOMFiles->setGeometry(QRect(20, 100, 91, 23));
        isoLevelSlider = new QSlider(centralWidget);
        isoLevelSlider->setObjectName(QStringLiteral("isoLevelSlider"));
        isoLevelSlider->setGeometry(QRect(140, 520, 160, 22));
        isoLevelSlider->setMaximum(255);
        isoLevelSlider->setValue(100);
        isoLevelSlider->setOrientation(Qt::Horizontal);
        loadTestImageButton = new QPushButton(centralWidget);
        loadTestImageButton->setObjectName(QStringLiteral("loadTestImageButton"));
        loadTestImageButton->setGeometry(QRect(180, 20, 81, 23));
        loadDICOMButton = new QPushButton(centralWidget);
        loadDICOMButton->setObjectName(QStringLiteral("loadDICOMButton"));
        loadDICOMButton->setGeometry(QRect(410, 20, 75, 23));
        loadTestImage2Button = new QPushButton(centralWidget);
        loadTestImage2Button->setObjectName(QStringLiteral("loadTestImage2Button"));
        loadTestImage2Button->setGeometry(QRect(290, 20, 91, 23));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(70, 520, 47, 13));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(70, 620, 47, 13));
        resolutionSlider = new QSlider(centralWidget);
        resolutionSlider->setObjectName(QStringLiteral("resolutionSlider"));
        resolutionSlider->setGeometry(QRect(140, 570, 160, 22));
        resolutionSlider->setMinimum(1);
        resolutionSlider->setMaximum(20);
        resolutionSlider->setValue(4);
        resolutionSlider->setOrientation(Qt::Horizontal);
        resolutionSlider->setInvertedAppearance(true);
        resolutionSlider->setTickPosition(QSlider::TicksBelow);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(60, 570, 51, 16));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(330, 570, 47, 13));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(330, 520, 47, 13));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(330, 610, 47, 13));
        linearInterpolationButton = new QRadioButton(centralWidget);
        linearInterpolationButton->setObjectName(QStringLiteral("linearInterpolationButton"));
        linearInterpolationButton->setGeometry(QRect(410, 550, 111, 17));
        linearInterpolationButton->setChecked(true);
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(400, 570, 141, 80));
        pointFlagButton = new QCheckBox(groupBox);
        pointFlagButton->setObjectName(QStringLiteral("pointFlagButton"));
        pointFlagButton->setGeometry(QRect(10, 20, 111, 17));
        borderFlagButton = new QCheckBox(groupBox);
        borderFlagButton->setObjectName(QStringLiteral("borderFlagButton"));
        borderFlagButton->setGeometry(QRect(10, 50, 111, 17));
        loadMFDICOMButton = new QPushButton(centralWidget);
        loadMFDICOMButton->setObjectName(QStringLiteral("loadMFDICOMButton"));
        loadMFDICOMButton->setGeometry(QRect(510, 20, 121, 23));
        dicomviewer2dgl = new DicomViewer2DGL(centralWidget);
        dicomviewer2dgl->setObjectName(QStringLiteral("dicomviewer2dgl"));
        dicomviewer2dgl->setGeometry(QRect(650, 510, 151, 131));
        toleranceSlider = new QSlider(centralWidget);
        toleranceSlider->setObjectName(QStringLiteral("toleranceSlider"));
        toleranceSlider->setGeometry(QRect(460, 510, 160, 22));
        toleranceSlider->setMaximum(360);
        toleranceSlider->setOrientation(Qt::Horizontal);
        loadDICOMFromFile = new QPushButton(centralWidget);
        loadDICOMFromFile->setObjectName(QStringLiteral("loadDICOMFromFile"));
        loadDICOMFromFile->setGeometry(QRect(640, 20, 111, 23));
        showWireframeButton = new QCheckBox(centralWidget);
        showWireframeButton->setObjectName(QStringLiteral("showWireframeButton"));
        showWireframeButton->setGeometry(QRect(30, 260, 91, 17));
        VolumeRendererClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(VolumeRendererClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 890, 21));
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
        addDICOMFiles->setText(QApplication::translate("VolumeRendererClass", "AddDICOMFiles", 0));
        loadTestImageButton->setText(QApplication::translate("VolumeRendererClass", "loadTestImage", 0));
        loadDICOMButton->setText(QApplication::translate("VolumeRendererClass", "loadDICOM", 0));
        loadTestImage2Button->setText(QApplication::translate("VolumeRendererClass", "loadTestImage2", 0));
        label->setText(QApplication::translate("VolumeRendererClass", "IsoLevel", 0));
        label_2->setText(QApplication::translate("VolumeRendererClass", "Frame", 0));
        label_3->setText(QApplication::translate("VolumeRendererClass", "Resolution", 0));
        label_4->setText(QApplication::translate("VolumeRendererClass", "4", 0));
        label_5->setText(QApplication::translate("VolumeRendererClass", "100", 0));
        label_6->setText(QApplication::translate("VolumeRendererClass", "0", 0));
        linearInterpolationButton->setText(QApplication::translate("VolumeRendererClass", "linearInterpolation", 0));
        groupBox->setTitle(QApplication::translate("VolumeRendererClass", "PointRepresentation", 0));
        pointFlagButton->setText(QApplication::translate("VolumeRendererClass", "showPointCloud", 0));
        borderFlagButton->setText(QApplication::translate("VolumeRendererClass", "showPointInBorder", 0));
        loadMFDICOMButton->setText(QApplication::translate("VolumeRendererClass", "loadMultiFrameDicom", 0));
        loadDICOMFromFile->setText(QApplication::translate("VolumeRendererClass", "loadDICOMFromFile", 0));
        showWireframeButton->setText(QApplication::translate("VolumeRendererClass", "showWireframe", 0));
    } // retranslateUi

};

namespace Ui {
    class VolumeRendererClass: public Ui_VolumeRendererClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VOLUMERENDERER_H
