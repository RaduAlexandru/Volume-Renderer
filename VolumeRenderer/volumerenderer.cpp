#include "GL/glew.h"
#include "volumerenderer.h"
#include <iostream>


#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"

#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcfilefo.h" 
#include "dcmtk/dcmdata/dcmetinf.h" 
#include "dcmtk/dcmimgle/dcmimage.h" 

#include "dcmtk/dcmjpeg/djdecode.h"     
#include "dcmtk/dcmjpeg/djencode.h"  

#include "dcmtk/dcmdata/dcpxitem.h"


#include <stdlib.h>
#include <cstdlib>
#include <QtWidgets/QApplication>
#include <QDebug>
#include <QFileDialog>


#include <GLFW/glfw3.h>
#include "glwidget.h"
#include "dicomviewer2dgl.h"
#include "Model.h"
#include "MarchingCuber.h"
#include "AdaptiveCuber.h"
#include <math.h> 
#include <tuple>
#include <boost/math/special_functions/round.hpp>
#include <boost/thread.hpp>
#include <queue>

//#include <boost/variant.hpp>
#define PI 3.14159265


struct mapCompare
{
	bool operator() (const std::pair<int, int>& lhs, const std::pair<int, int>& rhs)
	{
		if ((lhs.first + lhs.second) < (rhs.first + rhs.second))
		return lhs < rhs;
	}
};


using namespace::std;

/*
const int edgeTable[256] = {
	0x0, 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
	0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
	0x190, 0x99, 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
	0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
	0x230, 0x339, 0x33, 0x13a, 0x636, 0x73f, 0x435, 0x53c,
	0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
	0x3a0, 0x2a9, 0x1a3, 0xaa, 0x7a6, 0x6af, 0x5a5, 0x4ac,
	0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
	0x460, 0x569, 0x663, 0x76a, 0x66, 0x16f, 0x265, 0x36c,
	0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
	0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff, 0x3f5, 0x2fc,
	0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
	0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55, 0x15c,
	0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
	0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc,
	0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
	0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
	0xcc, 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
	0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
	0x15c, 0x55, 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
	0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
	0x2fc, 0x3f5, 0xff, 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
	0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
	0x36c, 0x265, 0x16f, 0x66, 0x76a, 0x663, 0x569, 0x460,
	0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
	0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa, 0x1a3, 0x2a9, 0x3a0,
	0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
	0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33, 0x339, 0x230,
	0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
	0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99, 0x190,
	0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
	0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0 };


const int triTable[256][16] =
{ { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1 },
{ 8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1 },
{ 3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1 },
{ 4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1 },
{ 4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1 },
{ 9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1 },
{ 10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1 },
{ 5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1 },
{ 5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1 },
{ 8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1 },
{ 2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1 },
{ 2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1 },
{ 11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1 },
{ 5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1 },
{ 11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1 },
{ 11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1 },
{ 2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1 },
{ 6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1 },
{ 3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1 },
{ 6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1 },
{ 6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1 },
{ 8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1 },
{ 7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1 },
{ 3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1 },
{ 0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1 },
{ 9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1 },
{ 8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1 },
{ 5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1 },
{ 0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1 },
{ 6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1 },
{ 10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1 },
{ 1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1 },
{ 0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1 },
{ 3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1 },
{ 6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1 },
{ 9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1 },
{ 8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1 },
{ 3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1 },
{ 10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1 },
{ 10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1 },
{ 2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1 },
{ 7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1 },
{ 2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1 },
{ 1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1 },
{ 11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1 },
{ 8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1 },
{ 0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1 },
{ 7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1 },
{ 7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1 },
{ 10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1 },
{ 0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1 },
{ 7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1 },
{ 6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1 },
{ 4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1 },
{ 10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1 },
{ 8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1 },
{ 1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1 },
{ 10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1 },
{ 10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1 },
{ 9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1 },
{ 7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1 },
{ 3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1 },
{ 7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1 },
{ 3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1 },
{ 6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1 },
{ 9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1 },
{ 1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1 },
{ 4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1 },
{ 7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1 },
{ 6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1 },
{ 0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1 },
{ 6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1 },
{ 0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1 },
{ 11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1 },
{ 6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1 },
{ 5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1 },
{ 9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1 },
{ 1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1 },
{ 10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1 },
{ 0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1 },
{ 11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1 },
{ 9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1 },
{ 7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1 },
{ 2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1 },
{ 9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1 },
{ 9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1 },
{ 1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1 },
{ 0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1 },
{ 10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1 },
{ 2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1 },
{ 0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1 },
{ 0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1 },
{ 9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1 },
{ 5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1 },
{ 5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1 },
{ 8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1 },
{ 9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1 },
{ 1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1 },
{ 3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1 },
{ 4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1 },
{ 9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1 },
{ 11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1 },
{ 11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1 },
{ 2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1 },
{ 9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1 },
{ 3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1 },
{ 1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1 },
{ 4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1 },
{ 0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1 },
{ 9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1 },
{ 1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ 0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
{ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 } };*/


VolumeRenderer::VolumeRenderer(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	model = new Model();
	ui.glwidget->model = model;
	ui.dicomviewer2dgl->model = model;
	interactive = true;
	connect(this, SIGNAL(progressValueChangedSignal(int)), this, SLOT(progressValueChangedSlot(int)));
	connect(this, SIGNAL(generatingFinishedSignal()), this, SLOT(generatingFinishedSlot()));
	connect(this, SIGNAL(dataFinishedReading()), ui.glwidget, SLOT(dataFinishedReading()));


	QFile File("stylesheet.qss");
	File.open(QFile::ReadOnly);
	QString StyleSheet = QLatin1String(File.readAll());

	qApp->setStyleSheet(StyleSheet);
	

}

VolumeRenderer::~VolumeRenderer()
{

}


/*In here we should have a vector of pixel datas. 
When the user selects a some dicom images, we clear the vector in case there is something from an posterior execution,
We then read each file and store the pixels data in the vector
If its only one file we check how many frames it has.
We then pass a reference to this vector of pixel data to the glWidget we created in the ui.
The widget will then have the pixel datas and also how many it can posibly display (the size of the vector)

Also the widget will probably need to know the representation,
i mean the width and height of the pixel data, the internal representation, uint16 or uint8 so it can know 
if it need to do a draw pixels with gl_unsigned_byte or gl_unsignet_short. We will use other set functions to do that

Things i need to resolve, i dont know if i want to do so many sets to the opengl widget because of the acoplamiento between them
Maybe i should just make an object called model and put everything there, and just pass a reference to that model object to the openglwidget,
In that case the widget can look in the model and see the height and width and everything else and in that case we only pass one thing as a reference
*/

