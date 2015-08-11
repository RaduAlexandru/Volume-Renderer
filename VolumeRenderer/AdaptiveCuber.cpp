#include "AdaptiveCuber.h"
#include "OctreeCube.h"
#include <math.h>
#include <queue>
#include <boost/dynamic_bitset.hpp>
#define NOMINMAX

#define PI 3.14159265
using namespace std;


#define RIGHT 1
#define LEFT 2
#define UP 3
#define DOWN 4
#define FURTHER 5
#define CLOSER 6


AdaptiveCuber::AdaptiveCuber()
{
}


AdaptiveCuber::~AdaptiveCuber()
{
}

AdaptiveCuber::AdaptiveCuber(PixelData* pixelData, Mesh* mesh, int isoLevel, int cellSizeX, int cellSizeY, int cellSizeZ,  int interpolateDepth, int octreeMaxDepth, std::vector< boost::unordered_map< std::pair<int, int>, glm::vec3> >* gradient, int tolerance):
MarchingCuber(pixelData,  mesh,  isoLevel,  cellSizeX,  cellSizeY,  cellSizeZ,  interpolateDepth) {

	this->octreeMaxDepth = octreeMaxDepth;
	this->gradient = gradient;
	this->tolerance = tolerance;

}

/*! \brief Ejecuta el agoritmo de AMC
*/
int AdaptiveCuber::run(){
	mesh->verts.clear();
	mesh->normals.clear();
	octreeVector.clear();
	std::cout << "Starting adaptive marching cubes 3" << std::endl;
	//Calculate the gradients
	//Create the original cube
	/*
	7---------6
	size Z
	/			  !
	3---------2	  5
	!
	size Y
	!
	origin----size X
	*/

	emit progressTextSignal("Generating Gradients");
	int cubesSize = 0, depth = 0, maxDepth = octreeMaxDepth;

	if (gradient->size() == 0 || gradient->empty())
		calculateGradient();
	cout << "Finished calculating gradients" << endl;


	OctreeCube* initial = new OctreeCube;
	

	initial = createInitialCube();
	octreeVector.push_back(initial);

	emit progressTextSignal("Generating Octree");
	std::cout << "starting to generate octree" << std::endl;
	generateOctree_tree_version(*initial);
	std::cout << "finished generateing octree" << std::endl;


	//Grab each octreebube in our octree and polygonise it

	emit progressTextSignal("Polygonise Octree");
	polygoniseOctree2(initial);	//Polygonise octree will polygonise each octree but assign the points to the cube itself an not to the verts in the mesh

	emit progressTextSignal("Patch Cracks");
	std::cout << "starting patch cracks" << std::endl;
	crackPatch(initial);
	std::cout << "finished patching cracks" << std::endl;

	emit progressTextSignal("Reading Points");
	readPointsFromOctree(initial);




	//Need to delete the cube or otherwise we will leek memory
	for (int i = 1; i < octreeVector.size(); i++){
		delete octreeVector[i];
	}
	octreeVector.clear();

	emit finishedMeshSignal();

	return 0;

}


/*! \brief Ejecuta el algoritmo de AMC pero dejando que se formen agujeros ya que no usa la funcion de crackPatch. Solo se usa para demonstraciones
*/
int AdaptiveCuber::runWithCracks(){
	std::cout << "Starting adaptive marching cubes 3 with cracks" << std::endl;
	mesh->verts.clear();
	mesh->normals.clear();
	octreeVector.clear();
	//Calculate the gradients
	//Create the original cube
	/*
	7---------6
	size Z
	/			  !
	3---------2	  5
	!
	size Y
	!
	origin----size X
	*/


	int cubesSize = 0, depth = 0, maxDepth = octreeMaxDepth;

	emit progressTextSignal("Generating Gradients");
	if (gradient->size() == 0 || gradient->empty())
		calculateGradient();
	cout << "Finished calculating gradients" << endl;


	OctreeCube* initial = new OctreeCube;
	

	initial = createInitialCube();

	octreeVector.push_back(initial);
	emit progressTextSignal("Generating Octree");
	std::cout << "starting to generate octree" << std::endl;
	generateOctree_tree_version(*initial);
	std::cout << "finished generateing octree" << std::endl;


	emit progressTextSignal("Polygonise Octree");
	polygoniseOctree2(initial);	//Polygonise octree will polygonise each octree but assign the points to the cube itself an not to the model->verts


	std::cout << "starting patch cracks" << std::endl;
	//crackPatch(initial);
	std::cout << "finished patching cracks" << std::endl;

	readPointsFromOctree(initial);


	//Need to delete the cube or oterwise we will leek memory
	for (int i = 1; i < octreeVector.size(); i++){
		delete octreeVector[i];
	}
	octreeVector.clear();

	emit finishedMeshSignal();
	return 0;

}


