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
#include <math.h> 
#include <tuple>
#include <boost/math/special_functions/round.hpp>
#include <boost/thread.hpp>
#include <queue>


//#include <boost/variant.hpp>
#define PI 3.14159265


extern "C" void launch_kernel(std::vector<int >, std::vector< std::vector<int> >);


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
	mc = NULL;
	amc = NULL;
	ng = NULL;
	reader = NULL;



	ui.setupUi(this);
	model = new Model();
	ui.glwidget->model = model;
	ui.dicomviewer2dgl->model = model;
	interactive = true;
	generatingMesh = false;

	//establishConnections();
	connect(this, SIGNAL(generatingStartedSignal()), ui.glwidget, SLOT(generatingStartedSlot()));
	connect(this, SIGNAL(generatingFinishedSignal()), ui.glwidget, SLOT(generatingFinishedSlot()));

	QFile File("stylesheet.qss");
	File.open(QFile::ReadOnly);
	QString StyleSheet = QLatin1String(File.readAll());

	qApp->setStyleSheet(StyleSheet);

	ui.progressBar->setTextVisible(false);
	ui.progressBar->setMaximumHeight(4);
	ui.progressText->setText("<font color='black'></font>");

	ui.progressText->setStyleSheet("QLabel { color: black }");
	
	

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


/*! \brief Establece las conexiones de signals y slots entre la clase MarchingCubes y el controlador
*/
void VolumeRenderer::establishConnectionsMC(){
	connect(mc, SIGNAL(progressValueChangedSignal(int)), this, SLOT(progressValueChangedSlot(int)));
	connect(mc, SIGNAL(finishedMeshSignal()), this, SLOT(finishedMeshSlot()));
}

/*! \brief Establece las conexiones de signals y slots entre la clase AdaptiveMarchingCubes y el controlador
*/
void VolumeRenderer::establishConnectionsAMC(){
	connect(amc, SIGNAL(progressValueChangedSignal(int)), this, SLOT(progressValueChangedSlot(int)));
	connect(amc, SIGNAL(finishedMeshSignal()), this, SLOT(finishedMeshSlot()));
	connect(amc, SIGNAL(progressTextSignal(QString)), this, SLOT(progressTextSlot(QString)));
}

/*! \brief Establece las conexiones de signals y slots entre la clase NormalsGenerator y el controlador
*/
void VolumeRenderer::establishConnectionsNG(){
	connect(ng, SIGNAL(progressValueChangedSignal(int)), this, SLOT(progressValueChangedSlot(int)));
	connect(ng, SIGNAL(finishedNormalsSignal()), this, SLOT(finishedNormalsSlot()));
}

/*! \brief Establece las conexiones de signals y slots entre la clase Reader y el controlador
*/
void VolumeRenderer::establishConnectionsREADER(){
	connect(reader, SIGNAL(progressValueChangedSignal(int)), this, SLOT(progressValueChangedSlot(int)));
	connect(reader, SIGNAL(dataFinishedReadingSignal()), ui.glwidget, SLOT(dataFinishedReadingSlot()));
	connect(reader, SIGNAL(dataFinishedReadingSignal()), this, SLOT(dataFinishedReadingSlot()));
	connect(reader, SIGNAL(objFinishedReadingSignal()), this, SLOT(finishedNormalsSlot()));		//When you finish reading the obj it is the same as when you finish the normals because you completely change the verts and the normals of the mesh

}