void VolumeRenderer::on_test_clicked(){
	std::cout << "hola";
	/*DcmFileFormat fileformat;
	OFCondition status = fileformat.loadFile("E:\\Universidad\\Hecho por mi\\Volume Renderer\\cuda1\\MANIX\\MANIX\\MANIX\\CER-CT\\ANGIO CT\\IM-0001-0001.dcm");
	if (!status.good())
	{
		cerr << "Error: cannot read DICOM file (" << status.text() << ")" << endl;
		return;
	}


	OFString patientsName;
	fileformat.getDataset()->findAndGetOFString(DCM_PatientName, patientsName);
	cout << "Patient's Name: " << patientsName << endl;
	*/

	/*OFString rows;
	fileformat.getDataset()->findAndGetOFString(DCM_Rows, rows);
	cout << rows << endl;

	OFString columns;
	fileformat.getDataset()->findAndGetOFString(DCM_Columns, columns);
	cout << columns << endl;

	OFString Bits_stored;
	fileformat.getDataset()->findAndGetOFString(DCM_BitsStored, Bits_stored);
	cout << Bits_stored << endl;*/

	/*

	DicomImage *image = new DicomImage("E:\\Universidad\\Hecho por mi\\Volume Renderer\\cuda1\\MANIX\\MANIX\\MANIX\\CER-CT\\ANGIO CT\\IM-0001-0001.dcm");
	if (image != NULL)
	{
		if (image->getStatus() == EIS_Normal)
		{
			Uint16 *pixelData = (Uint16 *)(image->getOutputData(16 ));
			if (pixelData != NULL)
			{
				
				cout << "we got the pixels";
			}
		}
		else
			cerr << "Error: cannot load DICOM image (" << DicomImage::getString(image->getStatus()) << ")" << endl;
	}
	*/


	
	int nbFrame = 0;
	int bits_allocated = 8;
	//const char* filename = "E:\\Universidad\\Hecho por mi\\Volume Renderer\\Examples\\CT-MONO2-16-brain\\CT-MONO2-16-brain.dcm";
	const char* filename = "E:\\Universidad\\Hecho por mi\\Volume Renderer\\Examples\\MR-MONO2-8-16x-heart\\MR-MONO2-8-16x-heart";


	DcmFileFormat fileformat;
	OFCondition status = fileformat.loadFile(filename);

	DcmDataset *data = fileformat.getDataset();
	data->chooseRepresentation(EXS_LittleEndianImplicit, NULL); //Not really necesarry because the dicomImage class already does that
	DicomImage* img = new DicomImage(filename);	
	img->setMinMaxWindow();	//It is setting the adequate contrast and brightness
	Uint8* pixelData; 
	pixelData= (Uint8*)img->getOutputData(bits_allocated,0,0);
	
	
	// Do something with pixel data
	cout << "we got zem pixels!!";

	/*for (int i = 100; i < 5; i++){
		pixelData[i] = 0;
	}*/

	for (int i = 0; i < 256 ;i++)
	{
		cout << (int)pixelData[i] << " ";
	}
	
	cout << "we finished the pixels";
	
	
	
	GLFWwindow* window;
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(512, 512, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	while (!glfwWindowShouldClose(window))
	{
		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		//printf("%d, %d\n",width,height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		glDrawPixels(256, 256, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixelData);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

	



	free(pixelData);
	//DJLSDecoderRegistration::cleanup();

	


	
	
	//Sleep(100000);
	//system("PAUSE");
	return;
}

void VolumeRenderer::on_addDICOMFiles_clicked(){
	cout << "WE add dicom files" << endl;
	int bitsAllocated, bitsStored, highBit, pixelRepresentation,frames,height,width;
	void* pixelData=NULL;
	long insertIndex = 0;
	GLuint volumeTexture=0;

	QFileDialog dialog(this);
	//dialog.setDirectory(QDir::currentPath());
	dialog.setDirectory("E:\\Universidad\\Hecho por mi\\Volume Renderer\\Examples\\MR-MONO2-8-16x-heart\\"); 
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilter(tr("Files (*.*)"));
	QStringList fileNames;
	if (dialog.exec())
		fileNames = dialog.selectedFiles();

	//QString fileName;
	for (int i = 0; i < fileNames.size();i++){
		cout << "selected " << fileNames[i].toStdString() << "file " << endl;

		if (i == 0){
			getRepresentation(fileNames[i], bitsAllocated, bitsStored, highBit, pixelRepresentation, frames, height, width);
			if (bitsAllocated == 16){
				pixelData = static_cast<Uint16*>(pixelData);
				pixelData = (Uint16*)malloc(width*height*frames*sizeof(Uint16));
				for (int j = 0; j < width*height*frames*sizeof(Uint16); j++)
					((Uint16*)pixelData)[j] = -1;
			}
			if (bitsAllocated == 8){
				pixelData = static_cast<Uint8*>(pixelData);
				pixelData = (Uint8*)malloc(width*height*frames*sizeof(Uint8));
				for (int j = 0; j < width*height*frames*sizeof(Uint8); j++)
					((Uint8*)pixelData)[j] = 3;
			}
		}

		//we real pixel data like we did till now and then we copy that over to the pixelData array starting from insertIndex Then we update insert Index to be 256*256 more
		DcmFileFormat fileformat;
		OFCondition status = fileformat.loadFile(fileNames[i].toStdString().c_str());

		DcmDataset *data = fileformat.getDataset();
		data->chooseRepresentation(EXS_LittleEndianImplicit, NULL); //Not really necesarry because the dicomImage class already does that
		DicomImage* img = new DicomImage(fileNames[i].toStdString().c_str());
		img->setMinMaxWindow();	//It is setting the adequate contrast and brightness

		for (int j = 0; j < frames; j++){
			if (bitsAllocated == 8){
				Uint8* pixelDataOneFrame;
				pixelDataOneFrame = (Uint8*)img->getOutputData(bitsAllocated, j, 0);
				//Copy data from pixelDataOneFrame to pixelData

				/*for (int j = 0; j < width*height*sizeof(Uint8); j++)
					cout << " Pixel at one frame is" << (int)(pixelDataOneFrame[j]) <<endl;

				Sleep(100);*/

				for (int j = 0; j < width*height*sizeof(Uint8); j++)
					((Uint8*)pixelData)[j + insertIndex] = ((Uint8*)pixelDataOneFrame)[j];
				insertIndex += width*height;
			}
			if (bitsAllocated == 16){
				Uint16* pixelDataOneFrame;
				pixelDataOneFrame = (Uint16*)img->getOutputData(bitsAllocated, j, 0);
				for (int j = 0; j < width*height*sizeof(Uint16); j++)
					((Uint16*)pixelData)[j + insertIndex] = ((Uint16*)pixelDataOneFrame)[j];
				insertIndex += width*height;
			}
		}


	
	}

	
	cout << "shit is" << bitsAllocated << "   " << bitsStored << "  " << highBit << "  " << pixelRepresentation <<"frames"<< frames << "heidht " << height <<"Width" <<width << endl;
	//After all that we finally have our pixels so we print them
	for (int j = width*height*(frames-1); j < width*height*frames*sizeof(Uint8); j++)
		cout << "elemen " << j << "is" << (int) (((Uint8*)pixelData)[j]) << endl;


	cout << "Now we make the texture the size of uint8 is " <<sizeof(Uint8) <<endl;
	//Now we create the 3d texture, Maybe we pass the pixels and the dimenensions to a function that gives value to a Atribute From this object that we are in.
	glEnable(GL_TEXTURE_3D);
	glGenTextures(1, &volumeTexture);
	glBindTexture(GL_TEXTURE_3D, volumeTexture); 
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// download 3D volume texture for pre-classification
	glTexImage3D = (PFNGLTEXIMAGE3DPROC)wglGetProcAddress("glTexImage3D");	//We need this line to load the fuction because otherwise it will use the opengl 1.1 functions 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glTexImage3D(GL_TEXTURE_3D, 0,GL_INTENSITY8,width, height, frames,GL_COLOR_INDEX,GL_UNSIGNED_BYTE,0,pixelData );
	//glTexImage3D(GL_TEXTURE_3D, 0, GL_INTENSITY, width, height, frames, GL_COLOR_INDEX, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixelData);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE8, width, height, frames, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixelData);
	
	cout << "We finsishes making the texture" << endl;

	
	
	
	
}



int VolumeRenderer::getRepresentation(QString fileName,int& bitsAllocated, int& bitsStored,int& hightBit,int& pixelRepresentation,int& frames,int& height, int& width){
	DcmFileFormat fileformat;
	OFCondition status = fileformat.loadFile(fileName.toStdString().c_str());
	if (!status.good())
	{
		cerr << "Error: cannot read DICOM file (" << status.text() << ")" << endl;
		return 1;
	}


	long bitsAllocatedLong;
	long bitsStoredLong;
	long highBitLong;
	long pixelRepresentationLong;
	long framesLong;
	long heightLong;
	long widthLong;
	
	fileformat.getDataset()->findAndGetLongInt(DCM_BitsAllocated, bitsAllocatedLong);
	fileformat.getDataset()->findAndGetLongInt(DCM_BitsStored, bitsStoredLong);
	fileformat.getDataset()->findAndGetLongInt(DCM_HighBit, highBitLong);
	fileformat.getDataset()->findAndGetLongInt(DCM_PixelRepresentation, pixelRepresentationLong);
	fileformat.getDataset()->findAndGetLongInt(DCM_NumberOfFrames, framesLong);
	fileformat.getDataset()->findAndGetLongInt(DCM_Rows, heightLong);
	fileformat.getDataset()->findAndGetLongInt(DCM_Columns, widthLong);

	bitsAllocated = (int)bitsAllocatedLong;
	bitsStored = (int)bitsStoredLong;
	hightBit = (int)highBitLong;
	pixelRepresentation = (int)pixelRepresentationLong;
	frames = (int)framesLong;
	height = (int)heightLong;
	width = (int)widthLong;
	return 0;
}



void VolumeRenderer::load_image_data(const char* file) {
	

	//FILE *f = fopen("E:\\Universidad\\Hecho por mi\\Volume Renderer\\TFG\\VolumeRenderer\\Win32\\Debug\\angkor-grey.bmp", "rb");
	FILE *f = fopen(file, "rb");
	if (!f) {
		printf("failed to open file\n");
		exit(0);
	}

	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	//fread(signature, 1, 2, f);
	fread(&bfh, sizeof(BITMAPFILEHEADER), 1, f);
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, f);
	fseek(f, bfh.bfOffBits, SEEK_SET);	//The offbits is the byte where the pixels start so we move the file pointer there and we read from it

	model->frame_to_display = 0;
	ui.glwidget->setFrame(0);
	model->pixelDataWidth = bih.biWidth;
	model->pixelDataHeight = bih.biHeight;
	model->bitmapNumberOfPixels = bih.biWidth * bih.biHeight;
	model->bitmapImageSize = sizeof(char)* model->bitmapNumberOfPixels;
	model->numberOfBytes = 1;
	ui.isoLevelSlider->setMaximum(255);

	model->pixelData = (unsigned char**)malloc(1*sizeof(unsigned char*));
	model->pixelData[0] = (unsigned char *)malloc(model->bitmapImageSize);
	
	fread((model->pixelData)[0], 1, model->bitmapImageSize, f);


	//int pixel_value = 0;
	//int step = n_pixels / 255;
	//DEspite that we have this image here, we shall make a new one with values from0 to 255
	/*for (int i = 0; i < n_pixels; i++){
		if (i%step==0)
			pixel_value++;
		input_pixels[i] = pixel_value;
	}*/

	
	fclose(f);
}




int VolumeRenderer::polygonise(CELL & cell, std::vector<glm::vec3>& verts){

	//std::cout << "we are poligonizisng" << std::endl;
	//vector<Model::POINTF> points;
	glm::vec3 vertlist[12];
	
	int isoLevel = model->isoLevel;

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
	if (model->edgeTable[cubeIndex] & 1) {
		 interpolate(model->isoLevel, cell.position[0], cell.position[1], cell.val[0], cell.val[1], point);
		 vertlist[0] = point;
	}
	if (model->edgeTable[cubeIndex] & 2) {
		 interpolate(model->isoLevel, cell.position[1], cell.position[2], cell.val[1], cell.val[2] ,point);
		 vertlist[1] = point;
	}
	if (model->edgeTable[cubeIndex] & 4) {
		 interpolate(model->isoLevel, cell.position[2], cell.position[3], cell.val[2], cell.val[3], point);
		 vertlist[2] = point;
	}
	if (model->edgeTable[cubeIndex] & 8) {
		 interpolate(model->isoLevel, cell.position[3], cell.position[0], cell.val[3], cell.val[0], point);
		 vertlist[3] = point;

	}
	if (model->edgeTable[cubeIndex] & 16) {
		 interpolate(model->isoLevel, cell.position[4], cell.position[5], cell.val[4], cell.val[5], point);
		 vertlist[4] = point;
	}
	if (model->edgeTable[cubeIndex] & 32) {
		 interpolate(model->isoLevel, cell.position[5], cell.position[6], cell.val[5], cell.val[6], point);
		 vertlist[5] = point;
	}
	if (model->edgeTable[cubeIndex] & 64) {
		 interpolate(model->isoLevel, cell.position[6], cell.position[7], cell.val[6], cell.val[7], point);
		 vertlist[6] = point;
	}
	if (model->edgeTable[cubeIndex] & 128) {
		 interpolate(model->isoLevel, cell.position[7], cell.position[4], cell.val[7], cell.val[4], point);
		 vertlist[7] = point;
	}
	if (model->edgeTable[cubeIndex] & 256) {
		 interpolate(model->isoLevel, cell.position[0], cell.position[4], cell.val[0], cell.val[4], point);
		 vertlist[8] = point;
	}
	if (model->edgeTable[cubeIndex] & 512) {
		 interpolate(model->isoLevel, cell.position[1], cell.position[5], cell.val[1], cell.val[5], point);
		 vertlist[9] = point;
	}
	if (model->edgeTable[cubeIndex] & 1024) {
		 interpolate(model->isoLevel, cell.position[2], cell.position[6], cell.val[2], cell.val[6], point);
		 vertlist[10] = point;
	}
	if (model->edgeTable[cubeIndex] & 2048) {
		 interpolate(model->isoLevel, cell.position[3], cell.position[7], cell.val[3], cell.val[7], point);
		 vertlist[11] = point;
	}


	glm::vec3 normal;
	unsigned char* dataPointer;
	int pointerOffset = model->numberOfBytes;
	int value1 = 0, value2 = 0;

	/* Create the triangles */

	glm::vec3 point1, point2, point3;
	for (int i = 0; model->triTable[cubeIndex][i] != -1; i += 3) {
		
		point1.x = (vertlist[model->triTable[cubeIndex][i]]).x;
		point1.y = (vertlist[model->triTable[cubeIndex][i]]).y;
		point1.z = (vertlist[model->triTable[cubeIndex][i]]).z;

		point2.x = (vertlist[model->triTable[cubeIndex][i + 1]]).x;
		point2.y = (vertlist[model->triTable[cubeIndex][i + 1]]).y;
		point2.z = (vertlist[model->triTable[cubeIndex][i + 1]]).z;

		point3.x = (vertlist[model->triTable[cubeIndex][i + 2]]).x;
		point3.y = (vertlist[model->triTable[cubeIndex][i + 2]]).y;
		point3.z = (vertlist[model->triTable[cubeIndex][i + 2]]).z;

		
		verts.push_back(point1);
		verts.push_back(point2);
		verts.push_back(point3);

		/*verts.push_back((vertlist[model->triTable[cubeIndex][i]]).x);
		verts.push_back((vertlist[model->triTable[cubeIndex][i]]).y);
		verts.push_back((vertlist[model->triTable[cubeIndex][i]]).z);

		verts.push_back((vertlist[model->triTable[cubeIndex][i + 1]]).x);
		verts.push_back((vertlist[model->triTable[cubeIndex][i + 1]]).y);
		verts.push_back((vertlist[model->triTable[cubeIndex][i + 1]]).z);
		
		verts.push_back((vertlist[model->triTable[cubeIndex][i + 2]]).x);
		verts.push_back((vertlist[model->triTable[cubeIndex][i + 2]]).y);
		verts.push_back((vertlist[model->triTable[cubeIndex][i + 2]]).z);*/

		//Now we calculate the normal
		/*u = p2 - p1;
		2
			v = p3 - p1;
		3
			np.x = ((u.y * v.z) - (u.z * v.y));
		4
			np.y = ((u.z * v.x) - (u.x * v.z));
		5
			np.z = ((u.x * v.y) - (u.y * v.x));
		6
			DrawNormal(p1, p2, p3, np);
		7
			glNormal3f(np.x, np.y, np.z);


		Begin Function CalculateSurfaceNormal(Input Triangle) Returns Vector

			Set Vector U to(Triangle.p2 minus Triangle.p1)
			Set Vector V to(Triangle.p3 minus Triangle.p1)

			Set Normal.x to(multiply U.y by V.z) minus(multiply U.z by V.y)
			Set Normal.y to(multiply U.z by V.x) minus(multiply U.x by V.z)
			Set Normal.z to(multiply U.x by V.y) minus(multiply U.y by V.x)

			Returning Normal

			End Function*/

		/*glm::vec3 u,v,n;
		u.x = (vertlist[model->triTable[cubeIndex][i + 1]]).x - (vertlist[model->triTable[cubeIndex][i]]).x;
		u.y = (vertlist[model->triTable[cubeIndex][i + 1]]).y - (vertlist[model->triTable[cubeIndex][i]]).y;
		u.z = (vertlist[model->triTable[cubeIndex][i + 1]]).z - (vertlist[model->triTable[cubeIndex][i]]).z;

		v.x = (vertlist[model->triTable[cubeIndex][i + 2]]).x - (vertlist[model->triTable[cubeIndex][i]]).x;
		v.y = (vertlist[model->triTable[cubeIndex][i + 2]]).y - (vertlist[model->triTable[cubeIndex][i]]).y;
		v.z = (vertlist[model->triTable[cubeIndex][i + 2]]).z - (vertlist[model->triTable[cubeIndex][i]]).z;

		n.x = u.y*v.z - u.z*v.y;
		n.y = u.z*v.x - u.x*v.z;
		n.z = u.x*v.y - u.y*v.x;

		//we now normalize it
		

		double length = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);

		n.x = n.x / length;
		n.y = n.y / length;
		n.z = n.z / length;
		

		//WE push it once. Because we have 1 normal per triangle
		model->normals.push_back(n.x);
		model->normals.push_back(n.y);
		model->normals.push_back(n.z);*/

		//I will now try to do the same normal but with the sobel operator///
		///////////////////////////////////////////////////////////////////
		 
		/*double dx=0.0, dy=0.0, dz=0.0;
		int j, y, k;
		double length;

		j = boost::math::iround((vertlist[model->triTable[cubeIndex][i]]).x);
		y = boost::math::iround((vertlist[model->triTable[cubeIndex][i]]).y);
		k = boost::math::iround((vertlist[model->triTable[cubeIndex][i]]).z);

		dx = -1 * (model->getPixelValue(j - 1, y + 1, k - 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k - 1)) -
			2 * (model->getPixelValue(j - 1, y, k - 1)) + 2 * (model->getPixelValue(j + 1, y, k - 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k - 1)) + 1 * (model->getPixelValue(j + 1, y - 1, k - 1)) -

			2 * (model->getPixelValue(j - 1, y + 1, k)) + 2 * (model->getPixelValue(j + 1, y + 1, k)) -
			4 * (model->getPixelValue(j - 1, y, k)) + 2 * (model->getPixelValue(j + 1, y, k)) -
			2 * (model->getPixelValue(j - 1, y - 1, k)) + 2 * (model->getPixelValue(j + 1, y - 1, k)) -

			1 * (model->getPixelValue(j - 1, y + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k + 1)) -
			2 * (model->getPixelValue(j - 1, y, k + 1)) + 2 * (model->getPixelValue(j + 1, y, k + 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y - 1, k + 1));

		dy = 1 * (model->getPixelValue(j - 1, y + 1, k - 1)) + 2 * (model->getPixelValue(j, y + 1, k - 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k - 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k - 1)) - 2 * (model->getPixelValue(j, y - 1, k - 1)) - 1 * (model->getPixelValue(j + 1, y - 1, k - 1)) +

			2 * (model->getPixelValue(j - 1, y + 1, k)) + 4 * (model->getPixelValue(j, y + 1, k)) + 2 * (model->getPixelValue(j + 1, y + 1, k)) -
			2 * (model->getPixelValue(j - 1, y - 1, k)) - 4 * (model->getPixelValue(j, y - 1, k)) - 2 * (model->getPixelValue(j + 1, y - 1, k)) +

			1 * (model->getPixelValue(j - 1, y + 1, k + 1)) + 2 * (model->getPixelValue(j, y + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k + 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k + 1)) - 2 * (model->getPixelValue(j, y - 1, k + 1)) - 1 * (model->getPixelValue(j + 1, y - 1, k + 1));

		dz = -1 * (model->getPixelValue(j - 1, y + 1, k - 1)) - 2 * (model->getPixelValue(j, y + 1, k - 1)) - 1 * (model->getPixelValue(j + 1, y + 1, k - 1)) -
			2 * (model->getPixelValue(j - 1, y, k - 1)) - 4 * (model->getPixelValue(j, y, k - 1)) - 2 * (model->getPixelValue(j + 1, y, k - 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k - 1)) - 2 * (model->getPixelValue(j, y - 1, k - 1)) - 1 * (model->getPixelValue(j + 1, y - 1, k - 1)) +

			1 * (model->getPixelValue(j - 1, y + 1, k + 1)) + 2 * (model->getPixelValue(j, y + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k + 1)) -
			2 * (model->getPixelValue(j - 1, y, k + 1)) + 4 * (model->getPixelValue(j, y, k + 1)) + 2 * (model->getPixelValue(j + 1, y, k + 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k + 1)) + 2 * (model->getPixelValue(j, y - 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y - 1, k + 1));



		//we now normalize it


		length = sqrt(dx*dx + dy*dy + dz*dz);

		dx = dx / length;
		dy = dy / length;
		dz = dz / length;

		model->normals.push_back(dx);
		model->normals.push_back(dy);
		model->normals.push_back(dz);


		//Vertice 2
		j = boost::math::iround((vertlist[model->triTable[cubeIndex][i + 1]]).x);
		y = boost::math::iround((vertlist[model->triTable[cubeIndex][i + 1]]).y);
		k = boost::math::iround((vertlist[model->triTable[cubeIndex][i + 1]]).z);

		dx = -1 * (model->getPixelValue(j - 1, y + 1, k - 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k - 1)) -
			2 * (model->getPixelValue(j - 1, y, k - 1)) + 2 * (model->getPixelValue(j + 1, y, k - 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k - 1)) + 1 * (model->getPixelValue(j + 1, y - 1, k - 1)) -

			2 * (model->getPixelValue(j - 1, y + 1, k)) + 2 * (model->getPixelValue(j + 1, y + 1, k)) -
			4 * (model->getPixelValue(j - 1, y, k)) + 2 * (model->getPixelValue(j + 1, y, k)) -
			2 * (model->getPixelValue(j - 1, y - 1, k)) + 2 * (model->getPixelValue(j + 1, y - 1, k)) -

			1 * (model->getPixelValue(j - 1, y + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k + 1)) -
			2 * (model->getPixelValue(j - 1, y, k + 1)) + 2 * (model->getPixelValue(j + 1, y, k + 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y - 1, k + 1));

		dy = 1 * (model->getPixelValue(j - 1, y + 1, k - 1)) + 2 * (model->getPixelValue(j, y + 1, k - 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k - 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k - 1)) - 2 * (model->getPixelValue(j, y - 1, k - 1)) - 1 * (model->getPixelValue(j + 1, y - 1, k - 1)) +

			2 * (model->getPixelValue(j - 1, y + 1, k)) + 4 * (model->getPixelValue(j, y + 1, k)) + 2 * (model->getPixelValue(j + 1, y + 1, k)) -
			2 * (model->getPixelValue(j - 1, y - 1, k)) - 4 * (model->getPixelValue(j, y - 1, k)) - 2 * (model->getPixelValue(j + 1, y - 1, k)) +

			1 * (model->getPixelValue(j - 1, y + 1, k + 1)) + 2 * (model->getPixelValue(j, y + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k + 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k + 1)) - 2 * (model->getPixelValue(j, y - 1, k + 1)) - 1 * (model->getPixelValue(j + 1, y - 1, k + 1));

		dz = -1 * (model->getPixelValue(j - 1, y + 1, k - 1)) - 2 * (model->getPixelValue(j, y + 1, k - 1)) - 1 * (model->getPixelValue(j + 1, y + 1, k - 1)) -
			2 * (model->getPixelValue(j - 1, y, k - 1)) - 4 * (model->getPixelValue(j, y, k - 1)) - 2 * (model->getPixelValue(j + 1, y, k - 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k - 1)) - 2 * (model->getPixelValue(j, y - 1, k - 1)) - 1 * (model->getPixelValue(j + 1, y - 1, k - 1)) +

			1 * (model->getPixelValue(j - 1, y + 1, k + 1)) + 2 * (model->getPixelValue(j, y + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k + 1)) -
			2 * (model->getPixelValue(j - 1, y, k + 1)) + 4 * (model->getPixelValue(j, y, k + 1)) + 2 * (model->getPixelValue(j + 1, y, k + 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k + 1)) + 2 * (model->getPixelValue(j, y - 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y - 1, k + 1));



		//we now normalize it


		length = sqrt(dx*dx + dy*dy + dz*dz);

		dx = dx / length;
		dy = dy / length;
		dz = dz / length;

		model->normals.push_back(dx);
		model->normals.push_back(dy);
		model->normals.push_back(dz);


		//vertice 3
		j = boost::math::iround((vertlist[model->triTable[cubeIndex][i + 2]]).x);
		y = boost::math::iround((vertlist[model->triTable[cubeIndex][i + 2]]).y);
		k = boost::math::iround((vertlist[model->triTable[cubeIndex][i + 2]]).z);

		dx = -1 * (model->getPixelValue(j - 1, y + 1, k - 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k - 1)) -
			2 * (model->getPixelValue(j - 1, y, k - 1)) + 2 * (model->getPixelValue(j + 1, y, k - 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k - 1)) + 1 * (model->getPixelValue(j + 1, y - 1, k - 1)) -

			2 * (model->getPixelValue(j - 1, y + 1, k)) + 2 * (model->getPixelValue(j + 1, y + 1, k)) -
			4 * (model->getPixelValue(j - 1, y, k)) + 2 * (model->getPixelValue(j + 1, y, k)) -
			2 * (model->getPixelValue(j - 1, y - 1, k)) + 2 * (model->getPixelValue(j + 1, y - 1, k)) -

			1 * (model->getPixelValue(j - 1, y + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k + 1)) -
			2 * (model->getPixelValue(j - 1, y, k + 1)) + 2 * (model->getPixelValue(j + 1, y, k + 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y - 1, k + 1));

		dy = 1 * (model->getPixelValue(j - 1, y + 1, k - 1)) + 2 * (model->getPixelValue(j, y + 1, k - 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k - 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k - 1)) - 2 * (model->getPixelValue(j, y - 1, k - 1)) - 1 * (model->getPixelValue(j + 1, y - 1, k - 1)) +

			2 * (model->getPixelValue(j - 1, y + 1, k)) + 4 * (model->getPixelValue(j, y + 1, k)) + 2 * (model->getPixelValue(j + 1, y + 1, k)) -
			2 * (model->getPixelValue(j - 1, y - 1, k)) - 4 * (model->getPixelValue(j, y - 1, k)) - 2 * (model->getPixelValue(j + 1, y - 1, k)) +

			1 * (model->getPixelValue(j - 1, y + 1, k + 1)) + 2 * (model->getPixelValue(j, y + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k + 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k + 1)) - 2 * (model->getPixelValue(j, y - 1, k + 1)) - 1 * (model->getPixelValue(j + 1, y - 1, k + 1));

		dz = -1 * (model->getPixelValue(j - 1, y + 1, k - 1)) - 2 * (model->getPixelValue(j, y + 1, k - 1)) - 1 * (model->getPixelValue(j + 1, y + 1, k - 1)) -
			2 * (model->getPixelValue(j - 1, y, k - 1)) - 4 * (model->getPixelValue(j, y, k - 1)) - 2 * (model->getPixelValue(j + 1, y, k - 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k - 1)) - 2 * (model->getPixelValue(j, y - 1, k - 1)) - 1 * (model->getPixelValue(j + 1, y - 1, k - 1)) +

			1 * (model->getPixelValue(j - 1, y + 1, k + 1)) + 2 * (model->getPixelValue(j, y + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k + 1)) -
			2 * (model->getPixelValue(j - 1, y, k + 1)) + 4 * (model->getPixelValue(j, y, k + 1)) + 2 * (model->getPixelValue(j + 1, y, k + 1)) -
			1 * (model->getPixelValue(j - 1, y - 1, k + 1)) + 2 * (model->getPixelValue(j, y - 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y - 1, k + 1));



		//we now normalize it


		length = sqrt(dx*dx + dy*dy + dz*dz);

		dx = dx / length;
		dy = dy / length;
		dz = dz / length;

		model->normals.push_back(dx);
		model->normals.push_back(dy);
		model->normals.push_back(dz);*/

	}

	
	
	


	return 1;


	/*if ( cubeIndex == 255){	//The cube is totally in or out of the surface
		//cout << "cube is inside" << endl;
		//cout << "z value is" << cell.position[0].z << endl;
		model->verts.push_back(cell.position[0].x);
		model->verts.push_back(cell.position[0].y);
		model->verts.push_back(cell.position[0].z);

		model->verts.push_back(cell.position[1].x);
		model->verts.push_back(cell.position[1].y);
		model->verts.push_back(cell.position[1].z);

		model->verts.push_back(cell.position[2].x);
		model->verts.push_back(cell.position[2].y);
		model->verts.push_back(cell.position[2].z);

		model->verts.push_back(cell.position[3].x);
		model->verts.push_back(cell.position[3].y);
		model->verts.push_back(cell.position[3].z);

		model->verts.push_back(cell.position[4].x);
		model->verts.push_back(cell.position[4].y);
		model->verts.push_back(cell.position[4].z);

		model->verts.push_back(cell.position[7].x);
		model->verts.push_back(cell.position[7].y);
		model->verts.push_back(cell.position[7].z);
		return;
	}
	else{

		//cout << "we poly" << endl;
	}*/
		

	//cout << "cube index is" << cubeIndex << endl;
	//Now we have a cube of a certain index. Since we are doing the 2d version, we will just make a case statement for all the 16 cases
	//In each case we create a vector with the points generated and return that vector
	/*Model::POINTF point;
	switch (cubeIndex)
	{
	case 1:
		
		point = interpolate(isoLevel, cell.position[3], cell.position[0], cell.val[3], cell.val[0]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[1], cell.position[0], cell.val[1], cell.val[0]);
		points.push_back(point);

		break;
	case 2:

		point = interpolate(isoLevel, cell.position[1], cell.position[0], cell.val[1], cell.val[0]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[2], cell.position[1], cell.val[2], cell.val[1]);
		points.push_back(point);

		break;
	case 3:
		

		point = interpolate(isoLevel, cell.position[3], cell.position[0], cell.val[3], cell.val[0]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[2], cell.position[1], cell.val[2], cell.val[1]);
		points.push_back(point);

		break;
	case 4:
		
		point = interpolate(isoLevel, cell.position[2], cell.position[1], cell.val[2], cell.val[1]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[2], cell.position[3], cell.val[2], cell.val[3]);
		points.push_back(point);
		break;
	case 5:
		//Ambigous case
		point = interpolate(isoLevel, cell.position[1], cell.position[0], cell.val[1], cell.val[0]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[3], cell.position[0], cell.val[3], cell.val[0]);
		points.push_back(point);

		point = interpolate(isoLevel, cell.position[2], cell.position[1], cell.val[2], cell.val[1]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[2], cell.position[3], cell.val[2], cell.val[3]);
		points.push_back(point);
		//cout << "ambigous case " << endl;
		break;
	case 6:


		point = interpolate(isoLevel, cell.position[1], cell.position[0], cell.val[1], cell.val[0]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[2], cell.position[3], cell.val[2], cell.val[3]);
		points.push_back(point);

		break;
	case 7:
		point = interpolate(isoLevel, cell.position[3], cell.position[0], cell.val[3], cell.val[0]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[2], cell.position[3], cell.val[2], cell.val[3]);
		points.push_back(point);

		break;
	case 8:
		
		point = interpolate(isoLevel, cell.position[3], cell.position[0], cell.val[3], cell.val[0]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[2], cell.position[3], cell.val[2], cell.val[3]);
		points.push_back(point);

		break;
	case 9:
		
		point = interpolate(isoLevel, cell.position[1], cell.position[0], cell.val[1], cell.val[0]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[2], cell.position[3], cell.val[2], cell.val[3]);
		points.push_back(point);

		break;
	case 10:
		//Ambigous case
		point = interpolate(isoLevel, cell.position[1], cell.position[0], cell.val[1], cell.val[0]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[3], cell.position[0], cell.val[3], cell.val[0]);
		points.push_back(point);

		point = interpolate(isoLevel, cell.position[2], cell.position[1], cell.val[2], cell.val[1]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[2], cell.position[3], cell.val[2], cell.val[3]);
		points.push_back(point);
		//cout << "ambigous case " << endl;
		break;
	case 11:
		

		point = interpolate(isoLevel, cell.position[2], cell.position[1], cell.val[2], cell.val[1]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[2], cell.position[3], cell.val[2], cell.val[3]);
		points.push_back(point);

		break;
	case 12:

		point = interpolate(isoLevel,cell.position[3], cell.position[0],cell.val[3],cell.val[0]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[2], cell.position[1], cell.val[2], cell.val[1]);
		points.push_back(point);

		break;
	case 13:


		point = interpolate(isoLevel, cell.position[1], cell.position[0], cell.val[1], cell.val[0]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[2], cell.position[1], cell.val[2], cell.val[1]);
		points.push_back(point);

		break;
	case 14:

		point = interpolate(isoLevel, cell.position[1], cell.position[0], cell.val[1], cell.val[0]);
		points.push_back(point);
		point = interpolate(isoLevel, cell.position[3], cell.position[0], cell.val[3], cell.val[0]);
		points.push_back(point);

		break;
	default:
		break;
	}*/

	return 1; 
}


inline void VolumeRenderer::interpolate(int isoLevel, glm::vec3 point1, glm::vec3 point2, float val1, float val2, glm::vec3& resultPoint, int depth){
	

	if (depth == model->interpolateDepth){
		return;
	}

	//Model::POINTF point;
	//std::cout << "depth is " << depth << std::endl;

	if (model->linearInterpolation == false){

		resultPoint.x = (point1.x + point2.x) / 2.0;
		resultPoint.y = (point1.y + point2.y) / 2.0;
		resultPoint.z = (point1.z + point2.z) / 2.0;
	}
	else{

		float mu;
		if (abs(isoLevel - val1) < 0.00001){
			resultPoint = point1;
			return;
		}
			
		if (abs(isoLevel - val2) < 0.00001){
			resultPoint = point1;
			return;
		}
			
		mu = (isoLevel - val1) / (val2 - val1);
		resultPoint.x = point1.x + mu * (point2.x - point1.x);
		resultPoint.y = point1.y + mu * (point2.y - point1.y);
		resultPoint.z = point1.z + mu * (point2.z - point1.z);

		//Now we just do it again
		//We dedice witch one is the highest between point 1 and point2
		
		//The resulting point is inside the surface
		long valueOfResultingPoint = model->getPixelValue(boost::math::iround(resultPoint.x), boost::math::iround(resultPoint.y), boost::math::iround(resultPoint.z));
		if (valueOfResultingPoint > isoLevel){
			//We chose the lowest point and we use that to interpolate
			if (val1 < val2)
				interpolate(isoLevel, point1, resultPoint, val1, valueOfResultingPoint, resultPoint, depth+1);
			else
				interpolate(isoLevel, point2, resultPoint, val2, valueOfResultingPoint, resultPoint, depth + 1);
		}
		//The resulting point is outside the surface
		if (valueOfResultingPoint < isoLevel){
			if (val2 < val1)
				interpolate(isoLevel, point1, resultPoint, val1, valueOfResultingPoint, resultPoint, depth + 1);
			else
				interpolate(isoLevel, point2, resultPoint, val2, valueOfResultingPoint, resultPoint, depth + 1);
		}
		//The points is exactly on the surface
		
		


	}

	return;
}

/*void VolumeRenderer::marchingSquares(unsigned char** & pixelData, int numberOfByte, int celllSizeX, int celllSizeY, int celllSizeZ,
	int pixelDataHeight, int pixelDataWidth, int frames, int isoLevel, std::vector<glm::vec3>& verts, std::vector<glm::vec3>& normals)*/

void VolumeRenderer::marchingSquares(){
	model->verts.clear();
	model->normals.clear();
	std::cout << "marching cubes algorithm" << std::endl;
	const clock_t begin_time = clock();
	
	cout << "frame is " << model->frames << endl;

	//cout << "we march the cubes and write the points in the model" << endl;
	CELL cell;
	int cellSizeX, cellSizeY, cellSizeZ,frame;
	cellSizeX = model->cellSizeX;
	cellSizeY = model->cellSizeY;
	cellSizeZ = model->cellSizeZ;
	frame = model->frame_to_display;
	unsigned char* dataPointer;
	int value=0;
	int pointerOffset=model->numberOfBytes;

	std::ofstream outputFile;
	outputFile.open("PixelsValues.txt");

	//Now we make a square and march it
	//i is the y axis and j is the x axis
	//Since the bitmap is stored in reverse, we shall asign the y position to height-i insted of just i, thus mirroring the position
	//WE also put - CellsizeY insted of + because we consider the coordinated in the y acis as going from bottom to top insted of top to bottom like usual. Maybe in the dicom files you will change the y position to be only equals to i
	for (int i = 0; i < model->pixelDataHeight - cellSizeY; i = i + cellSizeY){	//WE make it to be till height -cellsizez because otherwise the last cube will be out of bound
		
		if (i%10==0)
			emit progressValueChangedSignal(i * 100 / model->pixelDataHeight);
		for (int j = 0; j < model->pixelDataWidth - cellSizeX; j = j + cellSizeX){
			for (int k = 0; k < model->frames - cellSizeZ; k = k + cellSizeZ){

				//cout << "k is" << k << endl;
				//if (model->getPixelValue(j, i, k)!=0)
				//cout << model->getPixelValue(j, i, k) << endl;


					
					/*
					   7---------6
					  /			 !
					 /			 !
					3---------2	 5
				    !         !	 
					!         !
					0---------1
					  */

					cell.position[0].x = j;
					cell.position[0].y = i;
					cell.position[0].z = k;
					cell.val[0] = model->getPixelValue(j,i,k);

					cell.position[1].x = j + cellSizeX;
					cell.position[1].y =  i;
					cell.position[1].z = k;
					cell.val[1] = model->getPixelValue(j +cellSizeX, i, k);

					cell.position[2].x = j + cellSizeX;
					cell.position[2].y =  i + cellSizeY;
					cell.position[2].z = k;
					cell.val[2] = model->getPixelValue(j + cellSizeX, i + cellSizeY, k);

					cell.position[3].x = j;
					cell.position[3].y = i + cellSizeY;
					cell.position[3].z = k;
					cell.val[3] = model->getPixelValue(j, i +cellSizeY, k);
					//////
					cell.position[4].x = j;
					cell.position[4].y =  i;
					cell.position[4].z = k+cellSizeZ;
					cell.val[4] = model->getPixelValue(j, i , k +cellSizeZ);

					cell.position[5].x = j + cellSizeX;
					cell.position[5].y =  i;
					cell.position[5].z = k + cellSizeZ;
					cell.val[5] = model->getPixelValue(j+ cellSizeX, i, k + cellSizeZ);

					cell.position[6].x = j + cellSizeX;
					cell.position[6].y =  i + cellSizeY;
					cell.position[6].z = k + cellSizeZ;
					cell.val[6] = model->getPixelValue(j + cellSizeX, i + cellSizeY, k + cellSizeZ);

					cell.position[7].x = j;
					cell.position[7].y =  i + cellSizeY;
					cell.position[7].z = k + cellSizeZ;
					cell.val[7] = model->getPixelValue(j, i + cellSizeY, k + cellSizeZ);

					

					//now we have that cell and we have to polygonise it
					//polygonise(cell, model->isoLevel, model->totalPoints);
					polygonise(cell, model->verts);
				
			}
		}
	}

	boost::thread workerThread(boost::bind(&VolumeRenderer::generateNormals, this));
	//model->generatingMesh = false;
	outputFile.close();

	//ui.glwidget->setFrame(5);
	//ui.glwidget->setDataToDisplay(output_pixels, model->totalPoints, width_image, height_image);

	//showPixels(output_pixels,totalPoints);
	std::cout << "Elapsed Time: "<< float(clock() - begin_time) / CLOCKS_PER_SEC << endl;

	//Now we write to obj file
	/*long numberOfVertsWritten = 0;
	ofstream myfile;
	myfile.open("feetWithSkin.obj");
	myfile << "Writing this to a file.\n";
	for (int i = 0; i < model->verts.size()-3; i=i+3){
		myfile << "v " << model->verts[i] << " " << model->verts[i + 1] << " " << model->verts[i + 2] << "\n";
		numberOfVertsWritten++;
		if (numberOfVertsWritten == 3){
			myfile << "f " << -3 << " " << -2 << " " << -1 << "\n\n";
			numberOfVertsWritten = 0;
		}
	}
	//for (int i = 0; i < model->verts.size() - 3; i = i + 3){
	//	myfile << "f " << i+1 << " " << i+2 << " " << i+3 << "\n";
	//}
	myfile.close(); */
}


OctreeCube VolumeRenderer::createInitialCube(){
	OctreeCube cube;
	cube.origin.x = 0;
	cube.origin.y = 0;
	cube.origin.z = 0;


	int cubeSize=std::max(std::max(model->pixelDataWidth, model->pixelDataHeight), model->frames - 1);	//WE get the maximum value
	cubeSize = pow(2, ceil(log(cubeSize) / log(2)));	// we round it up to the neerest power of 2

	cube.sizeX = cubeSize;
	cube.sizeY = cubeSize;
	cube.sizeZ = cubeSize;

	model->cubes.push_back(cube);
	return cube;
}


void VolumeRenderer::crackPatch(OctreeCube* root){
	

	OctreeCube* currentCube;
	OctreeCube* adyacent;

	OctreeCube* right, *left, *top, *bottom, *further, *closer;

	int count = 0;
	//Do the same but using the octree vector insted of a queue
	for (int i = 0; i < model->octreeVector.size(); i++){
		currentCube = model->octreeVector[i];
		count++;
		right = NULL;
		left = NULL;
		top = NULL;
		bottom = NULL;
		further = NULL;
		closer = NULL;

		if (currentCube->isLeaf && currentCube->containsVerts){
			//std::cout << "We need to check it's adyacents of cube "  << count << std::endl;
			//std::cout << "checking cube  " << count << "of " << model->octreeVector.size() << std::endl;
			for (int j = 0; j < model->octreeVector.size(); j++){



				//We check if it adyacent is the one on the right, left, top, bottom, further and closer cube
				adyacent = model->octreeVector[j];

				//Right cube
				if (right == NULL &&

					adyacent->origin.x == currentCube->origin.x + currentCube->sizeX &&
					adyacent->origin.y == currentCube->origin.y &&
					adyacent->origin.z == currentCube->origin.z &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					right = adyacent;
				}

				//Left cube
				if (left == NULL &&

					adyacent->origin.x == currentCube->origin.x - currentCube->sizeX &&
					adyacent->origin.y == currentCube->origin.y &&
					adyacent->origin.z == currentCube->origin.z &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					left = adyacent;
				}

				//Top cube
				if (top == NULL &&

					adyacent->origin.x == currentCube->origin.x &&
					adyacent->origin.y == currentCube->origin.y + currentCube->sizeY &&
					adyacent->origin.z == currentCube->origin.z &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					top = adyacent;
				}

				//bottom cube
				if (bottom == NULL &&

					adyacent->origin.x == currentCube->origin.x &&
					adyacent->origin.y == currentCube->origin.y - currentCube->sizeY &&
					adyacent->origin.z == currentCube->origin.z &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					bottom = adyacent;
				}

				//further cube
				if (further == NULL &&

					adyacent->origin.x == currentCube->origin.x &&
					adyacent->origin.y == currentCube->origin.y &&
					adyacent->origin.z == currentCube->origin.z + currentCube->sizeZ &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					further = adyacent;
				}


				//closer cube
				if (closer == NULL &&

					adyacent->origin.x == currentCube->origin.x &&
					adyacent->origin.y == currentCube->origin.y &&
					adyacent->origin.z == currentCube->origin.z - currentCube->sizeZ &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					closer = adyacent;
				}

				if (right != NULL && left != NULL && top != NULL && bottom != NULL && further != NULL && closer != NULL){
					break;	//WE stop searching because we found all the neihbours
				}


			}














			
			/*

			if (currentCube->origin.x == 256 && currentCube->origin.y == 168 && currentCube->origin.z == 56 && currentCube->sizeX == 8){
				std::cout << "found cube" << std::endl << std::endl;
				//Grab the points that are on the closer face
				//get rid of the duplicates

				//grab all the  points from the closer cube
				//remove those that are not on the face

				if (closer!=NULL)
					std::cout << "we have a closer cube" << std::endl;
				

				//we grab the points from the cube that are on the closer face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).z == (currentCube->origin.z)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				for (int i = 0; i < pointsOnFace.size(); i++){
					std::cout << "all point from currentcube on closer face " << pointsOnFace[i].x << "  " << pointsOnFace[i].y << "  " << pointsOnFace[i].z << std::endl;
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					skipPoint = false;
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}

				//The points from the current cube with no duplicates
				for (int i = 0; i < pointsOnFaceNoDuplicates.size(); i++){
					std::cout << "point from currentcube on closer face " << pointsOnFaceNoDuplicates[i].x << "  " << pointsOnFaceNoDuplicates[i].y << "  " << pointsOnFaceNoDuplicates[i].z << std::endl;
				}



				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(closer);
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
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								if (child->points->at(j).z == currentCube->origin.z){
									pointsOnFace2Old.push_back(child->points->at(j));
								}
							}
						}
					}


					std::cout << "the points from the closer cube on the face" << std::endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						std::cout << "point from closer cube on closer face " << pointsOnFace2Old[i].x << "  " << pointsOnFace2Old[i].y << "  " << pointsOnFace2Old[i].z << std::endl;
					}



					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					//pointsOnFace2New.reserve(pointsOnFace2Old.size());
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));

						//std::cout << "point 1 " << pointsOnFaceNoDuplicates[0].x << "  " << pointsOnFaceNoDuplicates[0].y << "  " << pointsOnFaceNoDuplicates[0].z << std::endl;
						//std::cout << "point 2 " << pointsOnFaceNoDuplicates[1].x << "  " << pointsOnFaceNoDuplicates[1].y << "  " << pointsOnFaceNoDuplicates[1].z << std::endl;
						//std::cout << "point for projection " << pointsOnFace2Old[i].x << "  " << pointsOnFace2Old[i].y << "  " << pointsOnFace2Old[i].z << std::endl;
						//std::cout << "point projected " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << std::endl;
						//std::cin.get();
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}


					std::cout  << "the points from the closer cube projected" << std::endl;
					for (int i = 0; i < pointsOnFace2New.size(); i++){
						std::cout << "point from closer cube projected " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << std::endl;
					}


					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					std::cout << std::endl << "interchanging old with the new" << std::endl;

					queue.push(closer);
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

										std::cout << "changing " << pointsOnFace2Old[k].x << "|" << pointsOnFace2Old[k].y << "|" << pointsOnFace2Old[k].z << "|" << " With " <<
											pointsOnFace2New[k].x << "|" << pointsOnFace2New[k].y << "|" << pointsOnFace2New[k].z << "|" << std::endl;


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

			*/
















			/*if (right != NULL && left != NULL && top != NULL && bottom != NULL && further != NULL && closer != NULL){
			std::cout << "found one with all the neighbours" << std::endl;
			}*/

			//Now we have the current cube and it's neighbour, we need to check and see if there are any conflicts woth the neighbours. A conflict occurs if the current cube is leaf and the neighbour isint

			//The right one is not a leaf, we must check it's chilcren and return their verts. If the children have any verts, check if they are on the correspondant face
			if (right != NULL && !right->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).x == (currentCube->origin.x + currentCube->sizeX)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					skipPoint = false;
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(right);
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
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).x != (currentCube->origin.x + currentCube->sizeX)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));

						//std::cout << "point 1 " << pointsOnFaceNoDuplicates[0].x << "  " << pointsOnFaceNoDuplicates[0].y << "  " << pointsOnFaceNoDuplicates[0].z << std::endl;
						//std::cout << "point 2 " << pointsOnFaceNoDuplicates[1].x << "  " << pointsOnFaceNoDuplicates[1].y << "  " << pointsOnFaceNoDuplicates[1].z << std::endl;
						//std::cout << "point for projection " << pointsOnFace2Old[i].x << "  " << pointsOnFace2Old[i].y << "  " << pointsOnFace2Old[i].z << std::endl;
						//std::cout << "point projected " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << std::endl;
						//std::cin.get();

						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(right);
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




				/*std::cout << "point on the right hace are " << pointsOnFaceNoDuplicates.size() << std::endl;
				if (pointsOnFaceNoDuplicates.size() >= 3){
				std::cout << "cube has sizes " << endl << " origin.x= " << currentCube->origin.x << endl <<
				" origin.y= " << currentCube->origin.y <<
				" origin.z= " << currentCube->origin.z <<
				" sizex= " << currentCube->sizeX <<
				" sizey= " << currentCube->sizeY <<
				" sizez= " << currentCube->sizeZ << endl;
				std::cout << "the vertices are" << endl;
				for (int j = 0; j < currentCube->points->size(); j++){
				std::cout << "  " << currentCube->points->at(j).x << "  " << currentCube->points->at(j).y << "  " << currentCube->points->at(j).z << endl;
				}
				std::cin.get();
				std::cout << endl << endl;
				}*/
			}

















			///////////////////////////////////////////////////////
			if (left != NULL && !left->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).x == (currentCube->origin.x)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					skipPoint = false;
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(left);
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
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).x != (currentCube->origin.x)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(left);
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



			if (top != NULL && !top->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).y == (currentCube->origin.y + currentCube->sizeY)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					skipPoint = false;
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(top);
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
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).y != (currentCube->origin.y + currentCube->sizeY)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(top);
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

			if (bottom != NULL && !bottom->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).y == (currentCube->origin.y)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					skipPoint = false;
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(bottom);
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
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).y != (currentCube->origin.y)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(bottom);
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


			if (further != NULL && !further->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).z == (currentCube->origin.z + currentCube->sizeZ)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					skipPoint = false;
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(further);
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
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).z != (currentCube->origin.z + currentCube->sizeZ)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(further);
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

			
			if (closer != NULL && !closer->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).z == (currentCube->origin.z)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					skipPoint = false;
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(closer);
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
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).z != (currentCube->origin.z)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					//pointsOnFace2New.reserve(pointsOnFace2Old.size());
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));

						//std::cout << "point 1 " << pointsOnFaceNoDuplicates[0].x << "  " << pointsOnFaceNoDuplicates[0].y << "  " << pointsOnFaceNoDuplicates[0].z << std::endl;
						//std::cout << "point 2 " << pointsOnFaceNoDuplicates[1].x << "  " << pointsOnFaceNoDuplicates[1].y << "  " << pointsOnFaceNoDuplicates[1].z << std::endl;
						//std::cout << "point for projection " << pointsOnFace2Old[i].x << "  " << pointsOnFace2Old[i].y << "  " << pointsOnFace2Old[i].z << std::endl;
						//std::cout << "point projected " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << std::endl;
						//std::cin.get();
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(closer);
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
	}






	//Go breath-first through the octree
	//For each cube, find its neibhours
	//check if thos eneigbours have a conflict, if there is a conflict then resolve it
	/*OctreeCube* currentCube;
	OctreeCube* adyacent;
	std::queue<OctreeCube*> queue;
	std::queue<OctreeCube*> queueInternal;
	int count = 0;

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

		std::cout << "iter number " << count << std::endl;
		count++;

		if (currentCube->children[0] != NULL)	//If the current cube is not a leaf one then it does not polygonons in it so it doesnt need checking  for conflicts with adyacent cubes
			continue;

		//For this current cube, make another loop so that it finds the nighours
		queueInternal.push(root);
		while (!queueInternal.empty()){
			adyacent = queueInternal.front();
			queueInternal.pop();
			if (adyacent->children[0] != NULL){
				queueInternal.push(adyacent->children[0]);
				queueInternal.push(adyacent->children[1]);
				queueInternal.push(adyacent->children[2]);
				queueInternal.push(adyacent->children[3]);
				queueInternal.push(adyacent->children[4]);
				queueInternal.push(adyacent->children[5]);
				queueInternal.push(adyacent->children[6]);
				queueInternal.push(adyacent->children[7]);
			}
		}

	}*/

	


	//THe code below is higly unoptimized because is On^6
	/*for (int cube = 0; cube < model->cubes.size(); cube++){
		if (model->cubes[cube].isLeaf && model->cubes[cube].needsChecking){
			// check the adyacent cubes, if there are not leaf that means that they are subdivided and thus the face that divides the two cubes is a transition face
			
			//we search for the cube that is on the right
			for (int ady = 0; ady < model->cubes.size(); ady++){
				if (model->cubes[ady].origin.x == (model->cubes[cube].origin.x + model->cubes[cube].sizeX) &&
					model->cubes[ady].origin.y == model->cubes[cube].origin.y &&
					model->cubes[ady].origin.z == model->cubes[cube].origin.z && 
					!model->cubes[ady].isLeaf){
					cout << "found a cube on the right that is not leaf" << endl;
				}
			}

			//we search for the cube that is on the left
			for (int ady = 0; ady < model->cubes.size(); ady++){
				if (model->cubes[ady].origin.x == (model->cubes[cube].origin.x - model->cubes[cube].sizeX) &&
					model->cubes[ady].origin.y == model->cubes[cube].origin.y &&
					model->cubes[ady].origin.z == model->cubes[cube].origin.z && 
					!model->cubes[ady].isLeaf){
					cout << "found a cube on the left that is not leaf" << endl;
				}
			}

			//we search for the cube that is toward the screen
			for (int ady = 0; ady < model->cubes.size(); ady++){
				if (model->cubes[ady].origin.x == model->cubes[cube].origin.x &&
					model->cubes[ady].origin.y == model->cubes[cube].origin.y &&
					model->cubes[ady].origin.z == (model->cubes[cube].origin.z + model->cubes[cube].sizeZ) &&
					!model->cubes[ady].isLeaf){
					cout << "found a cube on the z+ that is not leaf" << endl;
				}
			}

			//we search for the cube that is toward me
			for (int ady = 0; ady < model->cubes.size(); ady++){
				if (model->cubes[ady].origin.x == model->cubes[cube].origin.x &&
					model->cubes[ady].origin.y == model->cubes[cube].origin.y &&
					model->cubes[ady].origin.z == (model->cubes[cube].origin.z - model->cubes[cube].sizeZ) &&
					!model->cubes[ady].isLeaf){
					cout << "found a cube on the z- that is not leaf" << endl;
				}
			}

			//we search for the cube that is on top
			for (int ady = 0; ady < model->cubes.size(); ady++){
				if (model->cubes[ady].origin.x == model->cubes[cube].origin.x &&
					model->cubes[ady].origin.y == (model->cubes[cube].origin.y + model->cubes[cube].sizeY) &&
					model->cubes[ady].origin.z == model->cubes[cube].origin.z &&
					!model->cubes[ady].isLeaf){
					cout << "found a cube on the top that is not leaf" << endl;
				}
			}

			//we search for the cube that is on bottom
			for (int ady = 0; ady < model->cubes.size(); ady++){
				if (model->cubes[ady].origin.x == model->cubes[cube].origin.x &&
					model->cubes[ady].origin.y == (model->cubes[cube].origin.y - model->cubes[cube].sizeY) &&
					model->cubes[ady].origin.z == model->cubes[cube].origin.z &&
					!model->cubes[ady].isLeaf){
					cout << "found a cube on the bottom that is not leaf" << endl;
				}
			}


			cout << "Finished with that cube" << cube << " of " << model->cubes.size() << endl << endl;

		}

	}*/
}






