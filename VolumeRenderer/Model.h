#pragma once
#include <vector>
#include <list>
#include <glm/glm.hpp>
#include "OctreeCube.h"
//#include <boost/variant.hpp>
//#include <boost/any.hpp>




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
	int getPixelValue(int x, int y, int z);

	

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

	int isoLevel;
	std::vector<glm::vec3> totalPoints;
	std::vector<float> verts;
	std::vector<float> normals;
	int cellSizeX;
	int cellSizeY;
	int cellSizeZ;	
	unsigned char** pixelData;

	std::vector<int>  edgeTable;
	std::vector< std::vector<int> > triTable;
	

	std::vector<OctreeCube> cubes;


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

	unsigned char *bitmapPixels;
	int pixelDataWidth;
	int pixelDataHeight;
	int bitmapImageSize;
	int bitmapNumberOfPixels;
	char signature[2];
};