/*! \brief Calcula los gradientes del volumen usando un operador Sobel
*
*  Halla los puntos de la superficie del volumen (superficie definida por el valor isoLevel). 
*  Despues para cada uno de estos puntos situados en la frontera o la superficie, se convoluciona un filtro Sobel para hallar el gradiente.
*/
void AdaptiveCuber::calculateGradient(){


	int dx = 0, dy = 0, dz = 0, magnitude = 0;
	float angle = -1;
	glm::vec3  gradientAtPoint;


	//Create as many gradients frames as the number of frames in the dicom file
	gradient->resize(frames);
	if (gradient->size() == 0)
		return;


	std::vector<glm::vec3> border; //Puntos de la frontera 

	for (int i = 0; i < pixelDataHeight-1; i = i + 1){
		for (int j = 0; j < pixelDataWidth-1; j = j + 1){
			for (int k = 0; k < frames-1; k = k + 1){


				if (pixelData->getPixelValue(j, i, k) > isoLevel){
					//si todos sus vecinos en las 6 dirreciones, tambien estan dentro del isolevel, es que es un punto interior
					//Si no es el caso, significa que un vecino esta fuera por lo tanto este punto tiene una ventana fuera de la superficie y se consiera como un contorno

					//No se si considerar 6 vecinos o tambien los vecinos en diagonal, es decir las dirreciones j+1,i+1,k+1, etc

					if (pixelData->getPixelValue(j + 1, i, k) > isoLevel &&
						pixelData->getPixelValue(j, i + 1, k) > isoLevel &&
						pixelData->getSmoothPixelValue(j, i, k + 1) > isoLevel &&

						pixelData->getPixelValue(j - 1, i, k) > isoLevel &&
						pixelData->getPixelValue(j, i - 1, k) > isoLevel &&
						pixelData->getPixelValue(j, i, k - 1) > isoLevel){
						continue;
					}
					else{
						glm::vec3 point;
						point.x = j;
						point.y = i;
						point.z = k;
						border.push_back(point);
					}

				}
				
			}
		}
	}


	//Go through all the points in border al convolver them
	for (int i = 0; i < border.size(); i++){

		dx = pixelData->convolveX(border[i].x, border[i].y, border[i].z);
		dy = pixelData->convolveY(border[i].x, border[i].y, border[i].z);
		dz = pixelData->convolveZ(border[i].x, border[i].y, border[i].z);

		if (dx == 0)
			dx = 1;
		if (dy == 0)
			dy = 1;
		if (dz == 0)
			dz = 1;

		gradientAtPoint.x = atan2(dy, dx) * 180 / PI + 180;
		gradientAtPoint.y = atan2(dx, dz) * 180 / PI + 180;
		gradientAtPoint.z = atan2(dz, dy) * 180 / PI + 180;


		gradient->at(border[i].z)[std::make_pair(round(border[i].x), round(border[i].y))] = gradientAtPoint;
	}


	return;

}





/*! \brief Crea el cubo inicial del octree, el cubo raiz.
*
*  Es necesario que el cubo inicial tenga las mismas dimensiones en los tres ejes y ademas esas dimensiones sean potencias de 2 para que el proceso de subdividir funcione correctamente.
*  Esto se realiza  buscando el maximo valor de las dimensiones del volumen y luego se redondea hacia arriba a la proxima potencia de 2. Este valor se asigna como las dimensiones del cubo.
*/
OctreeCube* AdaptiveCuber::createInitialCube(){
	OctreeCube* cube= new OctreeCube;
	cube->origin.x = 0 ;
	cube->origin.y = 0 ;
	cube->origin.z = 0 ;


	int cubeSize = (std::max)((std::max)(pixelDataWidth , pixelDataHeight ), frames - 1 );	//We get the maximum value
	cubeSize = pow(2, ceil(log(cubeSize) / log(2)));	// we round it up to the neerest power of 2

	cube->sizeX = cubeSize;
	cube->sizeY = cubeSize;
	cube->sizeZ = cubeSize;

	cube->depth = 0;

	cube->locationalCode.resize(octreeMaxDepth);
	for (int i = 0; i < cube->locationalCode.size(); i++){
		cube->locationalCode[i] = 0;
	}

	//model->cubes.push_back(cube);
	return cube;
}

/*! \brief Genera el arbol octree mediante un algoritmo recursivo, recibiendo como parametro el cubo raiz y la profundidad actual.
*
*	Crea el octree que particiona el volumen de datos en zonas dependiendo de la complejidad de la region. Es el corazon del algoritmo AMC
*/
int AdaptiveCuber::generateOctree_tree_version(OctreeCube& currentCube, int currentDepth){


	octreeVector.push_back(&currentCube);


	//Grab the initial one and see if it needs subdivision and if we are also below the maximum octree depth, if it needs subdiviosn, sundivid it, and also recursivelly call the same function for the children

	if (currentDepth < octreeMaxDepth && cubeNeedsSubdivision(currentCube) && currentCube.needsChecking){
		currentCube.subdivide();
		currentCube.isLeaf = false;
		
		generateOctree_tree_version(*(currentCube.children[0]), currentDepth + 1);
		generateOctree_tree_version(*(currentCube.children[1]), currentDepth + 1);
		generateOctree_tree_version(*(currentCube.children[2]), currentDepth + 1);
		generateOctree_tree_version(*(currentCube.children[3]), currentDepth + 1);
		generateOctree_tree_version(*(currentCube.children[4]), currentDepth + 1);
		generateOctree_tree_version(*(currentCube.children[5]), currentDepth + 1);
		generateOctree_tree_version(*(currentCube.children[6]), currentDepth + 1);
		generateOctree_tree_version(*(currentCube.children[7]), currentDepth + 1);
	}
	else{
		currentCube.isLeaf = true;	
		currentCube.needsChecking = false;
		return 0;
	}
	return 0;
}