void VolumeRenderer::crackPatch2(OctreeCube* root){


	OctreeCube* currentCube;
	OctreeCube* adyacent;

	OctreeCube* right, *left, *top, *bottom, *further, *closer;

	int count = 0;
	//Do the same but using the octree vector insted of a queue
	for (int i = 0; i < model->octreeVector.size(); i++){
		currentCube = model->octreeVector[i];

		right = NULL;
		left = NULL;
		top = NULL;
		bottom = NULL;
		further = NULL;
		closer = NULL;

		if (currentCube->isLeaf && currentCube->containsVerts){
			//std::cout << "We need to check it's adyacents of cube "  << count << std::endl;
			for (int j = 0; j < model->octreeVector.size(); j++){



				//We check if it adyacent is the one on the right, left, top, bottom, further and closer cube
				adyacent = model->octreeVector[j];

				//Right cube
				if (right == NULL &&

					adyacent->origin.x == currentCube->origin.x + currentCube->sizeX &&
					adyacent->origin.y == currentCube->origin.y &&
					adyacent->origin.z == currentCube->origin.z &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					right = adyacent;
				}

				//Left cube
				if (left == NULL &&

					adyacent->origin.x == currentCube->origin.x - currentCube->sizeX &&
					adyacent->origin.y == currentCube->origin.y &&
					adyacent->origin.z == currentCube->origin.z &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					left = adyacent;
				}

				//Top cube
				if (top == NULL &&

					adyacent->origin.x == currentCube->origin.x &&
					adyacent->origin.y == currentCube->origin.y + currentCube->sizeY &&
					adyacent->origin.z == currentCube->origin.z &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					top = adyacent;
				}

				//bottom cube
				if (bottom == NULL &&

					adyacent->origin.x == currentCube->origin.x &&
					adyacent->origin.y == currentCube->origin.y - currentCube->sizeY &&
					adyacent->origin.z == currentCube->origin.z &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					bottom = adyacent;
				}

				//further cube
				if (further == NULL &&

					adyacent->origin.x == currentCube->origin.x &&
					adyacent->origin.y == currentCube->origin.y &&
					adyacent->origin.z == currentCube->origin.z + currentCube->sizeZ &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					further = adyacent;
				}


				//closer cube
				if (closer == NULL &&

					adyacent->origin.x == currentCube->origin.x &&
					adyacent->origin.y == currentCube->origin.y &&
					adyacent->origin.z == currentCube->origin.z - currentCube->sizeZ &&

					adyacent->sizeX == currentCube->sizeX &&
					adyacent->sizeY == currentCube->sizeY &&
					adyacent->sizeZ == currentCube->sizeZ)
				{
					closer = adyacent;
				}


			}

			/*if (right != NULL && left != NULL && top != NULL && bottom != NULL && further != NULL && closer != NULL){
			std::cout << "found one with all the neighbours" << std::endl;
			}*/

			//Now we have the current cube and it's neighbour, we need to check and see if there are any conflicts woth the neighbours. A conflict occurs if the current cube is leaf and the neighbour isint

			//The right one is not a leaf, we must check it's chilcren and return their verts. If the children have any verts, check if they are on the correspondant face
			if (right != NULL && !right->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).x == (currentCube->origin.x + currentCube->sizeX)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(right);
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
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).x != (currentCube->origin.x + currentCube->sizeX)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));

						//std::cout << "point 1 " << pointsOnFaceNoDuplicates[0].x << "  " << pointsOnFaceNoDuplicates[0].y << "  " << pointsOnFaceNoDuplicates[0].z << std::endl;
						//std::cout << "point 2 " << pointsOnFaceNoDuplicates[1].x << "  " << pointsOnFaceNoDuplicates[1].y << "  " << pointsOnFaceNoDuplicates[1].z << std::endl;
						//std::cout << "point for projection " << pointsOnFace2Old[i].x << "  " << pointsOnFace2Old[i].y << "  " << pointsOnFace2Old[i].z << std::endl;
						//std::cout << "point projected " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << std::endl;
						//std::cin.get();

						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(right);
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




				/*std::cout << "point on the right hace are " << pointsOnFaceNoDuplicates.size() << std::endl;
				if (pointsOnFaceNoDuplicates.size() >= 3){
				std::cout << "cube has sizes " << endl << " origin.x= " << currentCube->origin.x << endl <<
				" origin.y= " << currentCube->origin.y <<
				" origin.z= " << currentCube->origin.z <<
				" sizex= " << currentCube->sizeX <<
				" sizey= " << currentCube->sizeY <<
				" sizez= " << currentCube->sizeZ << endl;
				std::cout << "the vertices are" << endl;
				for (int j = 0; j < currentCube->points->size(); j++){
				std::cout << "  " << currentCube->points->at(j).x << "  " << currentCube->points->at(j).y << "  " << currentCube->points->at(j).z << endl;
				}
				std::cin.get();
				std::cout << endl << endl;
				}*/
			}

















			///////////////////////////////////////////////////////
			if (left != NULL && !left->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).x == (currentCube->origin.x)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(left);
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
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).x != (currentCube->origin.x)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(left);
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



			if (top != NULL && !top->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).y == (currentCube->origin.y + currentCube->sizeY)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(top);
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
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).y != (currentCube->origin.y + currentCube->sizeY)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(top);
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

			if (bottom != NULL && !bottom->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).y == (currentCube->origin.y)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(bottom);
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
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).y != (currentCube->origin.y)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(bottom);
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


			if (further != NULL && !further->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).z == (currentCube->origin.z + currentCube->sizeZ)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(further);
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
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).z != (currentCube->origin.z + currentCube->sizeZ)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(further);
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


			if (closer != NULL && !closer->isLeaf){
				//std::cout << "possible conflict with the cube on the right" << std::endl;

				//we grab the points from the cube that are on the right face
				std::vector<glm::vec3> pointsOnFace;
				pointsOnFace.clear();
				for (int i = 0; i < currentCube->points->size(); i++){
					if (currentCube->points->at(i).z == (currentCube->origin.z)){
						pointsOnFace.push_back(currentCube->points->at(i));
					}
				}

				//Remove the duplicates from points on face
				std::vector<glm::vec3> pointsOnFaceNoDuplicates;
				bool skipPoint = false;
				for (int i = 0; i < pointsOnFace.size(); i++){
					//For each point check if it's not already in the pointOnFaceNoDuplicates
					for (int j = 0; j < pointsOnFaceNoDuplicates.size(); j++){
						if (pointsOnFace[i].x == pointsOnFaceNoDuplicates[j].x && pointsOnFace[i].y == pointsOnFaceNoDuplicates[j].y && pointsOnFace[i].z == pointsOnFaceNoDuplicates[j].z){
							//There is already on in pointface no duplicates with the same value
							skipPoint = true;
						}
					}
					if (!skipPoint)
						pointsOnFaceNoDuplicates.push_back(pointsOnFace[i]);
				}


				//After removing the duplicates , if we have lines (2 points) we continue with the algorithm
				if (pointsOnFaceNoDuplicates.size() == 2){
					//Now we get the points from the face on the cube on the right. WE establish an old vector for the points that we read and a new vector of points in witch we store the modified points
					std::vector<glm::vec3> pointsOnFace2Old;
					std::vector<glm::vec3> pointsOnFace2New;

					std::queue<OctreeCube*> queue;
					OctreeCube* child;
					queue.push(closer);
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
						//Get the points from that child and put them in pointsOnFace2Old
						if (child->points != NULL){
							for (int j = 0; j < child->points->size(); j++){
								pointsOnFace2Old.push_back(child->points->at(j));
							}
						}
					}

					//Now that we have the points from the cube on the right, we need to get only those which are on the face on the left (Right of current cube)
					vector< glm::vec3 >::iterator it = pointsOnFace2Old.begin();
					while (it != pointsOnFace2Old.end()) {

						if ((*it).z != (currentCube->origin.z)) {

							it = pointsOnFace2Old.erase(it);
						}
						else ++it;
					}

					//No we have the points from the face on the right, We iterate through every point and change the value to be the paralel projection of that point onto the two point from face1. The new point is stored in PointsonFace2New
					//std::cout << "we have points on the right face" << pointsOnFace2Old.size() << endl;
					//pointsOnFace2New.reserve(pointsOnFace2Old.size());
					for (int i = 0; i < pointsOnFace2Old.size(); i++){
						pointsOnFace2New.push_back(pointOnLine(pointsOnFaceNoDuplicates[0], pointsOnFaceNoDuplicates[1], pointsOnFace2Old[i]));

						//std::cout << "point 1 " << pointsOnFaceNoDuplicates[0].x << "  " << pointsOnFaceNoDuplicates[0].y << "  " << pointsOnFaceNoDuplicates[0].z << std::endl;
						//std::cout << "point 2 " << pointsOnFaceNoDuplicates[1].x << "  " << pointsOnFaceNoDuplicates[1].y << "  " << pointsOnFaceNoDuplicates[1].z << std::endl;
						//std::cout << "point for projection " << pointsOnFace2Old[i].x << "  " << pointsOnFace2Old[i].y << "  " << pointsOnFace2Old[i].z << std::endl;
						//std::cout << "point projected " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << std::endl;
						//std::cin.get();
						//std::cout << "  " << pointsOnFace2New[i].x << "  " << pointsOnFace2New[i].y << "  " << pointsOnFace2New[i].z << endl;
					}

					//We have all the points already changed in the pointsOnFace2New. Iteare throgh the children again and change their points

					queue.push(closer);
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
	}







	//Go breath-first through the octree
	//For each cube, find its neibhours
	//check if thos eneigbours have a conflict, if there is a conflict then resolve it
	/*OctreeCube* currentCube;
	OctreeCube* adyacent;
	std::queue<OctreeCube*> queue;
	std::queue<OctreeCube*> queueInternal;
	int count = 0;
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
	std::cout << "iter number " << count << std::endl;
	count++;
	if (currentCube->children[0] != NULL)	//If the current cube is not a leaf one then it does not polygonons in it so it doesnt need checking  for conflicts with adyacent cubes
	continue;
	//For this current cube, make another loop so that it finds the nighours
	queueInternal.push(root);
	while (!queueInternal.empty()){
	adyacent = queueInternal.front();
	queueInternal.pop();
	if (adyacent->children[0] != NULL){
	queueInternal.push(adyacent->children[0]);
	queueInternal.push(adyacent->children[1]);
	queueInternal.push(adyacent->children[2]);
	queueInternal.push(adyacent->children[3]);
	queueInternal.push(adyacent->children[4]);
	queueInternal.push(adyacent->children[5]);
	queueInternal.push(adyacent->children[6]);
	queueInternal.push(adyacent->children[7]);
	}
	}
	}*/




	//THe code below is higly unoptimized because is On^6
	/*for (int cube = 0; cube < model->cubes.size(); cube++){
	if (model->cubes[cube].isLeaf && model->cubes[cube].needsChecking){
	// check the adyacent cubes, if there are not leaf that means that they are subdivided and thus the face that divides the two cubes is a transition face

	//we search for the cube that is on the right
	for (int ady = 0; ady < model->cubes.size(); ady++){
	if (model->cubes[ady].origin.x == (model->cubes[cube].origin.x + model->cubes[cube].sizeX) &&
	model->cubes[ady].origin.y == model->cubes[cube].origin.y &&
	model->cubes[ady].origin.z == model->cubes[cube].origin.z &&
	!model->cubes[ady].isLeaf){
	cout << "found a cube on the right that is not leaf" << endl;
	}
	}
	//we search for the cube that is on the left
	for (int ady = 0; ady < model->cubes.size(); ady++){
	if (model->cubes[ady].origin.x == (model->cubes[cube].origin.x - model->cubes[cube].sizeX) &&
	model->cubes[ady].origin.y == model->cubes[cube].origin.y &&
	model->cubes[ady].origin.z == model->cubes[cube].origin.z &&
	!model->cubes[ady].isLeaf){
	cout << "found a cube on the left that is not leaf" << endl;
	}
	}
	//we search for the cube that is toward the screen
	for (int ady = 0; ady < model->cubes.size(); ady++){
	if (model->cubes[ady].origin.x == model->cubes[cube].origin.x &&
	model->cubes[ady].origin.y == model->cubes[cube].origin.y &&
	model->cubes[ady].origin.z == (model->cubes[cube].origin.z + model->cubes[cube].sizeZ) &&
	!model->cubes[ady].isLeaf){
	cout << "found a cube on the z+ that is not leaf" << endl;
	}
	}
	//we search for the cube that is toward me
	for (int ady = 0; ady < model->cubes.size(); ady++){
	if (model->cubes[ady].origin.x == model->cubes[cube].origin.x &&
	model->cubes[ady].origin.y == model->cubes[cube].origin.y &&
	model->cubes[ady].origin.z == (model->cubes[cube].origin.z - model->cubes[cube].sizeZ) &&
	!model->cubes[ady].isLeaf){
	cout << "found a cube on the z- that is not leaf" << endl;
	}
	}
	//we search for the cube that is on top
	for (int ady = 0; ady < model->cubes.size(); ady++){
	if (model->cubes[ady].origin.x == model->cubes[cube].origin.x &&
	model->cubes[ady].origin.y == (model->cubes[cube].origin.y + model->cubes[cube].sizeY) &&
	model->cubes[ady].origin.z == model->cubes[cube].origin.z &&
	!model->cubes[ady].isLeaf){
	cout << "found a cube on the top that is not leaf" << endl;
	}
	}
	//we search for the cube that is on bottom
	for (int ady = 0; ady < model->cubes.size(); ady++){
	if (model->cubes[ady].origin.x == model->cubes[cube].origin.x &&
	model->cubes[ady].origin.y == (model->cubes[cube].origin.y - model->cubes[cube].sizeY) &&
	model->cubes[ady].origin.z == model->cubes[cube].origin.z &&
	!model->cubes[ady].isLeaf){
	cout << "found a cube on the bottom that is not leaf" << endl;
	}
	}
	cout << "Finished with that cube" << cube << " of " << model->cubes.size() << endl << endl;
	}
	}*/
}