/*void VolumeRenderer::marchingSquares(unsigned char** & pixelData, int numberOfByte, int celllSizeX, int celllSizeY, int celllSizeZ,
	int pixelDataHeight, int pixelDataWidth, int frames, int isoLevel, std::vector<glm::vec3>& verts, std::vector<glm::vec3>& normals)*/



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



 
void VolumeRenderer::ballPivot(){
	std::cout << "creating mesh with ball pivot algorithm" << std::endl;
	//calculateGradient();	//I should actually make it so tht it calculates gradients only when it gradients are empty. And also make it so that when we load a new dicom file we clear the previous gradients

	//grab a point from the gradients. Since gradients is still a map we convert it to two vector of positons and normals
	//Create another vector of sortedPoints which are sorted by the distance from the onw we picked
	//Pick two points from the sortedPoints (whithin a certain distance range) and see if those 3 points are compatible with the R-sphere

	pair <glm::vec3,glm::vec3> referencePoint;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;

	if (positions.empty()){
		for (int k = 0; k < model->pixelData->frames; k++){
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






/*! \brief Escribe en el modelo el nuevo dato de isoLevel cuando cambia el slider correspondiente
*/
void VolumeRenderer::on_isoLevelSlider_valueChanged(){
	cout << "isolevel changed" << endl;

	if (model->isoLevel == ui.isoLevelSlider->value())
		return;

	model->isoLevel=ui.isoLevelSlider->value();
	//Now we should march all the cubes again, and refresh the openglwidget
	//wipePoints();
	generateMesh();
	ui.glwidget->dataSended = 0;
	ui.glwidget->update();
	//on_marchingSquares_clicked();
	
}







/*! \brief Abre una ventana de dialogo donde seleccionar los archivos dicom. La ruta de los archivos se pasan a la clase Reader donde se leen los pixeles de cada archivo
*/
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
	wipePoints();
	wipePixelData();

	ui.progressText->setText("Reading DICOM Files");
	reader = new FileReader;
	establishConnectionsREADER();
	//reader->loadDICOMPixelData(fileNames, model->pixelData);
	boost::thread workerThread(boost::bind(&FileReader::loadDICOMPixelData, reader,fileNames,model->pixelData));
	//loadDICOMPixelData(fileNames);

	ui.frameSlider->setMaximum(fileNames.size() - 1);
	

	

}

void VolumeRenderer::dataFinishedReadingSlot(){
	ui.progressText->setText("");
	ui.progressBar->setValue(0);

	
	if (model->pixelData->numberOfBytes == 1)
		ui.isoLevelSlider->setMaximum(255);
	if (model->pixelData->numberOfBytes == 2)
		ui.isoLevelSlider->setMaximum(65536);
	if (model->pixelData->numberOfBytes == 3)
		ui.isoLevelSlider->setMaximum(4294967296);

	generateMesh(1);
	ui.dicomviewer2dgl->setFrame(model->pixelData->frames / 2);
	//ui.glwidget->sendDataToGL();
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




/*! \brief Establece el frame para ser visualizado en el visor dicom 2d
*/
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
		model->algorithmChosen = 5;
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
	emit generatingStartedSignal();
	generatingMesh = true;
	generateNormals();
	
}
void VolumeRenderer::on_normalsPerVerticeButton_clicked(){
	if (ui.normalsPerVerticeButton->isChecked()){
		model->normalsAlgChosen = 2;
	}
	if (!interactive)
		return;
	model->normals.clear();
	emit generatingStartedSignal();
	generatingMesh = true;
	generateNormals();
	
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
		model->frame_to_display = model->pixelData->frames/2;
		ui.frameSlider->setValue(model->pixelData->frames / 2);
		ui.frameSlider->setMaximum(model->pixelData->frames);
		ui.dicomviewer2dgl->setFrame(model->pixelData->frames / 2);

		//Give values to the slider and maximums

		ui.borderYTopSlider->setValue(model->pixelData->borderYTop);
		ui.borderYTopSlider->setMaximum(model->pixelData->height);

		ui.borderYBottomSlider->setValue(model->pixelData->borderYBottom);
		ui.borderYBottomSlider->setMaximum(model->pixelData->height);
		
		ui.borderXLeftSlider->setValue(model->pixelData->borderXLeft);
		ui.borderXLeftSlider->setMaximum(model->pixelData->width);

		ui.borderXRightSlider->setValue(model->pixelData->borderXRight);
		ui.borderXRightSlider->setMaximum(model->pixelData->width);


		//Leave everything as original because in the z direction there cannot be any distorsion
		QRect rect;

		rect = ui.borderYTopSlider->geometry();
		rect.setBottom(360 + 231);
		rect.setTop(360);
		ui.borderYTopSlider->setGeometry(rect);
		rect = ui.borderYBottomSlider->geometry();
		rect.setBottom(360 + 231);
		rect.setTop(360);
		ui.borderYBottomSlider->setGeometry(rect);

		rect = ui.borderXLeftSlider->geometry();
		rect.setLeft(1100);
		rect.setRight(1100 + 231);
		ui.borderXLeftSlider->setGeometry(rect);
		rect = ui.borderXRightSlider->geometry();
		rect.setLeft(1100);
		rect.setRight(1100 + 231);
		ui.borderXRightSlider->setGeometry(rect);
		


	}
	
}
void VolumeRenderer::on_orientationXButton_clicked(){
	if (ui.orientationXButton->isChecked()){
		model->orientation = 2;
		model->frame_to_display = model->pixelData->width/2;
		ui.frameSlider->setValue(model->pixelData->width / 2);
		ui.frameSlider->setMaximum(model->pixelData->width);
		ui.dicomviewer2dgl->setFrame(model->pixelData->width / 2);

		ui.borderYTopSlider->setValue(model->pixelData->borderZFurther);
		ui.borderYTopSlider->setMaximum(model->pixelData->frames);

		ui.borderYBottomSlider->setValue(model->pixelData->borderZCloser);
		ui.borderYBottomSlider->setMaximum(model->pixelData->frames);

		ui.borderXLeftSlider->setValue(model->pixelData->borderYBottom);
		ui.borderXLeftSlider->setMaximum(model->pixelData->height);

		ui.borderXRightSlider->setValue(model->pixelData->borderYTop);
		ui.borderXRightSlider->setMaximum(model->pixelData->height);



		if (model->pixelData->frames < model->pixelData->width){	//change Y leave X as original
			double ratio = (float)model->pixelData->frames / (float)model->pixelData->width;

			QRect rect;
	
			rect = ui.borderYTopSlider->geometry();
			rect.setBottom(360 + 231);
			rect.setTop(360 + (231 - 231 * ratio));
			ui.borderYTopSlider->setGeometry(rect);
			rect = ui.borderYBottomSlider->geometry();
			rect.setBottom(360 + 231);
			rect.setTop(360 + (231 - 231 * ratio));
			ui.borderYBottomSlider->setGeometry(rect);


			//Leave X as original
			rect = ui.borderXLeftSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231);
			ui.borderXLeftSlider->setGeometry(rect);
			rect = ui.borderXRightSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231);
			ui.borderXRightSlider->setGeometry(rect);



		}
		else{
			QRect rect;

			rect = ui.borderYTopSlider->geometry();
			rect.setBottom(360 + 231);
			rect.setTop(360);
			ui.borderYTopSlider->setGeometry(rect);

			rect = ui.borderYBottomSlider->geometry();
			rect.setBottom(360 + 231);
			rect.setTop(360);
			ui.borderYBottomSlider->setGeometry(rect);
			//modify X

			double ratio = (float)model->pixelData->width / (float)model->pixelData->frames;

			rect = ui.borderXLeftSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231*ratio);
			ui.borderXLeftSlider->setGeometry(rect);
			rect = ui.borderXRightSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231*ratio);
			ui.borderXRightSlider->setGeometry(rect);

		}



	}
}
void VolumeRenderer::on_orientationYButton_clicked(){
	if (ui.orientationYButton->isChecked()){
		model->orientation = 3;
		model->frame_to_display = model->pixelData->height/2;
		ui.frameSlider->setValue(model->pixelData->height/2);
		ui.frameSlider->setMaximum(model->pixelData->height);
		ui.dicomviewer2dgl->setFrame(model->pixelData->height / 2);

		//std::cout << "pixels have border closer " << model->pixelData->borderZCloser << " and border further " << model->pixelData->borderZFurther << std::endl;


		ui.borderYTopSlider->setValue(model->pixelData->borderZFurther);
		ui.borderYTopSlider->setMaximum(model->pixelData->frames);

		ui.borderYBottomSlider->setValue(model->pixelData->borderZCloser);
		ui.borderYBottomSlider->setMaximum(model->pixelData->frames);


		ui.borderXLeftSlider->setValue(model->pixelData->borderXLeft);
		ui.borderXLeftSlider->setMaximum(model->pixelData->width);

		ui.borderXRightSlider->setValue(model->pixelData->borderXRight);
		ui.borderXRightSlider->setMaximum(model->pixelData->width);


		if (model->pixelData->frames < model->pixelData->height){ //The image will be squashed down so we need to move the Border Y sliders to coincide
			
			double ratio = (float)model->pixelData->frames / (float)model->pixelData->height;

			QRect rect;
			//360 is the slider y position got from the designer, and the 231 it the slider height
			rect = ui.borderYTopSlider->geometry();
			rect.setBottom(360 + 231);
			rect.setTop(360 +  (231-231 * ratio));
			ui.borderYTopSlider->setGeometry(rect);
			rect = ui.borderYBottomSlider->geometry();
			rect.setBottom(360 + 231);
			rect.setTop(360 + (231 - 231 * ratio));
			ui.borderYBottomSlider->setGeometry(rect);



			//Leave X as original
			rect = ui.borderXLeftSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231);
			ui.borderXLeftSlider->setGeometry(rect);
			rect = ui.borderXRightSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231);
			ui.borderXRightSlider->setGeometry(rect);

		}
		else{ //In this case we need to change the border X slider to coincide and leave the Y to their original values;

			//Leve the y as original
			QRect rect;

			rect = ui.borderYTopSlider->geometry();
			rect.setBottom(360 + 231);
			rect.setTop(360);
			ui.borderYTopSlider->setGeometry(rect);

			rect = ui.borderYBottomSlider->geometry();
			rect.setBottom(360 + 231);
			rect.setTop(360);
			ui.borderYBottomSlider->setGeometry(rect);
			//Modify x

			double ratio = (float)model->pixelData->width / (float)model->pixelData->frames;

			rect = ui.borderXLeftSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231 * ratio);
			ui.borderXLeftSlider->setGeometry(rect);
			rect = ui.borderXRightSlider->geometry();
			rect.setLeft(1100);
			rect.setRight(1100 + 231 * ratio);
			ui.borderXRightSlider->setGeometry(rect);

		}
		
	}
}
void VolumeRenderer::on_showCubesButton_clicked(){
	if (ui.showCubesButton->isChecked()){
		model->showCubes = true;
	}
}