/*! \brief Pasandole un cubo como parametro decide si necesita subdividirse o no
*
*  Se le pasa un cubo del octree como parametro. 
*  Se recorren todos los gradientes que puede haber dentro de ese cubo. 
*  Cuando se encuentra el primero se toma como valor de referencia y se siguen mirando los demas valores. 
*  Si en algun momento uno de ellos se desvia en alguna direccion mas que el valor de tolerancia, se retorna un true de la funcion indicando que el cubo necesita subdividirse.
*/
inline bool AdaptiveCuber::cubeNeedsSubdivision(OctreeCube &cube){



	glm::vec3 firstAngle(-1, -1, -1);
	boost::unordered_map< std::pair<int, int>, glm::vec3>::iterator it;
	bool gotReference = false;


	//We first check if the cube is totally out of the volume
	if (cube.origin.x > pixelDataWidth || cube.origin.y > pixelDataHeight || cube.origin.z >frames - 1){
		cube.needsChecking = false;
		cube.isLeaf = true;
		return false;
	}

	//Then we check if it is in the border between the actual volume and the initial cube in which case we need to subdivide
	if (cube.origin.x + cube.sizeX > pixelDataWidth || cube.origin.y + cube.sizeY > pixelDataHeight || cube.origin.z + cube.sizeZ > frames - 1){
		cube.needsChecking = true;
		return true;
	}

	//From now on all the cubes will be totally within the volume
	

	float angleDifX = 0.0;
	float angleDifY = 0.0;
	float angleDifZ = 0.0;
	float angleMagnitude = 0.0;

	for (int k = cube.origin.z; k < cube.sizeZ + cube.origin.z; k = k + 1){
		for (int i = cube.origin.y; i < cube.sizeY + cube.origin.y; i = i + 1){
			for (int j = cube.origin.x; j < cube.sizeX + cube.origin.x; j = j + 1){


				glm::vec3 actualValue(-1, -1, -1);
				it = gradient->at(k).find(std::make_pair(j, i));
				if (it != gradient->at(k).end())
				{
					//gradient found;
					actualValue = it->second;
					if (!gotReference){
						firstAngle = actualValue; //Valor de referencia
						gotReference = true;
					}
				}
				else{
					//We didn´t find a gradient at this position
					continue;
				}
			



				//Calculate differencia between the firstAngle that we encountered and the one that we found now
				angleDifX = 180 - abs(abs(firstAngle.x - actualValue.x) - 180);
				angleDifY = 180 - abs(abs(firstAngle.y - actualValue.y) - 180);
				angleDifZ = 180 - abs(abs(firstAngle.z - actualValue.z) - 180);

				angleMagnitude = sqrt(angleDifX*angleDifX + angleDifY*angleDifY + angleDifZ*angleDifZ);

				//If the angle changes more than the tolerance and we actually have a reference value and an actual value to compare, return true
				if (angleMagnitude > tolerance && firstAngle != glm::vec3(-1, -1, -1) && actualValue != glm::vec3(-1, -1, -1) && gotReference){
					return true;
				}

				
				

			}
		}
	}
	cube.needsChecking = false;
	return false;
}



/*! \brief Extrae los triangulos, poligonizando los cubos del octree pero asigna los puntos al cubo en si en vez de meterlos en el mallado
*
*  Se recorre el octree por amplitud y por cada cubo que sea hoja, se polygoniza para descubrir que triangulos se forman dentro de el. Despues los puntos de los trinagulo
*  se asiagnan como un vector al cubo en si y no al mesh todavia. Se necesita que los cubos tengan informacion de los puntos formados dentro para poder moverlos con el
*  algoritmo de crack patching. Despues la funcion readPointsFromOctree leeran los puntos de todo el arbol y los asignara al mallado
*/
int AdaptiveCuber::polygoniseOctree2(OctreeCube* root){

	OctreeCube* currentCube;
	std::queue<OctreeCube*> queue;
	queue.push(root);
	int cubeNumber = 0;
	while (!queue.empty()){
		currentCube = queue.front();
		queue.pop();

		cubeNumber++;
		if (cubeNumber % (octreeVector.size() / 10) == 0)
			emit progressValueChangedSignal(cubeNumber * 100 / (octreeVector.size()));


		if (currentCube->children[0] != NULL){
			queue.push(currentCube->children[0]);
			queue.push(currentCube->children[1]);
			queue.push(currentCube->children[2]);
			queue.push(currentCube->children[3]);
			queue.push(currentCube->children[4]);
			queue.push(currentCube->children[5]);
			queue.push(currentCube->children[6]);
			queue.push(currentCube->children[7]);
		}

		//Grab the current cube and polygonise it f it's leaf
		if (currentCube->isLeaf){
			polygoniseAssignToCube(currentCube);
		}

	}
	return 0;
}

