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
	void drawMesh(Mesh* mesh);
	void drawBackground();
	void readBackgroundImage();
	void setMatrices();
	void displayGradient();

	unsigned char* background;
	int backgroundWidth;
	int backgroundHeight;


	Mesh* mesh; //!< El mallado principal a mostrar en 3D
	Mesh* mesh2;//!< El mallado secundario a mostrar (el que se usa para hacer un pin)
	std::vector< boost::unordered_map< std::pair<int, int>, glm::vec3> >* gradient; //!< Puntero a los gradientes, para mostrarlos mapeados a RGB

	double xRot;	//!< El valor del giro de la figura en el eje X
	double yRot;	//!< El valor del giro de la figura en el eje Y
	double mouseSpeed = 0.5;	//!<  Indica la velocidad de giro o movimiento de la figura cuando el raton se arrastra por el visor 3D. A mas valor, mas rapido girara, o se movera la figura
	double xMove, yMove, zMove;	//!< Lo que se ha movido la figura en el eje X, Y, Z
	float opacity;	//!<La opacidad de la figura principal. La de la secundario (la del pin) no se puede cambiar

	
	bool generatingMesh;	/*!< Indica que en este momento se esta generando un mallado asi que no va a representar nada por pantalla para no provocar colisiones al recorrer el vector sobre el que se esta escribiendo en otro hilo. Los algoritmos de reconstruccion emitiran una señal para poner este valor a true, y despues de crear el mallado lo volveran a poner a false para dejar a la vista 3D leer los puntos del mesh */
	
	bool movingFigure;	//!<Indica que los movimientos del raton sobre la vista van a realizar un movimiento de la figura
	bool scalingFigure;	//!<Indica que los movimientos del raton sobre la vista van a realizar un escalado de la figura

private:

	double mouseXPosEntered;	//!<La posiciones dentro de la ventana en la que se ha hecho click. Se usa para saber en que dirrecion se ha movido el raton y por lo tanto donde tiene que girar la figura
	double mouseYPosEntered;	//!<La posiciones dentro de la en la que se ha empezado a arrastrar. Se usa para saber en que dirrecion hay que girar , mover o escalar la figura

	bool showPerspective;	//!<Indica si hay que mostrar el entorno 3D en perspectiva (true) o ortografia (false)
	bool showMesh;			//!<Indica si hay que mostrar los mallados
	bool showWireframe;		//!<Indica si hay que mostrar el wireframe, es decir los bordes entre los poligonos del mallado
	bool showCubes;			//!<Indica si hay que mostrar los cubos del octree
	bool showGradients;		//!<Indica si hay que mostrar los gradientes
	
	
public slots:
	void dataFinishedReadingSlot();
	void generatingFinishedSlot();
	void generatingStartedSlot();
	
	void opacityChangedSlot(int value);



	void showPerspectiveSlot(bool value);
	void showMeshSlot(bool value);
	void showWireframeSlot(bool value);
	void showCubesSlot(bool value);
	void showGradientSlot(bool value);
	void resetFigureSlot();
};

#endif // GLWIDGET_H
