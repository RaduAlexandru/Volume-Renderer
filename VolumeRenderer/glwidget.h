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
	void keyPressEvent(QKeyEvent *keyEvent);
	void keyReleaseEvent(QKeyEvent *keyEvent);
	void sendDataToGL();
	void drawMesh();
	void drawCubes();
	void drawCubes2();
	void drawBackground();
	void readBackgroundImage();
	void setMatrices();
	void displayGradient();

	int dataSended;
	unsigned char* background;
	int backgroundWidth;
	int backgroundHeight;

	Model* model;

	double xRot;
	double yRot;
	double mouseSpeed = 0.5;
	double xMove, yMove, zMove;


private:
	int frame_to_display;
	QTimer timer;
	double angle;

	double mouseXPosEntered;
	double mouseYPosEntered;

	bool shiftPressed;
	bool ctrlPressed;
	
/*private slots:
	void rotate();*/
	
public slots:
	void setFrame(int frame);
	
};

#endif // GLWIDGET_H