/*! \brief Crea triangulos dentro de una cubo
*
*  Se crea un vector on los puntos que se crean dentro del triangulo. 
*  Ese vector se le asigna al cubo, de esta manera cada cubo tendra informacion de los puntos (y por lo tanto triangulos) que se han formado dentro de el.
*  Es necesario que cada cubo tenga informacion de sus puntos y triangulo para el correcto funcionamiento del algoritmo de crack patching
*/
int AdaptiveCuber::polygoniseAssignToCube(OctreeCube* currentCube){



	//Create the cell
	CELL cell;

	for (int j = 0; j < 8; j++){
		cell.position[j].x = 0;
		cell.position[j].x = 0;
		cell.position[j].x = 0;
		cell.val[j] = 0;
	}

	cell.position[0].x = currentCube->origin.x;
	cell.position[0].y = currentCube->origin.y;
	cell.position[0].z = currentCube->origin.z;
	cell.val[0] = pixelData->getPixelValue(currentCube->origin.x, currentCube->origin.y, currentCube->origin.z);

	cell.position[1].x = currentCube->origin.x + currentCube->sizeX;
	cell.position[1].y = currentCube->origin.y;
	cell.position[1].z = currentCube->origin.z;
	cell.val[1] = pixelData->getPixelValue(currentCube->origin.x + currentCube->sizeX, currentCube->origin.y, currentCube->origin.z);

	cell.position[2].x = currentCube->origin.x + currentCube->sizeX;
	cell.position[2].y = currentCube->origin.y + currentCube->sizeY;
	cell.position[2].z = currentCube->origin.z;
	cell.val[2] = pixelData->getPixelValue(currentCube->origin.x + currentCube->sizeX, currentCube->origin.y + currentCube->sizeY, currentCube->origin.z);

	cell.position[3].x = currentCube->origin.x;
	cell.position[3].y = currentCube->origin.y + currentCube->sizeY;
	cell.position[3].z = currentCube->origin.z;
	cell.val[3] = pixelData->getPixelValue(currentCube->origin.x, currentCube->origin.y + currentCube->sizeY, currentCube->origin.z);
	//////
	cell.position[4].x = currentCube->origin.x;
	cell.position[4].y = currentCube->origin.y;
	cell.position[4].z = currentCube->origin.z + currentCube->sizeZ;
	cell.val[4] = pixelData->getPixelValue(currentCube->origin.x, currentCube->origin.y, currentCube->origin.z + currentCube->sizeZ);

	cell.position[5].x = currentCube->origin.x + currentCube->sizeX;
	cell.position[5].y = currentCube->origin.y;
	cell.position[5].z = currentCube->origin.z + currentCube->sizeZ;
	cell.val[5] = pixelData->getPixelValue(currentCube->origin.x + currentCube->sizeX, currentCube->origin.y, currentCube->origin.z + currentCube->sizeZ);

	cell.position[6].x = currentCube->origin.x + currentCube->sizeX;
	cell.position[6].y = currentCube->origin.y + currentCube->sizeY;
	cell.position[6].z = currentCube->origin.z + currentCube->sizeZ;
	cell.val[6] = pixelData->getPixelValue(currentCube->origin.x + currentCube->sizeX, currentCube->origin.y + currentCube->sizeY, currentCube->origin.z + currentCube->sizeZ);

	cell.position[7].x = currentCube->origin.x;
	cell.position[7].y = currentCube->origin.y + currentCube->sizeY;
	cell.position[7].z = currentCube->origin.z + currentCube->sizeZ;
	cell.val[7] = pixelData->getPixelValue(currentCube->origin.x, currentCube->origin.y + currentCube->sizeY, currentCube->origin.z + currentCube->sizeZ);


	//Now we have the cell for the cube created, so we polygonise it


	glm::vec3 vertlist[12];


	int cubeIndex = 0;

	if (cell.val[0] > isoLevel) cubeIndex += 1;
	if (cell.val[1] > isoLevel) cubeIndex += 2;
	if (cell.val[2] > isoLevel) cubeIndex += 4;
	if (cell.val[3] > isoLevel) cubeIndex += 8;
	if (cell.val[4] > isoLevel) cubeIndex += 16;
	if (cell.val[5] > isoLevel) cubeIndex += 32;
	if (cell.val[6] > isoLevel) cubeIndex += 64;
	if (cell.val[7] > isoLevel) cubeIndex += 128;


	if (cubeIndex == 255 || cubeIndex == 0){
		return 0;	//REturn a 0 to indicate that the cell doesnt contain any vertices
	}

	glm::vec3 point;

	/* Find the vertices where the surface intersects the cube */
	if (edgeTable[cubeIndex] & 1) {
		interpolate(isoLevel, cell.position[0], cell.position[1], cell.val[0], cell.val[1], point);
		vertlist[0] = point;
	}
	if (edgeTable[cubeIndex] & 2) {
		interpolate(isoLevel, cell.position[1], cell.position[2], cell.val[1], cell.val[2], point);
		vertlist[1] = point;
	}
	if (edgeTable[cubeIndex] & 4) {
		interpolate(isoLevel, cell.position[2], cell.position[3], cell.val[2], cell.val[3], point);
		vertlist[2] = point;
	}
	if (edgeTable[cubeIndex] & 8) {
		interpolate(isoLevel, cell.position[3], cell.position[0], cell.val[3], cell.val[0], point);
		vertlist[3] = point;

	}
	if (edgeTable[cubeIndex] & 16) {
		interpolate(isoLevel, cell.position[4], cell.position[5], cell.val[4], cell.val[5], point);
		vertlist[4] = point;
	}
	if (edgeTable[cubeIndex] & 32) {
		interpolate(isoLevel, cell.position[5], cell.position[6], cell.val[5], cell.val[6], point);
		vertlist[5] = point;
	}
	if (edgeTable[cubeIndex] & 64) {
		interpolate(isoLevel, cell.position[6], cell.position[7], cell.val[6], cell.val[7], point);
		vertlist[6] = point;
	}
	if (edgeTable[cubeIndex] & 128) {
		interpolate(isoLevel, cell.position[7], cell.position[4], cell.val[7], cell.val[4], point);
		vertlist[7] = point;
	}
	if (edgeTable[cubeIndex] & 256) {
		interpolate(isoLevel, cell.position[0], cell.position[4], cell.val[0], cell.val[4], point);
		vertlist[8] = point;
	}
	if (edgeTable[cubeIndex] & 512) {
		interpolate(isoLevel, cell.position[1], cell.position[5], cell.val[1], cell.val[5], point);
		vertlist[9] = point;
	}
	if (edgeTable[cubeIndex] & 1024) {
		interpolate(isoLevel, cell.position[2], cell.position[6], cell.val[2], cell.val[6], point);
		vertlist[10] = point;
	}
	if (edgeTable[cubeIndex] & 2048) {
		interpolate(isoLevel, cell.position[3], cell.position[7], cell.val[3], cell.val[7], point);
		vertlist[11] = point;
	}


	glm::vec3 normal;
	unsigned char* dataPointer;
	int value1 = 0, value2 = 0;

	/* Create the triangles */

	//We create a vector with the points and we assignt it to the cube
	vector<glm::vec3> *points = new vector<glm::vec3>;

	glm::vec3 point1, point2, point3;
	for (int i = 0; triTable[cubeIndex][i] != -1; i += 3) {

		point1.x = (vertlist[triTable[cubeIndex][i]]).x;
		point1.y = (vertlist[triTable[cubeIndex][i]]).y;
		point1.z = (vertlist[triTable[cubeIndex][i]]).z;

		point2.x = (vertlist[triTable[cubeIndex][i + 1]]).x;
		point2.y = (vertlist[triTable[cubeIndex][i + 1]]).y;
		point2.z = (vertlist[triTable[cubeIndex][i + 1]]).z;

		point3.x = (vertlist[triTable[cubeIndex][i + 2]]).x;
		point3.y = (vertlist[triTable[cubeIndex][i + 2]]).y;
		point3.z = (vertlist[triTable[cubeIndex][i + 2]]).z;

		points->push_back(point1);
		points->push_back(point2);
		points->push_back(point3);

	}

	currentCube->points = points;
	currentCube->containsVerts = true;

}