glm::vec3 VolumeRenderer::pointOnLine(glm::vec3 a, glm::vec3 b, glm::vec3& x){



	double dot = a.x*b.x + a.y*b.y + a.z*b.z;
	double lenSq1 = a.x*a.x + a.y*a.y + a.z*a.z;
	double lenSq2 = b.x*b.x + b.y*b.y + b.z*b.z;
	double angle = acos(dot / sqrt(lenSq1 * lenSq2));

	//std::cout << "the angles is " << angle << std::endl;


	glm::vec3 x_b, a_b ,a_bDiv, projection;
	float x_b_a_bDot, a_bModule, a_bModulePow, div, x_bModule,optional;

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

	/*if (angle < 0.001){
		std:cout << "angle is too small" << std::endl;
		return x;
	}*/


	///////End of optional


	x_b_a_bDot = x_b.x*a_b.x + x_b.y*a_b.y + x_b.z*a_b.z;
	a_bModule = sqrt(a_b.x*a_b.x + a_b.y*a_b.y + a_b.z*a_b.z);
	a_bModulePow = a_bModule*a_bModule;

	//div = optional / a_bModulePow;
	div = x_b_a_bDot / a_bModulePow;

	a_bDiv.x = a_b.x*div;
	a_bDiv.y = a_b.y*div;
	a_bDiv.z = a_b.z*div;

	projection.x = b.x + a_bDiv.x;
	projection.y = b.y + a_bDiv.y;
	projection.z = b.z + a_bDiv.z;

	return projection;

	//return x;

	/*glm::vec3 u;
	glm::vec3 projection;
	u.x = pt2.x - pt1.x;
	u.y = pt2.y - pt1.y;
	u.z = pt2.z - pt1.z;

	//Proy de pt sobre u

	double e_u, u2, u_magnitude, div, u_magnitude_pow;
	e_u = pt.x*u.x + pt.y*u.y + pt.z*u.z;
	u2 = u.x*u.x + u.y*u.y + u.z*u.z;
	u_magnitude = sqrt(u.x*u.x + u.y*u.y + u.z*u.z);
	u_magnitude_pow = u_magnitude*u_magnitude;
	div = e_u / u_magnitude_pow;

	std::cout << "u is " << u.x  << " " << u.y <<  "  " << u.z << std::endl;
	std::cout << "div is " << div << std::endl;

	projection.x = div*u.x;
	projection.y = div*u.y;
	projection.z = div*u.z;

	//projection.x = projection.x+ pt1.x;
	//projection.y = projection.y + pt1.y;
	//projection.z = projection.z + pt1.z;

	return projection;*/

	/*bool isValid = false;

	//var r = new Point(0, 0);
	glm::vec3 r(0, 0, 0);

	//if (pt1.Y == pt2.Y && pt1.X == pt2.X) { pt1.Y -= 0.00001; }

	int U = ((pt.y - pt1.y) * (pt2.y - pt1.y)) + ((pt.x - pt1.x) * (pt2.x - pt1.x)) + ((pt.z - pt1.z) * (pt2.z - pt1.z));

	int Udenom = pow(pt2.y - pt1.y, 2) + pow(pt2.x - pt1.x, 2) + pow(pt2.z - pt1.z, 2);


	if (Udenom == 0)
		return pt;
	U /= Udenom;

	r.y = pt1.y + (U * (pt2.y - pt1.y));
	r.x = pt1.x + (U * (pt2.x - pt1.x));
	r.z = pt1.z + (U * (pt2.z - pt1.z));

	double minx, maxx, miny, maxy , minz, maxz;

	minx = std::min(pt1.y, pt2.y);
	maxx = std::max(pt1.y, pt2.y);

	miny = std::min(pt1.x, pt2.x);
	maxy = std::max(pt1.x, pt2.x);

	minz = std::min(pt1.z, pt2.z);
	maxz = std::max(pt1.z, pt2.z);

	isValid = (r.y >= minx && r.y <= maxx) && (r.x >= miny && r.x <= maxy) && (r.z >= minz && r.z <= maxz);

	//return isValid ? r : new Point();

	return r;*/
} //-See more at : http ://www.dietabaiamonte.info/197386.html#sthash.WtI0J8q1.dpuf


