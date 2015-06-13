#pragma once
#include <vector>
#include <list>
#include <glm/glm.hpp>
#include "OctreeCube.h"
//#include <boost/variant.hpp>
//#include <boost/any.hpp>
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/vector_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <map>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <boost/unordered_map.hpp>




class Data
{
public:
	Data();
	~Data();
	virtual void** getData();
};

template<typename T>
class DataTemplate : public Data
{
public:
	T** data;
	DataTemplate();
	~DataTemplate();
	
};

class Model
{
public:
	Model();
	~Model();
	

	

	/*struct return_visitor: public boost::static_visitor <double >{
	public:
		double operator()(unsigned char**  i) const{

			return (double) i[z][x + y*width];
		}

		double operator()(unsigned short**  j) const{
			return (double)j[z][x + y*width];
		}

		double operator()(unsigned long**  k) const{
			return (double)k[z][x + y*width];
		}
		int x;
		int y;
		int z;
		int width;
	};*/
	int getPixelValue(int x, int y, int z);
	int getSmoothPixelValue(int x, int y, int z);


	int isoLevel;
	//std::vector<glm::vec3> totalPoints;
	std::vector<glm::vec3> verts;
	std::vector<glm::vec3> normals;
	int cellSizeX;
	int cellSizeY;
	int cellSizeZ;	
	unsigned char** pixelData;

	std::vector<std::vector<std::vector<std::vector<OctreeCube*> > > > array4D;
	std::vector<OctreeCube*>  octreeVector;

	std::vector<int>  edgeTable;
	std::vector< std::vector<int> > triTable;

	int algorithmChosen;
	int normalsAlgChosen;
	bool generatingMesh;
	
	

	std::vector<OctreeCube> cubes;
	//std::vector <std::vector<glm::vec3>> gradient;
	//std::vector <  boost::numeric::ublas::mapped_vector<int>   >> gradient;
	//boost::numeric::ublas::mapped_vector<glm::vec3> v;
	std::vector< boost::unordered_map< std::pair<int, int>, glm::vec3> > gradient;
	//std::vector<boost::numeric::ublas::compressed_matrix< glm::vec3> > m;
	std::vector <double> gradientPoints;

	int tolerance;
	int octreeMaxDepth;
	int interpolateDepth;
	int orientation;


	//boost::variant< unsigned char, unsigned short, unsigned long > ** pixelData;
	//boost::variant< unsigned char**, unsigned short**, unsigned long** >  pixelData;
	//boost::any **  pixelData;
	//void**  pixelData;
	int frames;
	int frame_to_display;
	int numberOfBytes;

	bool linearInterpolation;
	bool pointFlag;
	bool borderFlag;
	bool showWireframe;
	bool perspectiveActivated;
	bool showMesh;
	bool showGradient;
	bool showCubes;

	unsigned char *bitmapPixels;
	int pixelDataWidth;
	int pixelDataHeight;
	int bitmapImageSize;
	int bitmapNumberOfPixels;
	char signature[2];
};