/*! \brief Resuelve los agujeros en el mallado usando proyeccion ortogonal de los puntos que forman el agujero
*
*  Se recorre el octree y por cada cubo que sea hoja se buscan sus 6 vecinos. 
*  Se recogen los puntos tantos de los vecinos como del cubo actual y despues se proyectan los puntos del vecino sobre la linea formada en la cara del cubo actual.
*/
void AdaptiveCuber::crackPatch(OctreeCube* root){


	OctreeCube* currentCube;
	OctreeCube* adyacent;

	OctreeCube* right, *left, *top, *bottom, *further, *closer;

	int cubeNumber = 0;
	

	std::queue<OctreeCube*> bigQueue; //Cola para recorrer el arbol en amplitud
	bigQueue.push(root);
	while (!bigQueue.empty()){
		currentCube = bigQueue.front();
		bigQueue.pop();


		cubeNumber++; //Para mandar señales de progreso
		if (cubeNumber % (octreeVector.size() / 50) == 0)
			emit progressValueChangedSignal(cubeNumber * 100 / (octreeVector.size()));


		if (!currentCube->isLeaf){
			bigQueue.push(currentCube->children[0]);
			bigQueue.push(currentCube->children[1]);
			bigQueue.push(currentCube->children[2]);
			bigQueue.push(currentCube->children[3]);
			bigQueue.push(currentCube->children[4]);
			bigQueue.push(currentCube->children[5]);
			bigQueue.push(currentCube->children[6]);
			bigQueue.push(currentCube->children[7]);
		}
		
		right = NULL;
		left = NULL;
		top = NULL;
		bottom = NULL;
		further = NULL;
		closer = NULL;

		


		if (currentCube->isLeaf && currentCube->containsVerts){

			findNeighbours(currentCube, root,right,left,top,bottom,further,closer );

			
			patchFace(currentCube, right, RIGHT);
			patchFace(currentCube, left, LEFT);
			patchFace(currentCube, top, UP);
			patchFace(currentCube, bottom, DOWN);
			patchFace(currentCube, further, FURTHER);
			patchFace(currentCube, closer, CLOSER);



			


		}
	}

}

/*! \brief Realiza la proyeccion ortogonal del punto x sobre la linea formada por los puntos a y b.
*/
glm::vec3 AdaptiveCuber::pointOnLine(glm::vec3 a, glm::vec3 b, glm::vec3& x){


	//Se puede hacer el calculo de dos maneras, usando el modulo o el angulo entre los dos vectores
	//Ambos dan el mismo resultado. Se puede descomentar la linea div=... para observar 

	float dot = a.x*b.x + a.y*b.y + a.z*b.z;
	float lenSq1 = a.x*a.x + a.y*a.y + a.z*a.z;
	float lenSq2 = b.x*b.x + b.y*b.y + b.z*b.z;
	float angle = acos(dot / sqrt(lenSq1 * lenSq2));

	glm::vec3 x_b, a_b, a_bDiv, projection;
	float x_b_a_bDot, a_bModule, a_bModulePow, div, x_bModule, optional;

	x_b.x = x.x - b.x;
	x_b.y = x.y - b.y;
	x_b.z = x.z - b.z;

	a_b.x = a.x - b.x;
	a_b.y = a.y - b.y;
	a_b.z = a.z - b.z;



	/////////////Optional
	x_bModule = sqrt(x_b.x*x_b.x + x_b.y*x_b.y + x_b.z*x_b.z);
	a_bModule = sqrt(a_b.x*a_b.x + a_b.y*a_b.y + a_b.z*a_b.z);
	optional = x_bModule*a_bModule*cos(angle);

	///////End of optional


	x_b_a_bDot = x_b.x*a_b.x + x_b.y*a_b.y + x_b.z*a_b.z;
	a_bModule = sqrt(a_b.x*a_b.x + a_b.y*a_b.y + a_b.z*a_b.z);
	a_bModulePow = a_bModule*a_bModule;

	div = optional / a_bModulePow;
	//div = x_b_a_bDot / a_bModulePow;

	a_bDiv.x = a_b.x*div;
	a_bDiv.y = a_b.y*div;
	a_bDiv.z = a_b.z*div;

	projection.x = b.x + a_bDiv.x;
	projection.y = b.y + a_bDiv.y;
	projection.z = b.z + a_bDiv.z;

	return projection;
}