int VolumeRenderer::adaptiveMarchingCubes(){

	model->verts.clear();
	model->normals.clear();
	std::cout << "Starting adaptive marching cubes" << std::endl;
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

	
	int cubesSize = 0, depth = 0, maxDepth = model->octreeMaxDepth;

	

	createInitialCube();


	if (model->gradient.size()==0 || model->gradient.empty())
		calculateGradient();
	cout << "Finished calculating gradients" << endl;


	while (1){
		cubesSize = model->cubes.size();
		for (int cube = 0; cube < cubesSize; cube++){
			if (model->cubes[cube].isLeaf && model->cubes[cube].needsChecking){

				if (cubeNeedsSubdivision(model->cubes[cube])){
					model->cubes[cube].isLeaf = false;
					model->cubes[cube].subdivide(model->cubes);
				}

			}

		}
		depth++;
		if (depth == maxDepth){
			depth = 0;
			break;
		}
	}
	std:cout << "finished creating the octree" << std::endl;


	std::cout << "number of cubes is" << model->cubes.size() << std::endl;
	model->verts.clear();
	model->normals.clear();

	for (int i = 0; i < model->cubes.size(); i++){
		if (model->cubes[i].isLeaf){
			CELL cell;


			/*
			7---------6
			/			 !
			/			 !
			3---------2	 5
			!         !
			!         !
			0---------1
			*/

			for (int j = 0; j < 8; j++){
				cell.position[j].x = 0;
				cell.position[j].x = 0;
				cell.position[j].x = 0;
				cell.val[j] = 0;
			}

			cell.position[0].x = model->cubes[i].origin.x;
			cell.position[0].y = model->cubes[i].origin.y;
			cell.position[0].z = model->cubes[i].origin.z;
			cell.val[0] = model->getPixelValue(model->cubes[i].origin.x, model->cubes[i].origin.y, model->cubes[i].origin.z);

			cell.position[1].x = model->cubes[i].origin.x + model->cubes[i].sizeX;
			cell.position[1].y = model->cubes[i].origin.y;
			cell.position[1].z = model->cubes[i].origin.z;
			cell.val[1] = model->getPixelValue(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y, model->cubes[i].origin.z);

			cell.position[2].x = model->cubes[i].origin.x + model->cubes[i].sizeX;
			cell.position[2].y = model->cubes[i].origin.y + model->cubes[i].sizeY;
			cell.position[2].z = model->cubes[i].origin.z;
			cell.val[2] = model->getPixelValue(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z);

			cell.position[3].x = model->cubes[i].origin.x;
			cell.position[3].y = model->cubes[i].origin.y + model->cubes[i].sizeY;
			cell.position[3].z = model->cubes[i].origin.z;
			cell.val[3] = model->getPixelValue(model->cubes[i].origin.x, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z);
			//////
			cell.position[4].x = model->cubes[i].origin.x;
			cell.position[4].y = model->cubes[i].origin.y;
			cell.position[4].z = model->cubes[i].origin.z + model->cubes[i].sizeZ;
			cell.val[4] = model->getPixelValue(model->cubes[i].origin.x, model->cubes[i].origin.y, model->cubes[i].origin.z + model->cubes[i].sizeZ);

			cell.position[5].x = model->cubes[i].origin.x + model->cubes[i].sizeX;
			cell.position[5].y = model->cubes[i].origin.y;
			cell.position[5].z = model->cubes[i].origin.z + model->cubes[i].sizeZ;
			cell.val[5] = model->getPixelValue(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y, model->cubes[i].origin.z + model->cubes[i].sizeZ);

			cell.position[6].x = model->cubes[i].origin.x + model->cubes[i].sizeX;
			cell.position[6].y = model->cubes[i].origin.y + model->cubes[i].sizeY;
			cell.position[6].z = model->cubes[i].origin.z + model->cubes[i].sizeZ;
			cell.val[6] = model->getPixelValue(model->cubes[i].origin.x + model->cubes[i].sizeX, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z + model->cubes[i].sizeZ);

			cell.position[7].x = model->cubes[i].origin.x;
			cell.position[7].y = model->cubes[i].origin.y + model->cubes[i].sizeY;
			cell.position[7].z = model->cubes[i].origin.z + model->cubes[i].sizeZ;
			cell.val[7] = model->getPixelValue(model->cubes[i].origin.x, model->cubes[i].origin.y + model->cubes[i].sizeY, model->cubes[i].origin.z + model->cubes[i].sizeZ);

			//std::cout << "cell has values" << cell.val[0] << " " << cell.val[1] << " " << cell.val[2] << " " << cell.val[3] << " " << cell.val[4] << " " << cell.val[5] << " " << cell.val[6] << " " << cell.val[7] << std::endl;

			//now we have that cell and we have to polygonise it
			//polygonise(cell, model->isoLevel, model->totalPoints);
			polygonise(cell, model->verts);
		}
	}

	/*std::cout << "Printing verts" <<  std::endl;
	for (int i = 0; i < model->verts.size(); i++){
		std::cout << "Verts is" << model->verts[i] << std::endl;
	}*/



	/*I will now at attempt at making a recurrsive octree generator. When the maximum depth leves or reached or the cube no longer needs subdivision, we polygonise it*/

	//crackPatch();
	
	
	boost::thread workerThread(boost::bind(&VolumeRenderer::generateNormals, this));

	
	return 0;
}

int VolumeRenderer::generateOctree(OctreeCube currentCube, int currentDepth){
	if (currentDepth == model->octreeMaxDepth || !cubeNeedsSubdivision(currentCube)){
		//WE polyonise this cube
		CELL cell;


		/*
		7---------6
		/			 !
		/			 !
		3---------2	 5
		!         !
		!         !
		0---------1
		*/

		for (int j = 0; j < 8; j++){
			cell.position[j].x = 0;
			cell.position[j].x = 0;
			cell.position[j].x = 0;
			cell.val[j] = 0;
		}

		cell.position[0].x = currentCube.origin.x;
		cell.position[0].y = currentCube.origin.y;
		cell.position[0].z = currentCube.origin.z;
		cell.val[0] = model->getPixelValue(currentCube.origin.x, currentCube.origin.y, currentCube.origin.z);

		cell.position[1].x = currentCube.origin.x + currentCube.sizeX;
		cell.position[1].y = currentCube.origin.y;
		cell.position[1].z = currentCube.origin.z;
		cell.val[1] = model->getPixelValue(currentCube.origin.x + currentCube.sizeX, currentCube.origin.y, currentCube.origin.z);

		cell.position[2].x = currentCube.origin.x + currentCube.sizeX;
		cell.position[2].y = currentCube.origin.y + currentCube.sizeY;
		cell.position[2].z = currentCube.origin.z;
		cell.val[2] = model->getPixelValue(currentCube.origin.x + currentCube.sizeX, currentCube.origin.y + currentCube.sizeY, currentCube.origin.z);

		cell.position[3].x = currentCube.origin.x;
		cell.position[3].y = currentCube.origin.y + currentCube.sizeY;
		cell.position[3].z = currentCube.origin.z;
		cell.val[3] = model->getPixelValue(currentCube.origin.x, currentCube.origin.y + currentCube.sizeY, currentCube.origin.z);
		//////
		cell.position[4].x = currentCube.origin.x;
		cell.position[4].y = currentCube.origin.y;
		cell.position[4].z = currentCube.origin.z + currentCube.sizeZ;
		cell.val[4] = model->getPixelValue(currentCube.origin.x, currentCube.origin.y, currentCube.origin.z + currentCube.sizeZ);

		cell.position[5].x = currentCube.origin.x + currentCube.sizeX;
		cell.position[5].y = currentCube.origin.y;
		cell.position[5].z = currentCube.origin.z + currentCube.sizeZ;
		cell.val[5] = model->getPixelValue(currentCube.origin.x + currentCube.sizeX, currentCube.origin.y, currentCube.origin.z + currentCube.sizeZ);

		cell.position[6].x = currentCube.origin.x + currentCube.sizeX;
		cell.position[6].y = currentCube.origin.y + currentCube.sizeY;
		cell.position[6].z = currentCube.origin.z + currentCube.sizeZ;
		cell.val[6] = model->getPixelValue(currentCube.origin.x + currentCube.sizeX, currentCube.origin.y + currentCube.sizeY, currentCube.origin.z + currentCube.sizeZ);

		cell.position[7].x = currentCube.origin.x;
		cell.position[7].y = currentCube.origin.y + currentCube.sizeY;
		cell.position[7].z = currentCube.origin.z + currentCube.sizeZ;
		cell.val[7] = model->getPixelValue(currentCube.origin.x, currentCube.origin.y + currentCube.sizeY, currentCube.origin.z + currentCube.sizeZ);

		//std::cout << "cell has values" << cell.val[0] << " " << cell.val[1] << " " << cell.val[2] << " " << cell.val[3] << " " << cell.val[4] << " " << cell.val[5] << " " << cell.val[6] << " " << cell.val[7] << std::endl;

		//now we have that cell and we have to polygonise it
		//polygonise(cell, model->isoLevel, model->totalPoints);
		polygonise(cell, model->verts);
		//model->cubes.push_back(currentCube);
		return 0 ;
	}



	
		OctreeCube subdividedCube;

		subdividedCube.sizeX = currentCube.sizeX / 2;
		subdividedCube.sizeY = currentCube.sizeY / 2;
		subdividedCube.sizeZ = currentCube.sizeZ / 2;
		
		//Cube 1
		subdividedCube.origin.x = currentCube.origin.x;
		subdividedCube.origin.y = currentCube.origin.y + currentCube.sizeY / 2;
		subdividedCube.origin.z = currentCube.origin.z;
		generateOctree(subdividedCube, currentDepth+1);


		//Cube 2
		subdividedCube.origin.x = currentCube.origin.x + currentCube.sizeX / 2;
		subdividedCube.origin.y = currentCube.origin.y + currentCube.sizeY / 2;
		subdividedCube.origin.z = currentCube.origin.z;
		generateOctree(subdividedCube, currentDepth + 1);


		//Cube 3
		subdividedCube.origin.x = currentCube.origin.x + currentCube.sizeX / 2;
		subdividedCube.origin.y = currentCube.origin.y;
		subdividedCube.origin.z = currentCube.origin.z;
		generateOctree(subdividedCube, currentDepth + 1);

		//Cube 4
		subdividedCube.origin.x = currentCube.origin.x;
		subdividedCube.origin.y = currentCube.origin.y;
		subdividedCube.origin.z = currentCube.origin.z;
		generateOctree(subdividedCube, currentDepth + 1);

		//Cube 5
		subdividedCube.origin.x = currentCube.origin.x;
		subdividedCube.origin.y = currentCube.origin.y + currentCube.sizeY / 2;
		subdividedCube.origin.z = currentCube.origin.z + currentCube.sizeZ / 2;
		generateOctree(subdividedCube, currentDepth + 1);


		//Cube 6
		subdividedCube.origin.x = currentCube.origin.x + currentCube.sizeX / 2;
		subdividedCube.origin.y = currentCube.origin.y + currentCube.sizeY / 2;
		subdividedCube.origin.z = currentCube.origin.z + currentCube.sizeZ / 2;
		generateOctree(subdividedCube, currentDepth + 1);

		//Cube 7
		subdividedCube.origin.x = currentCube.origin.x + currentCube.sizeX / 2;
		subdividedCube.origin.y = currentCube.origin.y;
		subdividedCube.origin.z = currentCube.origin.z + currentCube.sizeZ / 2;
		generateOctree(subdividedCube, currentDepth + 1);

		//Cube 8
		subdividedCube.origin.x = currentCube.origin.x;
		subdividedCube.origin.y = currentCube.origin.y;
		subdividedCube.origin.z = currentCube.origin.z + currentCube.sizeZ / 2;
		generateOctree(subdividedCube, currentDepth + 1);
	
	
	return 0;
}

int VolumeRenderer::adaptiveMarchingCubes2(){

	model->verts.clear();
	model->normals.clear();

	if (model->gradient.size() == 0 || model->gradient.empty())
		calculateGradient();
	cout << "Finished calculating gradients" << endl;
	cout << "Gradient points has "  << model->gradientPoints.size() << "elements"<< endl;


	createInitialCube();

	generateOctree(model->cubes[0]);
	boost::thread workerThread(boost::bind(&VolumeRenderer::generateNormals, this));
	return 0;
}

