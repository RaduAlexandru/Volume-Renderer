#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QTimer>
//#include "volumerenderer.h"
#include "Model.h"
#include <QOpenGLFunctions_4_3_Core>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
	Q_OBJECT

	

public:
	

	GLWidget(QWidget *parent);
	~GLWidget();
	void initializeGL();
	void paintGL();
	void resizeGL(int w, int h);
	void mouseMoveEvent(QMouseEvent * event);
	void mousePressEvent(QMouseEvent *event);
	void sendDataToGL();

	int dataSended;

	Model* model;

	


private:
	int frame_to_display;
	QTimer timer;
	double angle;

	double mouseXPosEntered;
	double mouseYPosEntered;

	double xRot;
	double yRot;
	double mouseSpeed = 0.5;
	
/*private slots:
	void rotate();*/
	
public slots:
	void setFrame(int frame);
	
};

#endif // GLWIDGET_H