/*! \brief Recorre el arbol y lee los puntos a los que tiene referencia cada cubo. Esos puntos se escriben en el mallo.
*/
int AdaptiveCuber::readPointsFromOctree(OctreeCube* root){
	OctreeCube* currentCube;
	std::queue<OctreeCube*> queue;
	queue.push(root);
	while (!queue.empty()){
		currentCube = queue.front();
		queue.pop();
		if (currentCube->children[0] != NULL){
			queue.push(currentCube->children[0]);
			queue.push(currentCube->children[1]);
			queue.push(currentCube->children[2]);
			queue.push(currentCube->children[3]);
			queue.push(currentCube->children[4]);
			queue.push(currentCube->children[5]);
			queue.push(currentCube->children[6]);
			queue.push(currentCube->children[7]);
		}

		//Tiene puntos asi que se leen en el mallado
		if (currentCube->points != NULL){
			for (int i = 0; i < currentCube->points->size(); i++){
				mesh->verts.push_back(currentCube->points->at(i));

			}
		}

	}
	return 0;
}

/*! \brief Funcion que te devuelve los 6 hijos vecinos para uno dado. Es necesario para el algoritmo de crackPatch
*/
void AdaptiveCuber::findNeighbours(OctreeCube* currentCube, OctreeCube* initial, OctreeCube*& right, OctreeCube*& left, OctreeCube*& top, OctreeCube*& bottom, OctreeCube*& further, OctreeCube*& closer){

	right = NULL;
	left = NULL;
	top = NULL;
	bottom = NULL;
	further = NULL;
	closer = NULL;
	OctreeCube* adyacent=NULL;


	//Findlocational code for each direction
	//traverse the octree with that locational code and return the octreecube at that location or null if there is nothing

	// The directions are in the same orders as the points to the cube: right,left,top,bottom,further,closer
	std::vector<int> neighLoc;
	
	neighLoc=getLocationalOfNeighbours(currentCube->locationalCode, currentCube->depth, RIGHT);
	//Now that we have the location of the neightbout we need to traverse the cube and find it weather it exists or not
	right = traverseWithLocation(initial, neighLoc, currentCube->depth);

	neighLoc = getLocationalOfNeighbours(currentCube->locationalCode, currentCube->depth, LEFT);
	left = traverseWithLocation(initial, neighLoc, currentCube->depth);

	neighLoc = getLocationalOfNeighbours(currentCube->locationalCode, currentCube->depth, UP);
	top = traverseWithLocation(initial, neighLoc, currentCube->depth);

	neighLoc = getLocationalOfNeighbours(currentCube->locationalCode, currentCube->depth, DOWN);
	bottom = traverseWithLocation(initial, neighLoc, currentCube->depth);


	neighLoc = getLocationalOfNeighbours(currentCube->locationalCode, currentCube->depth, FURTHER);
	further = traverseWithLocation(initial, neighLoc, currentCube->depth);


	neighLoc = getLocationalOfNeighbours(currentCube->locationalCode, currentCube->depth, CLOSER);
	closer = traverseWithLocation(initial, neighLoc, currentCube->depth);

	return;
}


//Function that given a locational code, a depth and a direction return the locationalCode of the neighbour weather it exists or not

/*! \brief Dado un codigo de localizacion, una profundidad y una direccion, te calcula el codigo del vecino en esa direccion
*
*	Se pasa el codigo de localizacion dado a binario y se recorre desde el bit menos significativo hacia el mas, realizando sumas o restas dependiendo de la direccion escogida
*/
std::vector<int> AdaptiveCuber::getLocationalOfNeighbours(std::vector<int> currentLocation, int depth, int direction){


	std::vector<int> neighLoc (currentLocation); //First we copy the current location into the neighbour one



	for (int i = depth - 1; i >= 0; i--){
		//move the number at index i in the direction, if it changes subsection, continue moving them, if not then break from the loop
		unsigned long value = 0;
		value = neighLoc[i];
		boost::dynamic_bitset<> valueInBits (3, value);


		//in the ValueInBits the 0 index is the least significant one
		if (direction == RIGHT){
			if (valueInBits[0] == 0){
				valueInBits[0] = 1;
				//and we finished
				neighLoc[i] = valueInBits.to_ulong();
				break;
			}
			else{
				valueInBits[0] = 0;
				neighLoc[i] = valueInBits.to_ulong();
			}
		}



		if (direction == LEFT){
			if (valueInBits[0] == 1){
				valueInBits[0] = 0;
				neighLoc[i] = valueInBits.to_ulong();
				break;
			}
			else{
				valueInBits[0] = 1;
				neighLoc[i] = valueInBits.to_ulong();
			}
		}

		
		if (direction == UP){
			if (valueInBits[1] == 1){
				valueInBits[1] = 0;
				neighLoc[i] = valueInBits.to_ulong();
				break;
			}
			else{
				valueInBits[1] = 1;
				neighLoc[i] = valueInBits.to_ulong();
			}
		}


		if (direction == DOWN){
			if (valueInBits[1] == 0){
				valueInBits[1] = 1;
				neighLoc[i] = valueInBits.to_ulong();
				break;
			}
			else{
				valueInBits[1] = 0;
				neighLoc[i] = valueInBits.to_ulong();
			}
		}


		if (direction == FURTHER){
			if (valueInBits[2] == 0){
				valueInBits[2] = 1;
				neighLoc[i] = valueInBits.to_ulong();
				break;
			}
			else{
				valueInBits[2] = 0;
				neighLoc[i] = valueInBits.to_ulong();
			}
		}


		if (direction == CLOSER){
			if (valueInBits[2] == 1){
				valueInBits[2] = 0;
				neighLoc[i] = valueInBits.to_ulong();
				break;
			}
			else{
				valueInBits[2] = 1;
				neighLoc[i] = valueInBits.to_ulong();
			}
		}

	}

	
	return neighLoc;

}