void VolumeRenderer::calculateGradient(){


	//Sobel kernel on page 15 of http://www.diva-portal.se/smash/get/diva2:515510/FULLTEXT01.pdf

	unsigned char* dataPointer;
	int value = 0;
	int pointerOffset = model->numberOfBytes;
	int dx = 0, dy = 0, dz=0, magnitude = 0, left = 0, right = 0, top = 0, bottom = 0, closev=0, farv = 0, center=0;
	float angle = -1;
	glm::vec3  gradientAtPoint;


	//std::ofstream outputFile;
	//outputFile.open("GradientAtFrame100.txt");

	/*if (!model->gradient.empty())
		return;*/


	//Create as many gradients frames as the number of frames in the dicom file
	model->gradient.resize(model->frames);
	if (model->gradient.size()==0)
		return;

	
	
	

	//Here i is y axis and j is the x axis
	for (int i = 0; i < model->pixelDataHeight; i = i + 1){
		if (i % 10 == 0)
			emit progressValueChangedSignal(i * 100 / model->pixelDataHeight);
		for (int j = 0; j < model->pixelDataWidth; j = j + 1){
			for (int k = 0; k < model->frames; k = k + 1){			//You need to check why it doesnt work without the -1 in the frames

				if (i == 0 || i == model->pixelDataHeight-1 || j == 0 || j == model->pixelDataWidth-1 || k==0 || k==model->frames-1){		//If we are in a border we just put it at 0
					//model->gradient[k].push_back(glm::vec3(-1,-1,-1));
					//model->gradient[k].insert((j,i), (-1, -1, -1));
					continue;
				}

				//std::cout << "values are" << i  << "  "<< j << "  " << k  << std::endl;




				//Now we take the values
				dataPointer = &(model->pixelData[k][0]);
				dataPointer = dataPointer + (j + i*model->pixelDataWidth)*pointerOffset;
				memcpy(&center, dataPointer, pointerOffset);

				/*if (abs(center - model->isoLevel)>500)
					continue;*/


				dataPointer = &(model->pixelData[k][0]);
				dataPointer = dataPointer + (j - 1 + i*model->pixelDataWidth)*pointerOffset;
				memcpy(&left, dataPointer, pointerOffset);

				dataPointer = &(model->pixelData[k][0]);
				dataPointer = dataPointer + (j + 1 + i*model->pixelDataWidth)*pointerOffset;
				memcpy(&right, dataPointer, pointerOffset);

				dataPointer = &(model->pixelData[k][0]);
				dataPointer = dataPointer + (j + (i - 1)*model->pixelDataWidth)*pointerOffset;
				memcpy(&top, dataPointer, pointerOffset);

				dataPointer = &(model->pixelData[k][0]);
				dataPointer = dataPointer + (j + (i + 1)*model->pixelDataWidth)*pointerOffset;
				memcpy(&bottom, dataPointer, pointerOffset);

				dataPointer = &(model->pixelData[k-1][0]);
				dataPointer = dataPointer + (j + i*model->pixelDataWidth)*pointerOffset;
				memcpy(&closev, dataPointer, pointerOffset);

				dataPointer = &(model->pixelData[k+1][0]);
				dataPointer = dataPointer + (j + i*model->pixelDataWidth)*pointerOffset;
				memcpy(&farv, dataPointer, pointerOffset);

				
				if (center < model->isoLevel && left < model->isoLevel && right < model->isoLevel &&top < model->isoLevel && bottom < model->isoLevel&& farv < model->isoLevel&&closev < model->isoLevel)
					continue;
				if (center > model->isoLevel && left > model->isoLevel && right > model->isoLevel &&top > model->isoLevel && bottom > model->isoLevel&& farv > model->isoLevel&&closev > model->isoLevel)
					continue;
				/*dy = left - right;
				dx = top - bottom;
				dz = closev - farv;*/

				dx = -1 * (model->getPixelValue(j - 1, i + 1, k - 1)) + 1 * (model->getPixelValue(j + 1, i + 1, k - 1)) -
					2 * (model->getPixelValue(j - 1, i, k - 1)) + 2 * (model->getPixelValue(j + 1, i, k - 1)) -
					1 * (model->getPixelValue(j - 1, i - 1, k - 1)) + 1 * (model->getPixelValue(j + 1, i - 1, k - 1)) -

					2 * (model->getPixelValue(j - 1, i + 1, k)) + 2 * (model->getPixelValue(j + 1, i + 1, k)) -
					4 * (model->getPixelValue(j - 1, i, k)) + 2 * (model->getPixelValue(j + 1, i, k)) -
					2 * (model->getPixelValue(j - 1, i - 1, k)) + 2 * (model->getPixelValue(j + 1, i - 1, k)) -

					1 * (model->getPixelValue(j - 1, i + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, i + 1, k + 1)) -
					2 * (model->getPixelValue(j - 1, i, k + 1)) + 2 * (model->getPixelValue(j + 1, i, k + 1)) -
					1 * (model->getPixelValue(j - 1, i - 1, k + 1)) + 1 * (model->getPixelValue(j + 1, i - 1, k + 1));

				dy = 1 * (model->getPixelValue(j - 1, i + 1, k - 1)) + 2 * (model->getPixelValue(j, i + 1, k - 1)) + 1 * (model->getPixelValue(j + 1, i + 1, k - 1)) -
					1 * (model->getPixelValue(j - 1, i - 1, k - 1)) - 2 * (model->getPixelValue(j, i - 1, k - 1)) - 1 * (model->getPixelValue(j + 1, i - 1, k - 1)) +

					2 * (model->getPixelValue(j - 1, i + 1, k)) + 4 * (model->getPixelValue(j, i + 1, k)) + 2 * (model->getPixelValue(j + 1, i + 1, k)) -
					2 * (model->getPixelValue(j - 1, i - 1, k)) - 4 * (model->getPixelValue(j, i - 1, k)) - 2 * (model->getPixelValue(j + 1, i - 1, k)) +

					1 * (model->getPixelValue(j - 1, i + 1, k + 1)) + 2 * (model->getPixelValue(j, i + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, i + 1, k + 1)) -
					1 * (model->getPixelValue(j - 1, i - 1, k + 1)) - 2 * (model->getPixelValue(j, i - 1, k + 1)) - 1 * (model->getPixelValue(j + 1, i - 1, k + 1));

				dz = -1 * (model->getPixelValue(j - 1, i + 1, k - 1)) - 2 * (model->getPixelValue(j, i + 1, k - 1)) - 1 * (model->getPixelValue(j + 1, i + 1, k - 1)) -
					2 * (model->getPixelValue(j - 1, i, k - 1)) - 4 * (model->getPixelValue(j, i, k - 1)) - 2 * (model->getPixelValue(j + 1, i, k - 1)) -
					1 * (model->getPixelValue(j - 1, i - 1, k - 1)) - 2 * (model->getPixelValue(j, i - 1, k - 1)) - 1 * (model->getPixelValue(j + 1, i - 1, k - 1)) +

					1 * (model->getPixelValue(j - 1, i + 1, k + 1)) + 2 * (model->getPixelValue(j, i + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, i + 1, k + 1)) -
					2 * (model->getPixelValue(j - 1, i, k + 1)) + 4 * (model->getPixelValue(j, i, k + 1)) + 2 * (model->getPixelValue(j + 1, i, k + 1)) -
					1 * (model->getPixelValue(j - 1, i - 1, k + 1)) + 2 * (model->getPixelValue(j, i - 1, k + 1)) + 1 * (model->getPixelValue(j + 1, i - 1, k + 1));
					
				

				
				/*dx = -1 * (model->getSmoothPixelValue(j - 1, i + 1, k - 1)) + 1 * (model->getSmoothPixelValue(j + 1, i + 1, k - 1)) -
					2 * (model->getSmoothPixelValue(j - 1, i, k - 1)) + 2 * (model->getSmoothPixelValue(j + 1, i, k - 1)) -
					1 * (model->getSmoothPixelValue(j - 1, i - 1, k - 1)) + 1 * (model->getSmoothPixelValue(j + 1, i - 1, k - 1)) -

					2 * (model->getSmoothPixelValue(j - 1, i + 1, k)) + 2 * (model->getSmoothPixelValue(j + 1, i + 1, k)) -
					4 * (model->getSmoothPixelValue(j - 1, i, k)) + 2 * (model->getSmoothPixelValue(j + 1, i, k)) -
					2 * (model->getSmoothPixelValue(j - 1, i - 1, k)) + 2 * (model->getSmoothPixelValue(j + 1, i - 1, k)) -

					1 * (model->getSmoothPixelValue(j - 1, i + 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, i + 1, k + 1)) -
					2 * (model->getSmoothPixelValue(j - 1, i, k + 1)) + 2 * (model->getSmoothPixelValue(j + 1, i, k + 1)) -
					1 * (model->getSmoothPixelValue(j - 1, i - 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, i - 1, k + 1));

				dy = 1 * (model->getSmoothPixelValue(j - 1, i + 1, k - 1)) + 2 * (model->getSmoothPixelValue(j, i + 1, k - 1)) + 1 * (model->getSmoothPixelValue(j + 1, i + 1, k - 1)) -
					1 * (model->getSmoothPixelValue(j - 1, i - 1, k - 1)) - 2 * (model->getSmoothPixelValue(j, i - 1, k - 1)) - 1 * (model->getSmoothPixelValue(j + 1, i - 1, k - 1)) +

					2 * (model->getSmoothPixelValue(j - 1, i + 1, k)) + 4 * (model->getSmoothPixelValue(j, i + 1, k)) + 2 * (model->getSmoothPixelValue(j + 1, i + 1, k)) -
					2 * (model->getSmoothPixelValue(j - 1, i - 1, k)) - 4 * (model->getSmoothPixelValue(j, i - 1, k)) - 2 * (model->getSmoothPixelValue(j + 1, i - 1, k)) +

					1 * (model->getSmoothPixelValue(j - 1, i + 1, k + 1)) + 2 * (model->getSmoothPixelValue(j, i + 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, i + 1, k + 1)) -
					1 * (model->getSmoothPixelValue(j - 1, i - 1, k + 1)) - 2 * (model->getSmoothPixelValue(j, i - 1, k + 1)) - 1 * (model->getSmoothPixelValue(j + 1, i - 1, k + 1));

				dz = -1 * (model->getSmoothPixelValue(j - 1, i + 1, k - 1)) - 2 * (model->getSmoothPixelValue(j, i + 1, k - 1)) - 1 * (model->getSmoothPixelValue(j + 1, i + 1, k - 1)) -
					2 * (model->getSmoothPixelValue(j - 1, i, k - 1)) - 4 * (model->getSmoothPixelValue(j, i, k - 1)) - 2 * (model->getSmoothPixelValue(j + 1, i, k - 1)) -
					1 * (model->getSmoothPixelValue(j - 1, i - 1, k - 1)) - 2 * (model->getSmoothPixelValue(j, i - 1, k - 1)) - 1 * (model->getSmoothPixelValue(j + 1, i - 1, k - 1)) +

					1 * (model->getSmoothPixelValue(j - 1, i + 1, k + 1)) + 2 * (model->getSmoothPixelValue(j, i + 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, i + 1, k + 1)) -
					2 * (model->getSmoothPixelValue(j - 1, i, k + 1)) + 4 * (model->getSmoothPixelValue(j, i, k + 1)) + 2 * (model->getSmoothPixelValue(j + 1, i, k + 1)) -
					1 * (model->getSmoothPixelValue(j - 1, i - 1, k + 1)) + 2 * (model->getSmoothPixelValue(j, i - 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, i - 1, k + 1));
				*/

				if (dx == 0)
					dx = 1;
				if (dy == 0)
					dy = 1;
				if (dz == 0)
					dz = 1;

				/*THe 3 possible axis
				dx-dy
				dx-dz
				dy-dz
				*/


				magnitude = sqrt( pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
				if (magnitude < 2500)
					continue;
				gradientAtPoint.x = atan2(dy, dx) * 180 / PI +180;
				gradientAtPoint.y = atan2(dx, dz) * 180 / PI +180;
				gradientAtPoint.z = atan2(dz, dy) * 180 / PI +180;

				/*gradientAtPoint.x = (gradientAtPoint.x > 0.0 ? gradientAtPoint.x : (360.0 + gradientAtPoint.x)); 
				gradientAtPoint.y = (gradientAtPoint.y > 0.0 ? gradientAtPoint.y : (360.0 + gradientAtPoint.y));
				gradientAtPoint.z = (gradientAtPoint.z > 0.0 ? gradientAtPoint.z : (360.0 + gradientAtPoint.z));*/

				model->gradient[k][std::make_pair(j, i)] = gradientAtPoint;
				/*model->gradientPoints.push_back(j);
				model->gradientPoints.push_back(i);
				model->gradientPoints.push_back(k);*/
				//model->m[k].push_back(j,i,gradientAtPoint) ;
				


				/*if (k == 100){
					outputFile << " Values are "  << "Magnitude "  << magnitude << "Angles " << gradientAtPoint.x << " " << gradientAtPoint.y << " " << gradientAtPoint.z << endl;
				}*/


				
			}

		}
	}
}

inline bool VolumeRenderer::cubeNeedsSubdivision(OctreeCube &cube){
	

	//return (int)rand % 2;

	//std::ofstream outputFile;
	//outputFile.open("GradientsFromSubdivision1.txt");
	

	glm::vec3 firstAngle(-1,-1,-1);
	int tolerance = model->tolerance;
	boost::unordered_map< std::pair<int, int>, glm::vec3>::iterator it;



	//We first check if the cube is totally out of the volume
	if (cube.origin.x > model->pixelDataWidth || cube.origin.y > model->pixelDataHeight || cube.origin.z > model->frames - 1){
		cube.needsChecking = false;
		cube.isLeaf = true;
		return false;
	}
		
	//Then we check if it is in the border between the actual volume and the initial cube in which case we need to subdivide
	if (cube.origin.x + cube.sizeX > model->pixelDataWidth || cube.origin.y + cube.sizeY > model->pixelDataHeight || cube.origin.z + cube.sizeZ > model->frames - 1){
		cube.needsChecking = true;
		return true;
	}

	//From now on all the cubes will be totally within the volume


	for (int k = cube.origin.z; k < cube.sizeZ + cube.origin.z; k = k + 1){
		for (int i = cube.origin.y; i < cube.sizeY + cube.origin.y; i = i + 1){
			for (int j = cube.origin.x; j < cube.sizeX + cube.origin.x; j = j + 1){
			



				
				
				glm::vec3 actualValue (-1,-1,-1);
				it = model->gradient[k].find(std::make_pair(j, i));
				if (it != model->gradient[k].end())
				{
					//element found;
					actualValue = it->second;
				}
				//actualValue = model->gradient[k].find(std::make_pair(j,i));


				if (firstAngle == glm::vec3(-1, -1, -1) && actualValue != glm::vec3(-1, -1, -1)){
					firstAngle = actualValue ;
					
				}

				/*if (firstAngle.x == -1 && firstAngle.y == -1 && firstAngle.z == -1 && actualValue.x != -1 && actualValue.y != -1 && actualValue.z != -1){
					firstAngle.x = actualValue.x;
					firstAngle.y = actualValue.y;
					firstAngle.z = actualValue.z;
				}*/

				//std::cout << "i= " << i << " j= " << j << " k= " << k << std::endl;
				//if (cubesSubdivided < 2 && firstAngleGiven==1)
				//outputFile << " " << firstAngle.x << " " << firstAngle.y << " " << firstAngle.z << " -- " << actualValue.x << " " << actualValue.y << " " << actualValue.z << std::endl;


				
				//Now I will try to make a different way of seeing diference between angles. This time by looking at all the angles at the same time, and seeing the total magnitude of change in angles
				int angleDifX = 180 - abs(abs(firstAngle.x - actualValue.x) - 180);
				int angleDifY = 180 - abs(abs(firstAngle.y - actualValue.y) - 180);
				int angleDifZ = 180 - abs(abs(firstAngle.z - actualValue.z) - 180);

				int angleMagnitude = sqrt(angleDifX*angleDifX + angleDifY*angleDifY + angleDifZ*angleDifZ);
				if (angleMagnitude > tolerance && firstAngle != glm::vec3(-1, -1, -1) && actualValue != glm::vec3(-1, -1, -1))
					return true;
				



				/*int angleDif;
				angleDif = 180 - abs(abs(firstAngle.x - actualValue.x) - 180);
				

				if (angleDif > tolerance && firstAngle != glm::vec3(-1, -1, -1) && actualValue != glm::vec3(-1, -1, -1)){
					return true;
				}
				angleDif = 180 - abs(abs(firstAngle.y - actualValue.y) - 180);
				if (angleDif > tolerance && firstAngle != glm::vec3(-1, -1, -1) && actualValue != glm::vec3(-1, -1, -1)){
					//cout << "Subdivide because Y" << endl;
					return true;
				}
				angleDif = 180 - abs(abs(firstAngle.z - actualValue.z) - 180);
				if (angleDif > tolerance && firstAngle != glm::vec3(-1, -1, -1) && actualValue != glm::vec3(-1, -1, -1)){
					//cout << "Subdivide because Z" << endl;
					return true;
				}*/


				/*if (abs(firstAngle.x - actualValue.x) > tolerance && firstAngle.x != -1 && firstAngle.y != -1 && firstAngle.z != -1){
					return true;
				}
				if (abs(firstAngle.y - actualValue.y) > tolerance && firstAngle.x != -1 && firstAngle.y != -1 && firstAngle.z != -1){
					return true;
				}
				if (abs(firstAngle.y - actualValue.y) > tolerance && firstAngle.x != -1 && firstAngle.y != -1 && firstAngle.z != -1){
					return true;
				}*/
				
			}
		}
	}
	//outputFile << "cube finished with NO subdivision" << endl;
	//cube.isLeaf=false;	//If it doesnt need subdivision we just mark it as a non-leaf (as a father of no cubes) just so we don't recheck it later 
	cube.needsChecking = false;
	return false;
}


//This version creates an actual tree
int VolumeRenderer::adaptiveMarchingCubes3(){
	model->verts.clear();
	model->normals.clear();
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


	int cubesSize = 0, depth = 0, maxDepth = model->octreeMaxDepth;

	if (model->gradient.size() == 0 || model->gradient.empty())
		calculateGradient();
	cout << "Finished calculating gradients" << endl;


	OctreeCube* initial = new OctreeCube;
	OctreeCube returnedFromCreateInitial;
	returnedFromCreateInitial = createInitialCube();
	initial->origin.x = returnedFromCreateInitial.origin.x;
	initial->origin.y = returnedFromCreateInitial.origin.y;
	initial->origin.z = returnedFromCreateInitial.origin.z;

	initial->sizeX = returnedFromCreateInitial.sizeX;
	initial->sizeY = returnedFromCreateInitial.sizeY;
	initial->sizeZ = returnedFromCreateInitial.sizeZ;

	model->octreeVector.push_back(initial);
	std::cout << "starting to generate octree" << std::endl;
	generateOctree_tree_version(*initial);
	std::cout << "finished generateing octree" << std::endl;

	correctlyAssignLeafs(initial);

	//Grab each octreebube in our octree and polygonise it
	//polygoniseOctree(initial);
	polygoniseOctree2(initial);	//Polygonise octree will polygonise each octree but assign the points to the cube itself an not to the model->verts


	std::cout << "starting patch cracks" << std::endl;
	crackPatch(initial);
	//crackPatch(initial);
	//crackPatch(initial);
	std::cout << "finished patching cracks" << std::endl;

	readPointsFromOctree(initial);

	

	

	boost::thread workerThread(boost::bind(&VolumeRenderer::generateNormals, this));
	//model->generatingMesh = false;


	return 0;

}

void VolumeRenderer::correctlyAssignLeafs(OctreeCube* root){
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


		if (currentCube->children[0] == NULL){
			currentCube->isLeaf = true;
		}
		else{
			currentCube->isLeaf = false;
		}
	}

}


int VolumeRenderer::generateOctree_tree_version(OctreeCube& currentCube,int currentDepth){




	//Grab the initial one and see if it needs subdivision and if we are also below the maximum octree depth, if it needs subdiviosn, sundivid it, and also recursivelly call the same function for the children

	if (currentDepth < model->octreeMaxDepth && cubeNeedsSubdivision(currentCube) && currentCube.needsChecking){
		//std::cout << "The cube doesnt have children now" << std::endl;
		currentCube.subdivide_tree_version();
		//std::cout << "The cube should have children now" << std::endl;

		model->octreeVector.push_back(currentCube.children[0]);
		model->octreeVector.push_back(currentCube.children[1]);
		model->octreeVector.push_back(currentCube.children[2]);
		model->octreeVector.push_back(currentCube.children[3]);
		model->octreeVector.push_back(currentCube.children[4]);
		model->octreeVector.push_back(currentCube.children[5]);
		model->octreeVector.push_back(currentCube.children[6]);
		model->octreeVector.push_back(currentCube.children[7]);

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
		currentCube.isLeaf=true;	// we need to check if this value is set correctly
		currentCube.needsChecking = false;
		return 0;
	}
	return 0;
}

void VolumeRenderer::generateHypercube(){

	/*int HEIGHT = 512;
	int WIDTH = 512;
	int DEPTH = 512;*/

	//vector<vector<vector<OctreeCube*> > > array3D;

	// Set up sizes. (HEIGHT x WIDTH)
	/*array3D.resize(HEIGHT);
	for (int i = 0; i < HEIGHT; ++i) {
		array3D[i].resize(WIDTH);

		for (int j = 0; j < WIDTH; ++j)
			array3D[i][j].resize(DEPTH);
	}*/
	model->array4D.resize(model->octreeMaxDepth + 1);
	int size = 2;
	for (int i = 0; i < model->octreeMaxDepth; i++){


		model->array4D[i].resize(size);
		for (int j = 0; j < size; ++j) {
			model->array4D[i][j].resize(size);

			for (int k = 0; k < size; ++k)
				model->array4D[i][j][k].resize(size);
		}
		size = size*size;
	}


	// Put some values in
	//array3D[1][2][5] = 6.0;
	//array3D[3][1][4] = 5.5;

}