/*! \brief Funcion encargada de crear los objetos y los hilos necesarios para la creacion del mallado.
*
*  Si el sistema esta en modo interactivo esta funcion no se ejecuta, a no ser que el usuario fuerze la creacion de un mallado mediante el boton Generate Mesh.
*  La funcion se encarga de crear los objetos MarchingCuber o AdaptiveMarchingCuber y de crear un hilo donde se va a ejecutar la tarea principal de cada uno.
*/
void VolumeRenderer::generateMesh(int force){


	if (!force){			
		if (!interactive)
			return;
	}

	if (generatingMesh == true)
		return;

	wipePoints();

	std::cout << "generating mesh with" << model->algorithmChosen << std::endl;

	emit generatingStartedSignal();
	generatingMesh = true;
	if (model->algorithmChosen == 1){
		//boost::thread workerThread(boost::bind(&VolumeRenderer::marchingSquares, this));
		ui.progressText->setText("<font color='black'>Generating Mesh</font>");
		mc = new MarchingCuber((model->pixelData), &(model->verts), &(model->normals), model->isoLevel, model->cellSizeX, model->cellSizeY, model->cellSizeZ, model->interpolateDepth);
		establishConnectionsMC();
		//mc->run();
		boost::thread workerThread(boost::bind(&MarchingCuber::run, mc));
		
		
		//generateNormals();

		//workerThread.join();
		//marchingSquares();
	}
		
	if (model->algorithmChosen == 2){

		

		launch_kernel(model->edgeTable,model->triTable);
	}
	if (model->algorithmChosen == 3){
		amc = new AdaptiveCuber((model->pixelData), &(model->verts), &(model->normals), model->isoLevel, model->cellSizeX, model->cellSizeY, model->cellSizeZ, model->interpolateDepth, model->octreeMaxDepth, &(model->gradient), model->tolerance);
		establishConnectionsAMC();
		//amc->runWithCracks();
		boost::thread workerThread(boost::bind(&AdaptiveCuber::runWithCracks, amc));
		//generateNormals();
	}
	if (model->algorithmChosen == 4){
		//adaptiveMarchingCubes3();
		amc = new AdaptiveCuber((model->pixelData), &(model->verts), &(model->normals), model->isoLevel,  model->cellSizeX, model->cellSizeY, model->cellSizeZ,  model->interpolateDepth, model->octreeMaxDepth,&(model->gradient),model->tolerance);
		establishConnectionsAMC();
		//amc->run();
		boost::thread workerThread(boost::bind(&AdaptiveCuber::run, amc));
		//generateNormals();
	}

	//model->generatingMesh = true;
	//boost::thread workerThread(boost::bind(&VolumeRenderer::generateNormals, this));
	//generateNormals();


	//qWarning() << QString("%L1").arg(i);
	QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
	QLocale aEnglish;
	//qWarning() << aEnglish.toString(i);
	ui.numberOfTrianglesLabel->setText(aEnglish.toString(model->verts.size()/3));


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

/*! \brief Slot que se ejecuta cuando el MarchingCuber o AdaptiveMarching cuber han acabado indicando que el mallado esta creado. Ejecuta la funcion de generateNormals para crear las normales.
*/
void VolumeRenderer::finishedMeshSlot(){

	//Copy the octree vector over to the model
	/*if (amc!=NULL)
	for (int i = 0; i < amc->octreeVector.size(); i++){
		model->octreeVector.push_back(amc->octreeVector[i]);
	}*/


	//std::cout << "entered in finished mesh slot" << std::endl;
	ui.progressText->setText("<font color='black'>Generating Normals</font>");
	generateNormals();
}

/*! \brief Slot indicando que NormalsGenerator ha acabado por lo tanto el mallado esta listo para representarse asi que se emite la se�al correspondiente al visor en 3D.
*/
void VolumeRenderer::finishedNormalsSlot(){
	//std::cout << "entered in finished normals slot" << std::endl;
	emit generatingFinishedSignal();
	ui.progressText->setText("<font color='black'></font>");
	ui.progressBar->setValue(0);
	generatingMesh = false;
	ui.glwidget->update();
}

//************************************
//After the triangles are created we generate the normals for them, either one per triangle or one per vertice
//************************************

/*! \brief Funcion que crea el objeto NormalsGenerator y genera un hilo para ejecutar su funcion (normales por triangulos o por vertices dependiendo del algoritmo seleccionado)
*/
void VolumeRenderer::generateNormals(){

	ui.progressText->setText("Generating Normals");

	model->normals.clear();
	if (model->normalsAlgChosen == 1){

		ng = new NormalsGenerator();
		establishConnectionsNG();
		//ng->normalsPerTriangle(model->pixelData, model->verts, model->normals);
		boost::thread workerThread(boost::bind(&NormalsGenerator::normalsPerTriangle, ng, model->pixelData, boost::ref(model->verts), boost::ref(model->normals)));
	}

	if (model->normalsAlgChosen == 2){

		ng = new NormalsGenerator();
		establishConnectionsNG();
		//ng->normalsPerVertex(model->pixelData, model->verts, model->normals);
		boost::thread workerThread(boost::bind(&NormalsGenerator::normalsPerVertex, ng, model->pixelData, boost::ref(model->verts), boost::ref(model->normals)));
	}

	//model->generatingMesh = false;

}





/*! \brief Borra todos los puntos del mallado, sus normales y los cubos que se han creado, dejando listo el modelo para un nuevo mallado
*/
void VolumeRenderer::wipePoints(){
	
	model->cubes.clear();
	model->verts.clear();
	model->normals.clear();
	model->octreeVector.clear();
}

/*! \brief Borra todos los datos de los pixels, dejando listo al modelo para la carga de otros archivos DICOM
*/
void VolumeRenderer::wipePixelData(){
	int numberOfFrees=0;

	if (model->pixelData->data == NULL)
		return;

	for (int i = 0; i < model->pixelData->frames; i++){
		free(model->pixelData->data[i]);
		numberOfFrees++;
	}

	model->gradient.clear();
	free(model->pixelData->data);
	model->pixelData->data = NULL;
	model->pixelData->numberOfBytes = 0;;
	
	ui.glwidget->xMove = 0; //Now we reset the position so that the new model will be centered
	ui.glwidget->yMove = 0;
	ui.glwidget->zMove = 0;
	ui.glwidget->xRot = 0;
	ui.glwidget->yRot = 0;

}

/*! \brief Actualiza el valor del ProgressBar con los valores que se envian a traves de las se�ales de los diferentes hilos
*/
void VolumeRenderer::progressValueChangedSlot(int newValue){
	cout << "new value of progress" << newValue << endl;
	ui.progressBar->setValue(newValue);
}


void VolumeRenderer::progressTextSlot(QString text){
	//ui.progressText->setText("<font color='black'>Some text</font>");
	//ui.progressText->setText(text);



	ui.progressText->setText(text);
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

/*! \brief Fuerza la generacion de un mallado a partir de las opciones seleccionadas. Se crear� un mallado aunque el sistema no este en el modo interactivo
*/
void VolumeRenderer::on_generateMeshButton_clicked(){
	//std::cout << "clicked on generate mesh" << std::endl;
	generateMesh(1);	//We force the generation of a mesh by passing it 1
}

/*! \brief Establece el modo interactivo, es decir se creara un mallado automaticamente al cambiar cualquier opcion.
*/
void VolumeRenderer::on_interactiveButton_clicked(){
	if (ui.interactiveButton->isChecked()){
		interactive = true;
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




void VolumeRenderer::on_borderYBottomSlider_valueChanged(){

	if (model->orientation == 1){
		model->pixelData->borderYBottom = ui.borderYBottomSlider->value();
	}
	if (model->orientation == 2){
		model->pixelData->borderZCloser = ui.borderYBottomSlider->value();
	}
	if (model->orientation == 3){
		model->pixelData->borderZCloser = ui.borderYBottomSlider->value();
	}
	generateMesh();
}
void VolumeRenderer::on_borderYTopSlider_valueChanged(){
	if (model->orientation == 1){
		model->pixelData->borderYTop = ui.borderYTopSlider->value();
	}
	if (model->orientation == 2){
		model->pixelData->borderZFurther = ui.borderYTopSlider->value();
	}
	if (model->orientation == 3){
		model->pixelData->borderZFurther = ui.borderYTopSlider->value();
	}
	
	generateMesh();

}




void VolumeRenderer::on_borderXLeftSlider_valueChanged(){

	if (model->orientation == 1){
		model->pixelData->borderXLeft = ui.borderXLeftSlider->value();
	}
	if (model->orientation == 2){
		model->pixelData->borderYBottom = ui.borderXLeftSlider->value();
	}
	if (model->orientation == 3){
		model->pixelData->borderXLeft = ui.borderXLeftSlider->value();
	}
	generateMesh();
}
void VolumeRenderer::on_borderXRightSlider_valueChanged(){
	if (model->orientation == 1){
		model->pixelData->borderXRight = ui.borderXRightSlider->value();
	}
	if (model->orientation == 2){
		model->pixelData->borderYTop = ui.borderXRightSlider->value();
	}
	if (model->orientation == 3){
		model->pixelData->borderXRight = ui.borderXRightSlider->value();
	}

	generateMesh();

}

void VolumeRenderer::on_loadObjButton_clicked(){
	std::cout << "loading obj file" << std::endl;
	




	QString fileName=NULL;
	fileName = QFileDialog::getOpenFileName(this,
		tr("Open OBJ File"), "/home", tr("OBJ File (*.obj)"));
	
	if (fileName == NULL)
		return;

	//send the signal so stop rendering. The generating mesh signal
	//then when we are finished send the one that sais, finished generating
	//also have to read the pixelwidht, height and frames
	emit generatingStartedSignal();
	wipePixelData();
	wipePoints();

	ui.progressText->setText("Reading OBJ File");
	reader = new FileReader;
	establishConnectionsREADER();
	boost::thread workerThread(boost::bind(&FileReader::loadOBJFile, reader, fileName, model->pixelData, boost::ref(model->verts), boost::ref(model->normals)));

	

	
	//emit generatingFinishedSignal();

}
void VolumeRenderer::on_writeObjButton_clicked(){
	std::cout << "writing obj file" << std::endl;
	
	



	QString filters("OBJ files (*.obj);;Text files (*.txt);;All files (*.*)");
	QString defaultFilter("OBJ files (*.obj)");

	/* Static method approach */
	QString filename; 
	filename = QFileDialog::getSaveFileName(0, "Save file", QDir::currentPath(),
		filters, &defaultFilter);
	
	if (filename.isEmpty())
		return;

	ui.progressText->setText("Exporting to OBJ");

	Exporter* exporter = new Exporter;
	connect(exporter, SIGNAL(progressValueChangedSignal(int)), this, SLOT(progressValueChangedSlot(int)));
	connect(exporter, SIGNAL(finishedWritingToFileSignal()), this, SLOT(finishedNormalsSlot()));
	//exporter->writeToOBJ(filename, model->verts, model->normals, model->pixelData->width, model->pixelData->height, model->pixelData->frames);

	boost::thread workerThread(boost::bind(&Exporter::writeToOBJ, exporter, filename, model->verts, model->normals, model->pixelData->width, model->pixelData->height, model->pixelData->frames));

}