/*! \brief Dado un codigo de localizacion, itera sobre el arbol hasta encontrar el nodo con ese código. Devuelve null si no existe
*/
OctreeCube* AdaptiveCuber::traverseWithLocation(OctreeCube* root, std::vector<int> neighLoc, int depth){


	OctreeCube* neighbour;
	for (int i = 0; i < depth; i++){
		neighbour = root->children[neighLoc[i]];
		root = neighbour;

		if (root == NULL)
			return root;
	}

	return root;



}

/*! \brief Dados dos cubos y la direccion entre ellos, arregla el agujero que hay en la cara que los separa
* 
*	Se recorre el vector del nodo actual para recoger sus puntos. Se escogen solo los que estan en la cara indicada por la direccion. 
*   Si hay solo dos puntos, forman una recta asi que puede haber un agujero. El cubo de alta resolucion se recorre en amplitud para recoger los puntos de todos sus hijos
*   Estos puntos se proyectan sobre la recta anteriormente hallada y de esta manera se tapa el agujero
*/
void AdaptiveCuber::patchFace(OctreeCube* lowRes, OctreeCube* highRes, int direction){

	std::vector<glm::vec3> pointsOnLowResFace;
	pointsOnLowResFace.clear();



	/*bool test = true;
	test= checkSurfaceOrientation(lowRes, highRes);
	if (test == false)
		return;*/


	if (highRes != NULL && !highRes->isLeaf){


		//we grab the points from the cube that are on the face indicated by the direction

		//The first one is the cube from where we get the points, the second is the one used as reference for the faces, we append the points to the vector
		getPointsFromFace(lowRes, lowRes, pointsOnLowResFace, direction);
		pointsOnLowResFace=removeDuplicates(pointsOnLowResFace);

		//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
		if (pointsOnLowResFace.size() == 2){
			//Now we get the points from the face on the cube of higer resolution. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
			std::vector<glm::vec3> pointsOnFace2Old;
			std::vector<glm::vec3> pointsOnFace2New;

			std::queue<OctreeCube*> queue;
			OctreeCube* child;
			queue.push(highRes);
			while (!queue.empty()){
				child = queue.front();
				queue.pop();
				if (!child->isLeaf){
					queue.push(child->children[0]);
					queue.push(child->children[1]);
					queue.push(child->children[2]);
					queue.push(child->children[3]);
					queue.push(child->children[4]);
					queue.push(child->children[5]);
					queue.push(child->children[6]);
					queue.push(child->children[7]);
				}
				if (child->containsVerts)
					getPointsFromFace(child, lowRes, pointsOnFace2Old, direction);
			}
			

			//we project the points and store the new values in pointsOnFace2New
			//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
			for (int i = 0; i < pointsOnFace2Old.size(); i++){
				pointsOnFace2New.push_back(pointOnLine(pointsOnLowResFace[0], pointsOnLowResFace[1], pointsOnFace2Old[i]));
			}

			//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

			queue.push(highRes);
			while (!queue.empty()){
				child = queue.front();
				queue.pop();
				if (!child->isLeaf){
					queue.push(child->children[0]);
					queue.push(child->children[1]);
					queue.push(child->children[2]);
					queue.push(child->children[3]);
					queue.push(child->children[4]);
					queue.push(child->children[5]);
					queue.push(child->children[6]);
					queue.push(child->children[7]);
				}
				if (child->points != NULL){
					for (int j = 0; j < child->points->size(); j++){
						//For each one of those points, check to see if it matches on in pointsOnFace2old, If it does, change it with the point in the same index in pointsOnFace2New
						for (int k = 0; k < pointsOnFace2Old.size(); k++){
							if (child->points->at(j).x == pointsOnFace2Old[k].x && child->points->at(j).y == pointsOnFace2Old[k].y && child->points->at(j).z == pointsOnFace2Old[k].z){
								child->points->at(j).x = pointsOnFace2New[k].x;
								child->points->at(j).y = pointsOnFace2New[k].y;
								child->points->at(j).z = pointsOnFace2New[k].z;
							}
						}

					}
				}
			}


		}
	}

}


/*! \brief Dado un cubo del cual se reciben los puntos y otro cubo que se usará como referencia para la direccion, se devuelven los puntos sobre que estan en la cara
*/
void AdaptiveCuber::getPointsFromFace(OctreeCube* cube, OctreeCube* reference, std::vector<glm::vec3>& vec, int direction){


	//THe difference between the two values was of 0.004
	float epsilon = 0.25;

	for (int i = 0; i < cube->points->size(); i++){
		
		if (direction == RIGHT){
			if (  abs( cube->points->at(i).x - (reference->origin.x + reference->sizeX) )  < epsilon ){
				vec.push_back(cube->points->at(i));
			}
		}
		
		if (direction == LEFT){
			if (abs(cube->points->at(i).x - (reference->origin.x))<epsilon){
				vec.push_back(cube->points->at(i));
			}
		}
		

		if (direction == UP){
			if (abs(cube->points->at(i).y - (reference->origin.y + reference->sizeY))<epsilon){
				vec.push_back(cube->points->at(i));
			}
		}

		if (direction == DOWN){
			if (abs(cube->points->at(i).y - (reference->origin.y))<epsilon){
				vec.push_back(cube->points->at(i));
			}
		}

		if (direction == FURTHER){
			if (abs(cube->points->at(i).z - (reference->origin.z + reference->sizeZ))<epsilon){
				vec.push_back(cube->points->at(i));
			}
		}

		if (direction == CLOSER){
			if (abs(cube->points->at(i).z - (reference->origin.z))<epsilon){
				vec.push_back(cube->points->at(i));
			}
		}
	}

	

}