int VolumeRenderer::polygoniseOctree(OctreeCube* currentCube,int currentDepth){

	//Recursive alrogithm, we go to all the children of current cube and if the children is null, we go back in the recursive stack and polygonise the current cube

	if (currentCube != NULL){
		polygoniseOctree((currentCube->children[0]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)  //We check if the current cube is a left (children is null) and has not been polygonised yet
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
		polygoniseOctree((currentCube->children[1]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
		polygoniseOctree((currentCube->children[2]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
		polygoniseOctree((currentCube->children[3]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
		polygoniseOctree((currentCube->children[4]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
		polygoniseOctree((currentCube->children[5]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
		polygoniseOctree((currentCube->children[6]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
		polygoniseOctree((currentCube->children[7]));
		if (currentCube->isLeaf  && !currentCube->polygonised == true)
			octree2CellPolygonise(*currentCube);
		currentCube->polygonised = true;
	}
	else{
		return 0;
	}


}


void VolumeRenderer::octree2CellPolygonise(OctreeCube& currentCube){
	CELL cell;


	/*
	7---------6
	/			 !
	/			 !
	3---------2	 5
	!         !
	!         !
	0---------1
	*/

	for (int j = 0; j < 8; j++){
		cell.position[j].x = 0;
		cell.position[j].x = 0;
		cell.position[j].x = 0;
		cell.val[j] = 0;
	}

	cell.position[0].x = currentCube.origin.x;
	cell.position[0].y = currentCube.origin.y;
	cell.position[0].z = currentCube.origin.z;
	cell.val[0] = model->getPixelValue(currentCube.origin.x, currentCube.origin.y, currentCube.origin.z);

	cell.position[1].x = currentCube.origin.x + currentCube.sizeX;
	cell.position[1].y = currentCube.origin.y;
	cell.position[1].z = currentCube.origin.z;
	cell.val[1] = model->getPixelValue(currentCube.origin.x + currentCube.sizeX, currentCube.origin.y, currentCube.origin.z);

	cell.position[2].x = currentCube.origin.x + currentCube.sizeX;
	cell.position[2].y = currentCube.origin.y + currentCube.sizeY;
	cell.position[2].z = currentCube.origin.z;
	cell.val[2] = model->getPixelValue(currentCube.origin.x + currentCube.sizeX, currentCube.origin.y + currentCube.sizeY, currentCube.origin.z);

	cell.position[3].x = currentCube.origin.x;
	cell.position[3].y = currentCube.origin.y + currentCube.sizeY;
	cell.position[3].z = currentCube.origin.z;
	cell.val[3] = model->getPixelValue(currentCube.origin.x, currentCube.origin.y + currentCube.sizeY, currentCube.origin.z);
	//////
	cell.position[4].x = currentCube.origin.x;
	cell.position[4].y = currentCube.origin.y;
	cell.position[4].z = currentCube.origin.z + currentCube.sizeZ;
	cell.val[4] = model->getPixelValue(currentCube.origin.x, currentCube.origin.y, currentCube.origin.z + currentCube.sizeZ);

	cell.position[5].x = currentCube.origin.x + currentCube.sizeX;
	cell.position[5].y = currentCube.origin.y;
	cell.position[5].z = currentCube.origin.z + currentCube.sizeZ;
	cell.val[5] = model->getPixelValue(currentCube.origin.x + currentCube.sizeX, currentCube.origin.y, currentCube.origin.z + currentCube.sizeZ);

	cell.position[6].x = currentCube.origin.x + currentCube.sizeX;
	cell.position[6].y = currentCube.origin.y + currentCube.sizeY;
	cell.position[6].z = currentCube.origin.z + currentCube.sizeZ;
	cell.val[6] = model->getPixelValue(currentCube.origin.x + currentCube.sizeX, currentCube.origin.y + currentCube.sizeY, currentCube.origin.z + currentCube.sizeZ);

	cell.position[7].x = currentCube.origin.x;
	cell.position[7].y = currentCube.origin.y + currentCube.sizeY;
	cell.position[7].z = currentCube.origin.z + currentCube.sizeZ;
	cell.val[7] = model->getPixelValue(currentCube.origin.x, currentCube.origin.y + currentCube.sizeY, currentCube.origin.z + currentCube.sizeZ);

	//std::cout << "cell has values" << cell.val[0] << " " << cell.val[1] << " " << cell.val[2] << " " << cell.val[3] << " " << cell.val[4] << " " << cell.val[5] << " " << cell.val[6] << " " << cell.val[7] << std::endl;

	//now we have that cell and we have to polygonise it
	//polygonise(cell, model->isoLevel, model->totalPoints);
	if (polygonise(cell, model->verts)){
		currentCube.containsVerts = true;
	}



}




int VolumeRenderer::polygoniseOctree2(OctreeCube* root){

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

		//Grab the current cube and polygonise it f it's leaf
		if (currentCube->isLeaf){
			polygoniseAssignToCube(currentCube);
		}

	}
	return 0;
}

int VolumeRenderer::polygoniseAssignToCube(OctreeCube* currentCube){



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
	cell.val[0] = model->getPixelValue(currentCube->origin.x, currentCube->origin.y, currentCube->origin.z);

	cell.position[1].x = currentCube->origin.x + currentCube->sizeX;
	cell.position[1].y = currentCube->origin.y;
	cell.position[1].z = currentCube->origin.z;
	cell.val[1] = model->getPixelValue(currentCube->origin.x + currentCube->sizeX, currentCube->origin.y, currentCube->origin.z);

	cell.position[2].x = currentCube->origin.x + currentCube->sizeX;
	cell.position[2].y = currentCube->origin.y + currentCube->sizeY;
	cell.position[2].z = currentCube->origin.z;
	cell.val[2] = model->getPixelValue(currentCube->origin.x + currentCube->sizeX, currentCube->origin.y + currentCube->sizeY, currentCube->origin.z);

	cell.position[3].x = currentCube->origin.x;
	cell.position[3].y = currentCube->origin.y + currentCube->sizeY;
	cell.position[3].z = currentCube->origin.z;
	cell.val[3] = model->getPixelValue(currentCube->origin.x, currentCube->origin.y + currentCube->sizeY, currentCube->origin.z);
	//////
	cell.position[4].x = currentCube->origin.x;
	cell.position[4].y = currentCube->origin.y;
	cell.position[4].z = currentCube->origin.z + currentCube->sizeZ;
	cell.val[4] = model->getPixelValue(currentCube->origin.x, currentCube->origin.y, currentCube->origin.z + currentCube->sizeZ);

	cell.position[5].x = currentCube->origin.x + currentCube->sizeX;
	cell.position[5].y = currentCube->origin.y;
	cell.position[5].z = currentCube->origin.z + currentCube->sizeZ;
	cell.val[5] = model->getPixelValue(currentCube->origin.x + currentCube->sizeX, currentCube->origin.y, currentCube->origin.z + currentCube->sizeZ);

	cell.position[6].x = currentCube->origin.x + currentCube->sizeX;
	cell.position[6].y = currentCube->origin.y + currentCube->sizeY;
	cell.position[6].z = currentCube->origin.z + currentCube->sizeZ;
	cell.val[6] = model->getPixelValue(currentCube->origin.x + currentCube->sizeX, currentCube->origin.y + currentCube->sizeY, currentCube->origin.z + currentCube->sizeZ);

	cell.position[7].x = currentCube->origin.x;
	cell.position[7].y = currentCube->origin.y + currentCube->sizeY;
	cell.position[7].z = currentCube->origin.z + currentCube->sizeZ;
	cell.val[7] = model->getPixelValue(currentCube->origin.x, currentCube->origin.y + currentCube->sizeY, currentCube->origin.z + currentCube->sizeZ);


	//Now we have the cell for the cube created, so we polygonise it


	glm::vec3 vertlist[12];

	int isoLevel = model->isoLevel;

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
	if (model->edgeTable[cubeIndex] & 1) {
		interpolate(model->isoLevel, cell.position[0], cell.position[1], cell.val[0], cell.val[1], point);
		vertlist[0] = point;
	}
	if (model->edgeTable[cubeIndex] & 2) {
		interpolate(model->isoLevel, cell.position[1], cell.position[2], cell.val[1], cell.val[2], point);
		vertlist[1] = point;
	}
	if (model->edgeTable[cubeIndex] & 4) {
		interpolate(model->isoLevel, cell.position[2], cell.position[3], cell.val[2], cell.val[3], point);
		vertlist[2] = point;
	}
	if (model->edgeTable[cubeIndex] & 8) {
		interpolate(model->isoLevel, cell.position[3], cell.position[0], cell.val[3], cell.val[0], point);
		vertlist[3] = point;

	}
	if (model->edgeTable[cubeIndex] & 16) {
		interpolate(model->isoLevel, cell.position[4], cell.position[5], cell.val[4], cell.val[5], point);
		vertlist[4] = point;
	}
	if (model->edgeTable[cubeIndex] & 32) {
		interpolate(model->isoLevel, cell.position[5], cell.position[6], cell.val[5], cell.val[6], point);
		vertlist[5] = point;
	}
	if (model->edgeTable[cubeIndex] & 64) {
		interpolate(model->isoLevel, cell.position[6], cell.position[7], cell.val[6], cell.val[7], point);
		vertlist[6] = point;
	}
	if (model->edgeTable[cubeIndex] & 128) {
		interpolate(model->isoLevel, cell.position[7], cell.position[4], cell.val[7], cell.val[4], point);
		vertlist[7] = point;
	}
	if (model->edgeTable[cubeIndex] & 256) {
		interpolate(model->isoLevel, cell.position[0], cell.position[4], cell.val[0], cell.val[4], point);
		vertlist[8] = point;
	}
	if (model->edgeTable[cubeIndex] & 512) {
		interpolate(model->isoLevel, cell.position[1], cell.position[5], cell.val[1], cell.val[5], point);
		vertlist[9] = point;
	}
	if (model->edgeTable[cubeIndex] & 1024) {
		interpolate(model->isoLevel, cell.position[2], cell.position[6], cell.val[2], cell.val[6], point);
		vertlist[10] = point;
	}
	if (model->edgeTable[cubeIndex] & 2048) {
		interpolate(model->isoLevel, cell.position[3], cell.position[7], cell.val[3], cell.val[7], point);
		vertlist[11] = point;
	}


	glm::vec3 normal;
	unsigned char* dataPointer;
	int pointerOffset = model->numberOfBytes;
	int value1 = 0, value2 = 0;

	/* Create the triangles */

	//WE create a vector with the points and we assignt it to the cube
	vector<glm::vec3> *points = new vector<glm::vec3>;

	glm::vec3 point1, point2, point3;
	for (int i = 0; model->triTable[cubeIndex][i] != -1; i += 3) {

		point1.x = (vertlist[model->triTable[cubeIndex][i]]).x;
		point1.y = (vertlist[model->triTable[cubeIndex][i]]).y;
		point1.z = (vertlist[model->triTable[cubeIndex][i]]).z;

		point2.x = (vertlist[model->triTable[cubeIndex][i + 1]]).x;
		point2.y = (vertlist[model->triTable[cubeIndex][i + 1]]).y;
		point2.z = (vertlist[model->triTable[cubeIndex][i + 1]]).z;

		point3.x = (vertlist[model->triTable[cubeIndex][i + 2]]).x;
		point3.y = (vertlist[model->triTable[cubeIndex][i + 2]]).y;
		point3.z = (vertlist[model->triTable[cubeIndex][i + 2]]).z;

		points->push_back(point1);
		points->push_back(point2);
		points->push_back(point3);

		/*verts.push_back(point1);
		verts.push_back(point2);
		verts.push_back(point3);*/
	}

	currentCube->points = points;
	currentCube->containsVerts = true;

}

//Go through each cube and write the points info to the model->verts
int VolumeRenderer::readPointsFromOctree(OctreeCube* root){
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

		//Grab the current cube and polygonise it f it's leaf
		if (currentCube->points!=NULL){
			for (int i = 0; i < currentCube->points->size();i++){
				model->verts.push_back(currentCube->points->at(i));
			
			}
		}

	}
	return 0;
}
 
void VolumeRenderer::ballPivot(){
	std::cout << "creating mesh with ball pivot algorithm" << std::endl;
	calculateGradient();	//I should actually make it so tht it calculates gradients only when it gradients are empty. And also make it so that when we load a new dicom file we clear the previous gradients

	//grab a point from the gradients. Since gradients is still a map we convert it to two vector of positons and normals
	//Create another vector of sortedPoints which are sorted by the distance from the onw we picked
	//Pick two points from the sortedPoints (whithin a certain distance range) and see if those 3 points are compatible with the R-sphere

	pair <glm::vec3,glm::vec3> referencePoint;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;

	if (positions.empty()){
		for (int k = 0; k < model->frames; k++){
			for (auto kv : model->gradient[k]) {
				glm::vec3 positionKey;
				positionKey.x = kv.first.first;
				positionKey.y = kv.first.second;
				positionKey.z = k;
				positions.push_back(positionKey);
				normals.push_back(kv.second);
			}
		}
	}
	std::vector<glm::vec3> sortedPositions;
	std::vector<glm::vec3> sortedNormals;


	for (int i = 0; i < positions.size(); i++){
		referencePoint.first = positions[i];
		referencePoint.second = normals[i];

		//Now that we have the point, we create a sortedPoints list and we sort the rest of them based on the distance from our reference point
		sortedPositions.clear();
		sortedNormals.clear();

		sortedPositions = positions;
		sortedNormals = normals;

		sortedPositions.erase(sortedPositions.begin() + i);
		sortedNormals.erase(sortedNormals.begin() + i);

		//Now we sort the points that remain, which are all the points minus the one grabbed by reference
		bool needsSwapping = true;
		while (needsSwapping){
			needsSwapping = false;
			for (int j = 0; j < sortedPositions.size() - 1; j++){

				double distance1 = sqrt(pow(2, sortedPositions[j].x - referencePoint.first.x) + pow(2, sortedPositions[j].y - referencePoint.first.y) + pow(2, sortedPositions[j].z - referencePoint.first.z));
				double distance2 = sqrt(pow(2, sortedPositions[j + 1].x - referencePoint.first.x) + pow(2, sortedPositions[j + 1].y - referencePoint.first.y) + pow(2, sortedPositions[j + 1].z - referencePoint.first.z));

				if (distance1 > distance2){
					//swap positions and normals
					glm::vec3 temp;
					temp = sortedPositions[j];
					sortedPositions[j] = sortedPositions[j + 1];
					sortedPositions[j + 1] = temp;

					temp = sortedNormals[j];
					sortedNormals[j] = sortedNormals[j + 1];
					sortedNormals[j + 1] = temp;

					needsSwapping = true;
				}
			}
		}
		//Now we have the sorted points and normals so now we have to search in it.

		std::cout << "distance between the reference and the first points is" << sqrt(pow(2, sortedPositions[0].x - referencePoint.first.x) + pow(2, sortedPositions[0].y - referencePoint.first.y) + pow(2, sortedPositions[0].z - referencePoint.first.z)) << std::endl;
		std::cout << "distance between the reference and the first points is" << sqrt(pow(2, sortedPositions[1].x - referencePoint.first.x) + pow(2, sortedPositions[1].y - referencePoint.first.y) + pow(2, sortedPositions[1].z - referencePoint.first.z)) << std::endl;
		std::cout << "distance between the reference and the first points is" << sqrt(pow(2, sortedPositions[2].x - referencePoint.first.x) + pow(2, sortedPositions[2].y - referencePoint.first.y) + pow(2, sortedPositions[2].z - referencePoint.first.z)) << std::endl;
		std::cout << "distance between the reference and the first points is" << sqrt(pow(2, sortedPositions[3].x - referencePoint.first.x) + pow(2, sortedPositions[3].y - referencePoint.first.y) + pow(2, sortedPositions[3].z - referencePoint.first.z)) << std::endl;
		break;	//I just use the break for debuggin so that i stop at the first reference point
		

	}

}

int VolumeRenderer::loadDICOMPixelData(const char* file){


	int bitsAllocated, bitsStored, highBit, pixelRepresentation, frames, height, width;

	DcmFileFormat fileformat;
	OFCondition status = fileformat.loadFile(file);
	DcmDataset *data = fileformat.getDataset();
	data->chooseRepresentation(EXS_LittleEndianImplicit, NULL); //Not really necesarry because the dicomImage class already does that
	DicomImage* img = new DicomImage(file);

	img->setMinMaxWindow();	//It is setting the adequate contrast and brightness


	getRepresentation(file, bitsAllocated, bitsStored, highBit, pixelRepresentation, frames, height, width);
	if (frames == 0){	//It didnt find the frame tag so it's a singleframe dicom
		frames = 1;
	}
	cout << "bitsAlocated=" << bitsAllocated << " bitsStored=" << bitsStored << " pixelRepresentation=" << pixelRepresentation << " frames=" << frames << " height=" << height << endl;
	model->pixelDataHeight = height;
	model->pixelDataWidth = width;
	model->frames = frames;
	model->numberOfBytes = bitsAllocated/8;
	ui.frameSlider->setMaximum(frames-1);
	if (bitsAllocated==8)
		ui.isoLevelSlider->setMaximum(255);
	if (bitsAllocated == 16)
		ui.isoLevelSlider->setMaximum(65536);
	if (bitsAllocated == 32)
		ui.isoLevelSlider->setMaximum(4294967296);
	
	
	model->pixelData = (unsigned char**)malloc(frames*sizeof(unsigned char*));
	for (int i = 0; i < frames; i++){
		//Allocate data for the pixeldata from each frame, read data dn then store it in another free space in model->pixelData
		img = new DicomImage(data, data->getOriginalXfer(), CIF_UsePartialAccessToPixelData, i, 1);	//Get the img
		img->setMinMaxWindow();
		//(boost::get<unsigned char**>(model->pixelData))[i] = (unsigned char*)img->getOutputData(bitsAllocated, 0, 0);
		(model->pixelData)[i] = (unsigned char*)img->getOutputData(bitsAllocated, 0, 0);
	}

	return 1;
}

int VolumeRenderer::loadDICOMPixelData(QStringList fileNames){


	int bitsAllocated, bitsStored, highBit, pixelRepresentation, frames, height, width;

	getRepresentation(fileNames[0].toStdString().c_str(), bitsAllocated, bitsStored, highBit, pixelRepresentation, frames, height, width);
	if (frames == 0){	//It didnt find the frame tag so it's a singleframe dicom
		frames = 1;
	}
	cout << "bitsAlocated=" << bitsAllocated << " bitsStored=" << bitsStored << " pixelRepresentation=" << pixelRepresentation << " frames=" << frames << " height=" << height << endl;
	model->pixelDataHeight = height;
	model->pixelDataWidth = width;
	model->frames = fileNames.size();
	model->numberOfBytes = bitsAllocated / 8;
	ui.frameSlider->setMaximum(fileNames.size()-1);
	if (bitsAllocated == 8)
		ui.isoLevelSlider->setMaximum(255);
	if (bitsAllocated == 16)
		ui.isoLevelSlider->setMaximum(65536);
	if (bitsAllocated == 32)
		ui.isoLevelSlider->setMaximum(4294967296);


	model->pixelData = (unsigned char**)malloc(fileNames.size()*sizeof(unsigned char*));
	for (int i = 0; i < fileNames.size(); i++){
		
		cout << "reading file" << fileNames[i].toStdString() <<endl;
		//DicomImage *img = new DicomImage(fileNames[i].toStdString().c_str(), 0);
		/*DcmFileFormat fileformat;
		OFCondition status = fileformat.loadFile(fileNames[i].toStdString().c_str());
		DcmDataset *data = fileformat.getDataset();
		data->chooseRepresentation(EXS_LittleEndianImplicit, NULL);*/ //Not really necesarry because the dicomImage class already does tha
		//DicomImage* img = new DicomImage(data, data->getOriginalXfer(), CIF_UsePartialAccessToPixelData, i, 1);	//Get the img
		DicomImage *img = new DicomImage(fileNames[i].toStdString().c_str(), CIF_MayDetachPixelData , 0, 1);	//WatchOut. If you put CIF_UsePartialAccessToPixelData as the flag it seems to work but on the 510th frame it stops reading it. If you leave it at 0 it works correctly but it may not work well for multiframe dicomfiles (like the heart) more teasting needed
		//img->setMinMaxWindow();
		double center, width;
		img->getWindow(center, width);
		img->setWindow(400, 2000);
		//cout << "window is " << center << "  " << width << endl;
		cout << "copying pixel data" << endl;
		//(boost::get<unsigned char**>(model->pixelData))[i] = (unsigned char*)img->getOutputData(bitsAllocated, 0, 0);
		unsigned char* outputPointer=NULL;
		//(model->pixelData)[i] = (unsigned char*)img->getOutputData(bitsAllocated, 0, 0);		//WatchOut you are asinging pixel data to the output of that image which is made on the stack. when the function terminates, the data may not exist anymore
		outputPointer = (unsigned char*)img->getOutputData(bitsAllocated, 0, 0);
		(model->pixelData)[i] = (unsigned char*)malloc(model->pixelDataWidth*model->pixelDataHeight*bitsAllocated / 8);
		for (int j = 0; j < model->pixelDataWidth*model->pixelDataHeight*bitsAllocated/8; j++){
			(model->pixelData)[i][j] = outputPointer[j];
		}
		
		img->deleteOutputData();
		delete(img);
		cout << "done" << endl;
	}

	return 1;
}



void VolumeRenderer::on_isoLevelSlider_valueChanged(){
	cout << "isolevel changed" << endl;
	model->isoLevel=ui.isoLevelSlider->value();
	//Now we should march all the cubes again, and refresh the openglwidget
	//wipePoints();
	generateMesh();
	ui.glwidget->dataSended = 0;
	ui.glwidget->update();
	//on_marchingSquares_clicked();
	
}

void VolumeRenderer::on_loadTestImageButton_clicked(){
	cout << "Load the test image1" << endl;
	wipeBitmap();
	wipePoints();
	//load_image_data("E:\\Universidad\\Hecho por mi\\Volume Renderer\\TFG\\VolumeRenderer\\Win32\\Debug\\test.bmp");
	load_image_data("test7.bmp");
	marchingSquares();
	ui.dicomviewer2dgl->setFrame(0);
	
	
}
void VolumeRenderer::on_loadTestImage2Button_clicked(){
	cout << "Load the test image2" << endl;
	wipeBitmap();
	wipePoints();
	load_image_data("E:\\Universidad\\Hecho por mi\\Volume Renderer\\TFG\\VolumeRenderer\\Win32\\Debug\\angkor-grey.bmp");
	marchingSquares();
}
void VolumeRenderer::on_loadDICOMButton_clicked(){
	cout << "Load the dicom image" << endl;
	wipeBitmap();
	wipePoints();
	//Read the dicom file and store in a 3d array. An array that stores 2d array of pixeldata
	//loadDICOMPixelData("E:\\Universidad\\Hecho por mi\\Volume Renderer\\Examples\\MR-MONO2-8-16x-heart\\MR-MONO2-8-16x-heart");
	//loadDICOMPixelData("E:\\Universidad\\Hecho por mi\\Volume Renderer\\Examples\\US-MONO2-8-8x-execho\\US-MONO2-8-8x-execho");
	//loadDICOMPixelData("E:\\Universidad\\Hecho por mi\\Volume Renderer\\Examples\\NBIA\\Head-Neck Cetuximab-Demo\\0522c0017\\1.3.6.1.4.1.14519.5.2.1.5099.8010.301891355591695382542416006021\\1.3.6.1.4.1.14519.5.2.1.5099.8010.406309334041021679085400200182\\000050.dcm");
	loadDICOMPixelData("E:\\Universidad\\Hecho por mi\\Volume Renderer\\Examples\\NBIA\\Head-Neck Cetuximab-Demo\\0522c0017\\1.3.6.1.4.1.14519.5.2.1.5099.8010.211172929770388792425820374442\\1.3.6.1.4.1.14519.5.2.1.5099.8010.256809656670264137422504899725\\000090.dcm");
	marchingSquares();

	cout << "loaded" << endl;
	//cout << "value is" << model->pixelData[250][250] << endl;
	
}
void VolumeRenderer::on_loadMFDICOMButton_clicked(){
	wipeBitmap();
	wipePoints();
	loadDICOMPixelData("E:\\Universidad\\Hecho por mi\\Volume Renderer\\Examples\\MR-MONO2-8-16x-heart\\MR-MONO2-8-16x-heart");
	generateMesh();
	ui.glwidget->sendDataToGL();
	ui.glwidget->update();
}



void VolumeRenderer::on_loadDICOMFromFile_clicked(){
	
	QFileDialog dialog(this);
	//dialog.setOption(QFileDialog::DontUseNativeDialog, true);
	//dialog.setDirectory(QDir::currentPath());
	dialog.setDirectory("E:\\Universidad\\Hecho por mi\\Volume Renderer\\Examples\\");
	dialog.setFileMode(QFileDialog::ExistingFiles);
	dialog.setNameFilter(tr("Files (*.*)"));
	QStringList fileNames;
	if (dialog.exec())
		fileNames = dialog.selectedFiles();
	
	if (fileNames.empty())
		return;
	wipeBitmap();
	wipePoints();
	wipePixelData();


	loadDICOMPixelData(fileNames);

	generateMesh();
	ui.dicomviewer2dgl->setFrame(model->frames / 2);
	ui.glwidget->sendDataToGL();
	ui.glwidget->update();
	emit dataFinishedReading();

}

void VolumeRenderer::on_resolutionSlider_valueChanged(){
	cout << "Change the resolution to " << ui.resolutionSlider->value() << endl;
	model->cellSizeX = ui.resolutionSlider->value();
	model->cellSizeY = ui.resolutionSlider->value();
	model->cellSizeZ = ui.resolutionSlider->value();

	//wipePoints();
	generateMesh();
	ui.glwidget->dataSended = 0;
	ui.glwidget->update();
}



void VolumeRenderer::on_pointFlagButton_clicked(){
/*	if (ui.pointFlagButton->isChecked()){
		model->pointFlag = true;
		ui.borderFlagButton->setEnabled(false);
	}
	else{
		model->pointFlag = false;
		ui.borderFlagButton->setEnabled(true);
	}
	wipePoints();
	generateMesh();*/
}
void VolumeRenderer::on_borderFlagButton_clicked(){
	/*if (ui.borderFlagButton->isChecked()){
		model->borderFlag = true;
		ui.pointFlagButton->setEnabled(false);
	}
	else{
		model->borderFlag = false;
		ui.pointFlagButton->setEnabled(true);
	}
	wipePoints();
	generateMesh();*/
}

void VolumeRenderer::on_frameSlider_valueChanged(){
	model->frame_to_display = ui.frameSlider->value();
	ui.glwidget->setFrame(model->frame_to_display);
	ui.dicomviewer2dgl->setFrame(model->frame_to_display);
	ui.glwidget->update();
	ui.dicomviewer2dgl->update();
}


void VolumeRenderer::on_showWireframeButton_clicked(){
	if (ui.showWireframeButton->isChecked()){
		model->showWireframe = true;
	}
	else{
		model->showWireframe = false;
	}
	ui.glwidget->update();
}

void VolumeRenderer::on_toleranceSlider_valueChanged(){
	model->tolerance = ui.toleranceSlider->value();
	cout << "tolerance set to " << model->tolerance;
	model->cubes.clear();
	//wipePoints();
	//adaptiveMarchingCubes();
	generateMesh();
}

void VolumeRenderer::on_marchingCubesButton_clicked(){
	if (ui.marchingCubesButton->isChecked()){
		model->algorithmChosen = 1;
		//wipePoints();
		generateMesh();
	}
}

void VolumeRenderer::on_adaptiveMarchingCubesButton_clicked(){
	if (ui.adaptiveMarchingCubesButton->isChecked()){
		model->algorithmChosen = 2;
		//wipePoints();
		generateMesh();
	}
}
void VolumeRenderer::on_adaptiveMarchingCubes2Button_clicked(){
	if (ui.adaptiveMarchingCubes2Button->isChecked()){
		model->algorithmChosen = 3;
		//wipePoints();
		generateMesh();
	}
}
void VolumeRenderer::on_adaptiveMarchingCubes3Button_clicked(){
	if (ui.adaptiveMarchingCubes3Button->isChecked()){
		model->algorithmChosen = 4;
		//wipePoints();
		generateMesh();
	}
}
void VolumeRenderer::on_ballPivotButton_clicked(){
	if (ui.ballPivotButton->isChecked()){
		model->algorithmChosen = 4;
		//wipePoints();
		generateMesh();
	}
}
void VolumeRenderer::on_perspectiveButton_clicked(){
	if (ui.perspectiveButton->isChecked()){
		model->perspectiveActivated = true;
	}
	else{
		model->perspectiveActivated = false;
	}
}

void VolumeRenderer::on_normalsPerTriangleButton_clicked(){
	if (ui.normalsPerTriangleButton->isChecked()){
		model->normalsAlgChosen = 1;
	}
	if (!interactive)
		return;
	model->normals.clear();
	model->generatingMesh = true;
	boost::thread workerThread(boost::bind(&VolumeRenderer::generateNormals, this));
	//generateNormals();
	
}
void VolumeRenderer::on_normalsPerVerticeButton_clicked(){
	if (ui.normalsPerVerticeButton->isChecked()){
		model->normalsAlgChosen = 2;
	}
	if (!interactive)
		return;
	model->normals.clear();
	model->generatingMesh = true;
	boost::thread workerThread(boost::bind(&VolumeRenderer::generateNormals, this));
	//generateNormals();
	
}


void VolumeRenderer::on_showMeshButton_clicked(){
	if (ui.showMeshButton ->isChecked()){
		model->showMesh = true;
	}
	else{
		model->showMesh = false;
	}
}
void VolumeRenderer::on_showGradientButton_clicked(){
	if (ui.showGradientButton->isChecked()){
		model->showGradient = true;
	}
	else{
		model->showGradient = false;
	}
}
void VolumeRenderer::on_orientationZButton_clicked(){
	if (ui.orientationZButton->isChecked()){
		model->orientation = 1;
		model->frame_to_display = 0;
		ui.frameSlider->setValue(0);
	}
	
}
void VolumeRenderer::on_orientationXButton_clicked(){
	if (ui.orientationXButton->isChecked()){
		model->orientation = 2;
		model->frame_to_display = 0;
		ui.frameSlider->setValue(0);
	}
}
void VolumeRenderer::on_orientationYButton_clicked(){
	if (ui.orientationYButton->isChecked()){
		model->orientation = 3;
		model->frame_to_display = 0;
		ui.frameSlider->setValue(0);
	}
}
void VolumeRenderer::on_showCubesButton_clicked(){
	if (ui.showCubesButton->isChecked()){
		model->showCubes = true;;
	}
}


//************************************
// Method:    generateMesh
// FullName:  VolumeRenderer::generateMesh
// Access:    private 
// Returns:   void
// Qualifier:
//************************************
void VolumeRenderer::generateMesh(int force){


	if (!force){			
		if (!interactive)
			return;
	}

	wipePoints();

	std::cout << "generating mesh with" << model->algorithmChosen << std::endl;

	model->generatingMesh = true;
	if (model->algorithmChosen == 1){
		//boost::thread workerThread(boost::bind(&VolumeRenderer::marchingSquares, this));

		MarchingCuber* mc = new MarchingCuber(&(model->pixelData), &(model->verts), &(model->normals), model->isoLevel, model->frames, model->pixelDataHeight, model->pixelDataWidth, model->cellSizeX, model->cellSizeY, model->cellSizeZ, model->numberOfBytes, model->interpolateDepth, model->linearInterpolation);
		mc->run();
		generateNormals();

		//workerThread.join();
		//marchingSquares();
	}
		
	if (model->algorithmChosen == 2)
		adaptiveMarchingCubes();
	if (model->algorithmChosen == 3)
		adaptiveMarchingCubes2();
	if (model->algorithmChosen == 4){
		//adaptiveMarchingCubes3();
		AdaptiveCuber* amc = new AdaptiveCuber(&(model->pixelData), &(model->verts), &(model->normals), model->isoLevel, model->frames, model->pixelDataHeight, model->pixelDataWidth, model->cellSizeX, model->cellSizeY, model->cellSizeZ, model->numberOfBytes, model->interpolateDepth, model->linearInterpolation,model->octreeMaxDepth,&(model->gradient),model->tolerance);
		amc->run();
		generateNormals();
	}

	//model->generatingMesh = true;
	//boost::thread workerThread(boost::bind(&VolumeRenderer::generateNormals, this));
	//generateNormals();


	//qWarning() << QString("%L1").arg(i);
	QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
	QLocale aEnglish;
	//qWarning() << aEnglish.toString(i);
	ui.numberOfTrianglesLabel->setText(aEnglish.toString(model->verts.size()/9));


	/*QPalette palette;

	//white text
	QBrush brush(QColor(255, 255, 255, 255));
	brush.setStyle(Qt::SolidPattern);

	//black background
	QBrush brush1(QColor(0, 0, 0, 255));
	brush1.setStyle(Qt::SolidPattern);

	//set white text    
	palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
	palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);

	//set black background
	palette.setBrush(QPalette::Active, QPalette::Window, brush1);
	palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);

	//set palette    
	textLabel->setPalette(palette);*/

	QPalette plt;
	plt.setColor(QPalette::WindowText, Qt::white);
	ui.numberOfTrianglesLabel->setPalette(plt);
	
}

//************************************
//After the triangles are created we generate the normals for them, either one per triangle or one per vertice
//************************************
void VolumeRenderer::generateNormals(){
	model->normals.clear();

	if (model->verts.empty())
		return;

	glm::vec3 u, v, n;

	if (model->normalsAlgChosen == 1){

		for (int i = 0; i < model->verts.size() - 3; i = i + 3){

			//emit progressValueChangedSignal(i * 100 / model->verts.size());
			u.x = model->verts[i + 1].x - model->verts[i].x;
			u.y = model->verts[i + 1].y - model->verts[i].y;
			u.z = model->verts[i + 1].z - model->verts[i].z;

			v.x = model->verts[i + 2].x - model->verts[i].x;
			v.y = model->verts[i + 2].y - model->verts[i].y;
			v.z = model->verts[i + 2].z - model->verts[i].z;

			/*u.x = model->verts[i + 3] - model->verts[i];
			u.y = model->verts[i + 4] - model->verts[i + 1];
			u.z = model->verts[i + 5] - model->verts[i + 2];

			v.x = model->verts[i + 6] - model->verts[i];
			v.y = model->verts[i + 7] - model->verts[i + 1];
			v.z = model->verts[i + 8] - model->verts[i + 2];*/

			n.x = u.y*v.z - u.z*v.y;
			n.y = u.z*v.x - u.x*v.z;
			n.z = u.x*v.y - u.y*v.x;

			//we now normalize it
			double length = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);

			n.x = n.x / length;
			n.y = n.y / length;
			n.z = n.z / length;

			//WE push three time once. Because we have 3 normal per triangle, one for each vetice
			model->normals.push_back(n);
			model->normals.push_back(n);
			model->normals.push_back(n);
		}
	}

	if (model->normalsAlgChosen == 2){

		//I will now try to do the same normal but with the sobel operator///
		///////////////////////////////////////////////////////////////////

		double dx = 0.0, dy = 0.0, dz = 0.0;
		int j, y, k;
		double length;

		int steps = model->verts.size() / 10 - 1;

		for (int i = 0; i < model->verts.size() ; i = i + 1){

			if (i% steps==0)
				emit progressValueChangedSignal(i * 100 / model->verts.size());
			j = boost::math::iround(model->verts[i].x);
			y = boost::math::iround(model->verts[i].y);
			k = boost::math::iround(model->verts[i].z);

			//getSmoothPixelValue

			dx = -1 * (model->getPixelValue(j - 1, y + 1, k - 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k - 1)) -
				2 * (model->getPixelValue(j - 1, y, k - 1)) + 2 * (model->getPixelValue(j + 1, y, k - 1)) -
				1 * (model->getPixelValue(j - 1, y - 1, k - 1)) + 1 * (model->getPixelValue(j + 1, y - 1, k - 1)) -

				2 * (model->getPixelValue(j - 1, y + 1, k)) + 2 * (model->getPixelValue(j + 1, y + 1, k)) -
				4 * (model->getPixelValue(j - 1, y, k)) + 2 * (model->getPixelValue(j + 1, y, k)) -
				2 * (model->getPixelValue(j - 1, y - 1, k)) + 2 * (model->getPixelValue(j + 1, y - 1, k)) -

				1 * (model->getPixelValue(j - 1, y + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k + 1)) -
				2 * (model->getPixelValue(j - 1, y, k + 1)) + 2 * (model->getPixelValue(j + 1, y, k + 1)) -
				1 * (model->getPixelValue(j - 1, y - 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y - 1, k + 1));

			dy = 1 * (model->getPixelValue(j - 1, y + 1, k - 1)) + 2 * (model->getPixelValue(j, y + 1, k - 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k - 1)) -
				1 * (model->getPixelValue(j - 1, y - 1, k - 1)) - 2 * (model->getPixelValue(j, y - 1, k - 1)) - 1 * (model->getPixelValue(j + 1, y - 1, k - 1)) +

				2 * (model->getPixelValue(j - 1, y + 1, k)) + 4 * (model->getPixelValue(j, y + 1, k)) + 2 * (model->getPixelValue(j + 1, y + 1, k)) -
				2 * (model->getPixelValue(j - 1, y - 1, k)) - 4 * (model->getPixelValue(j, y - 1, k)) - 2 * (model->getPixelValue(j + 1, y - 1, k)) +

				1 * (model->getPixelValue(j - 1, y + 1, k + 1)) + 2 * (model->getPixelValue(j, y + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k + 1)) -
				1 * (model->getPixelValue(j - 1, y - 1, k + 1)) - 2 * (model->getPixelValue(j, y - 1, k + 1)) - 1 * (model->getPixelValue(j + 1, y - 1, k + 1));

			dz = -1 * (model->getPixelValue(j - 1, y + 1, k - 1)) - 2 * (model->getPixelValue(j, y + 1, k - 1)) - 1 * (model->getPixelValue(j + 1, y + 1, k - 1)) -
				2 * (model->getPixelValue(j - 1, y, k - 1)) - 4 * (model->getPixelValue(j, y, k - 1)) - 2 * (model->getPixelValue(j + 1, y, k - 1)) -
				1 * (model->getPixelValue(j - 1, y - 1, k - 1)) - 2 * (model->getPixelValue(j, y - 1, k - 1)) - 1 * (model->getPixelValue(j + 1, y - 1, k - 1)) +

				1 * (model->getPixelValue(j - 1, y + 1, k + 1)) + 2 * (model->getPixelValue(j, y + 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y + 1, k + 1)) -
				2 * (model->getPixelValue(j - 1, y, k + 1)) + 4 * (model->getPixelValue(j, y, k + 1)) + 2 * (model->getPixelValue(j + 1, y, k + 1)) -
				1 * (model->getPixelValue(j - 1, y - 1, k + 1)) + 2 * (model->getPixelValue(j, y - 1, k + 1)) + 1 * (model->getPixelValue(j + 1, y - 1, k + 1));

			///Smooth one

			/*dx = -1 * (model->getSmoothPixelValue(j - 1, y + 1, k - 1)) + 1 * (model->getSmoothPixelValue(j + 1, y + 1, k - 1)) -
				2 * (model->getSmoothPixelValue(j - 1, y, k - 1)) + 2 * (model->getSmoothPixelValue(j + 1, y, k - 1)) -
				1 * (model->getSmoothPixelValue(j - 1, y - 1, k - 1)) + 1 * (model->getSmoothPixelValue(j + 1, y - 1, k - 1)) -

				2 * (model->getSmoothPixelValue(j - 1, y + 1, k)) + 2 * (model->getSmoothPixelValue(j + 1, y + 1, k)) -
				4 * (model->getSmoothPixelValue(j - 1, y, k)) + 2 * (model->getSmoothPixelValue(j + 1, y, k)) -
				2 * (model->getSmoothPixelValue(j - 1, y - 1, k)) + 2 * (model->getSmoothPixelValue(j + 1, y - 1, k)) -

				1 * (model->getSmoothPixelValue(j - 1, y + 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, y + 1, k + 1)) -
				2 * (model->getSmoothPixelValue(j - 1, y, k + 1)) + 2 * (model->getSmoothPixelValue(j + 1, y, k + 1)) -
				1 * (model->getSmoothPixelValue(j - 1, y - 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, y - 1, k + 1));

			dy = 1 * (model->getSmoothPixelValue(j - 1, y + 1, k - 1)) + 2 * (model->getSmoothPixelValue(j, y + 1, k - 1)) + 1 * (model->getSmoothPixelValue(j + 1, y + 1, k - 1)) -
				1 * (model->getSmoothPixelValue(j - 1, y - 1, k - 1)) - 2 * (model->getSmoothPixelValue(j, y - 1, k - 1)) - 1 * (model->getSmoothPixelValue(j + 1, y - 1, k - 1)) +

				2 * (model->getSmoothPixelValue(j - 1, y + 1, k)) + 4 * (model->getSmoothPixelValue(j, y + 1, k)) + 2 * (model->getSmoothPixelValue(j + 1, y + 1, k)) -
				2 * (model->getSmoothPixelValue(j - 1, y - 1, k)) - 4 * (model->getSmoothPixelValue(j, y - 1, k)) - 2 * (model->getSmoothPixelValue(j + 1, y - 1, k)) +

				1 * (model->getSmoothPixelValue(j - 1, y + 1, k + 1)) + 2 * (model->getSmoothPixelValue(j, y + 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, y + 1, k + 1)) -
				1 * (model->getSmoothPixelValue(j - 1, y - 1, k + 1)) - 2 * (model->getSmoothPixelValue(j, y - 1, k + 1)) - 1 * (model->getSmoothPixelValue(j + 1, y - 1, k + 1));

			dz = -1 * (model->getSmoothPixelValue(j - 1, y + 1, k - 1)) - 2 * (model->getSmoothPixelValue(j, y + 1, k - 1)) - 1 * (model->getSmoothPixelValue(j + 1, y + 1, k - 1)) -
				2 * (model->getSmoothPixelValue(j - 1, y, k - 1)) - 4 * (model->getSmoothPixelValue(j, y, k - 1)) - 2 * (model->getSmoothPixelValue(j + 1, y, k - 1)) -
				1 * (model->getSmoothPixelValue(j - 1, y - 1, k - 1)) - 2 * (model->getSmoothPixelValue(j, y - 1, k - 1)) - 1 * (model->getSmoothPixelValue(j + 1, y - 1, k - 1)) +

				1 * (model->getSmoothPixelValue(j - 1, y + 1, k + 1)) + 2 * (model->getSmoothPixelValue(j, y + 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, y + 1, k + 1)) -
				2 * (model->getSmoothPixelValue(j - 1, y, k + 1)) + 4 * (model->getSmoothPixelValue(j, y, k + 1)) + 2 * (model->getSmoothPixelValue(j + 1, y, k + 1)) -
				1 * (model->getSmoothPixelValue(j - 1, y - 1, k + 1)) + 2 * (model->getSmoothPixelValue(j, y - 1, k + 1)) + 1 * (model->getSmoothPixelValue(j + 1, y - 1, k + 1));*/


			//we now normalize it


			length = sqrt(dx*dx + dy*dy + dz*dz);

			dx = dx / length;
			dy = dy / length;
			dz = dz / length;

			n.x = dx;
			n.y = dy;
			n.z = dz;

			model->normals.push_back(n);
			//model->normals.push_back(dy);
			//model->normals.push_back(dz);
		}
	}

	//model->generatingMesh = false;
	emit generatingFinishedSignal();

}

void VolumeRenderer::wipeBitmap(){
	//WE first wipe the data that migth already be in the model.
	if (model->bitmapPixels != NULL){
		free(model->bitmapPixels);
		model->bitmapPixels = NULL;
	}
	
}


//************************************
//Wipes all the verts and normals. It is used before calculating a new mesh
//************************************
void VolumeRenderer::wipePoints(){
	
	model->cubes.clear();
	model->verts.clear();
	model->normals.clear();
	model->octreeVector.clear();
}
//************************************
// Wipes and frees all the PixelData. It is used before loading a new Dicom file
//************************************
void VolumeRenderer::wipePixelData(){
	int numberOfFrees=0;
	for (int i = 0; i < model->frames; i++){
		free(model->pixelData[i]);
		numberOfFrees++;
	}
	free(model->pixelData);
	
	ui.glwidget->xMove = 0; //Now we reset the position so that the new model will be centered
	ui.glwidget->yMove = 0;
	ui.glwidget->zMove = 0;
	ui.glwidget->xRot = 0;
	ui.glwidget->yRot = 0;

}

void VolumeRenderer::progressValueChangedSlot(int newValue){
	cout << "new value of progress" << newValue << endl;
	ui.progressBar->setValue(newValue);
}

void VolumeRenderer::generatingFinishedSlot(){
	model->generatingMesh = false;
	ui.progressBar->setValue(0);
}

void VolumeRenderer::on_xPosText_editingFinished(){
	//std::cout << "the text is" << ui.interpolateDepthText->text().toUtf8().constData() << std::endl;
	if (model->xPosPoint == ui.xPosText->text().toInt())
		return;
	model->xPosPoint = ui.xPosText->text().toInt();
	std::cout << "setting x to " << model->xPosPoint << std::endl;
	//wipePoints();
	//generateMesh();
}
void VolumeRenderer::on_yPosText_editingFinished(){
	//std::cout << "the text is" << ui.interpolateDepthText->text().toUtf8().constData() << std::endl;
	if (model->yPosPoint == ui.yPosText->text().toInt())
		return;
	model->yPosPoint = ui.yPosText->text().toInt();
	std::cout << "setting y to " << model->yPosPoint << std::endl;
	//wipePoints();
	//generateMesh();
}

void VolumeRenderer::on_zPosText_editingFinished(){
	//std::cout << "the text is" << ui.interpolateDepthText->text().toUtf8().constData() << std::endl;
	if (model->zPosPoint == ui.zPosText->text().toInt())
		return;
	model->zPosPoint = ui.zPosText->text().toInt();
	std::cout << "setting z to " << model->zPosPoint << std::endl;
	//wipePoints();
	//generateMesh();
}

void VolumeRenderer::on_linearInterpolationSlider_valueChanged(){
	model->interpolateDepth = ui.linearInterpolationSlider->value();
	cout << "interpolate set to " << model->interpolateDepth;
	model->cubes.clear();
	//wipePoints();
	//adaptiveMarchingCubes();
	generateMesh();
}


void VolumeRenderer::on_generateMeshButton_clicked(){
	//std::cout << "clicked on generate mesh" << std::endl;
	generateMesh(1);	//We force the generation of a mesh by passing it 1
}
void VolumeRenderer::on_interactiveButton_clicked(){
	if (ui.interactiveButton->isChecked()){
		interactive = true;
		generateMesh();
	}
	else{
		interactive = false;
	}
}

void  VolumeRenderer::on_octreeDepthSlider_valueChanged(){
	model->octreeMaxDepth = ui.octreeDepthSlider->value();
	cout << "octree depth set to " << model->octreeMaxDepth;
	//model->cubes.clear();
	//wipePoints();
	//adaptiveMarchingCubes();
	generateMesh();
}