/*! \brief Dados dos cubos y la direccion entre ellos, arregla el agujero que hay en la cara que los separa
*
*	Es necesario ya que dados dos triangulos que comparten arista, significa que los 2 puntos que tienen en comun son realmetne 4 puntos pero que se superponen 2 a 2.
*   Para el algoritmo de cack patch tenemos que ver si en la cara de transicion se forma una recta, es decir 2 puntos pero que estos dos puntos no coincida. 
*   Por lo tanto es necesario quitar los puntos que coinciden en el espacio, es decir los duplicados
*/
std::vector<glm::vec3>  AdaptiveCuber::removeDuplicates(std::vector<glm::vec3> vec){

	std::vector<glm::vec3> pointsOnFaceNoDuplicates;
	bool skipPoint = false;
	for (int i = 0; i < vec.size(); i++){
		skipPoint = false;
		//For each point check if it's not already in the pointOnFaceNoDuplicates
		for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
			//if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
			if (vec[i] == pointsOnFaceNoDuplicates[j]){
				//There is already on in pointface no duplicates with the same value
				skipPoint = true;
			}
		}
		if (!skipPoint)
			pointsOnFaceNoDuplicates.push_back(vec[i]);
	}


	return pointsOnFaceNoDuplicates;
}

/*! \brief Intento de arreglar el hecho de que crackPatch parchea agujeros que no deberia a veces
*
*	CrackPatch a veces tapa agujeros indebidos como la separacion entre los dedos. Esto se debe a que interpreta estas separaciones como agujeros
*	La solucion viene por comprobar los gradientes, es decir los angulos de la superficie entre los dos cubos que se deben aprchear
*	Si los gradientes apuntan en mas o menos la misma dirreción, se deberia parchear ya que representan la continuacion de la misma superficie
*	Si los angulos de los gradientes están enfrentados significa que la superficie no es ontinua, es decir, representan formas distintas asi que no se deberian parchear esos dos cubos
*/
bool AdaptiveCuber::checkSurfaceOrientation(OctreeCube* lowRes, OctreeCube* highRes){
	//Go through all the gradients of each cube and calculate the average gradient

	//If the magnitude of change between the two gradients surfaces is higher than a certain threshold, return false indicating that they are not on the same surface

	if (lowRes == NULL || highRes == NULL)
		return false;

	boost::unordered_map< std::pair<int, int>, glm::vec3>::iterator it;
	int count = 0;
	glm::vec3 averageLowRes(0, 0, 0);
	glm::vec3 averageHighRes(0, 0, 0);

	float angleDifX = 0.0;
	float angleDifY = 0.0;
	float angleDifZ = 0.0;
	float angleMagnitude = 0.0;


	for (int k = lowRes->origin.z; k < lowRes->sizeZ + lowRes->origin.z; k = k + 1){
		for (int i = lowRes->origin.y; i < lowRes->sizeY + lowRes->origin.y; i = i + 1){
			for (int j = lowRes->origin.x; j < lowRes->sizeX + lowRes->origin.x; j = j + 1){



				glm::vec3 actualValue(-1, -1, -1);
				it = gradient->at(k).find(std::make_pair(j, i));
				if (it != gradient->at(k).end())
				{
					//gradient found;
					actualValue = it->second;
					count++;
					averageLowRes.x += actualValue.x;
					averageLowRes.y += actualValue.y;
					averageLowRes.z += actualValue.z;

					// this value will get pishes onto an average vector

				}
				
			}
		}
	}

	if (count != 0){
		averageLowRes.x = averageLowRes.x / count;
		averageLowRes.y = averageLowRes.y / count;
		averageLowRes.z = averageLowRes.z / count;
	}


	


	count = 0;
	for (int k = highRes->origin.z; k < highRes->sizeZ + highRes->origin.z; k = k + 1){
		for (int i = highRes->origin.y; i < highRes->sizeY + highRes->origin.y; i = i + 1){
			for (int j = highRes->origin.x; j < highRes->sizeX + highRes->origin.x; j = j + 1){



				glm::vec3 actualValue(-1, -1, -1);
				if (k >= frames)
					return false;
				it = gradient->at(k).find(std::make_pair(j, i));
				if (it != gradient->at(k).end())
				{
					//gradient found;
					actualValue = it->second;
					count++;
					averageHighRes.x += actualValue.x;
					averageHighRes.y += actualValue.y;
					averageHighRes.z += actualValue.z;

					// this value will get pishes onto an average vector

				}

			}
		}
	}

	if (count != 0){
		averageHighRes.x = averageHighRes.x / count;
		averageHighRes.y = averageHighRes.y / count;
		averageHighRes.z = averageHighRes.z / count;
	}

	if (averageHighRes == glm::vec3(0, 0, 0) || averageLowRes == glm::vec3(0, 0, 0))	//they dont have any gradients inside therefore no surface
		return false;	//Return false because it doesnt need patching

	
	//Calculate the difference between the two vectors if it is bigger than a certain value, return false because they dont need patching
	angleDifX = 180 - abs(abs(averageLowRes.x - averageHighRes.x) - 180);
	angleDifY = 180 - abs(abs(averageLowRes.y - averageHighRes.y) - 180);
	angleDifZ = 180 - abs(abs(averageLowRes.z - averageHighRes.z) - 180);

	angleMagnitude = sqrt(angleDifX*angleDifX + angleDifY*angleDifY + angleDifZ*angleDifZ);

	if (angleMagnitude > 40){
		return false;
	}



	return true;


}