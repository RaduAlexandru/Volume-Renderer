#pragma once
#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"

#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/dcfilefo.h" 
#include "dcmtk/dcmdata/dcmetinf.h" 
#include "dcmtk/dcmimgle/dcmimage.h" 

#include "dcmtk/dcmjpeg/djdecode.h"     
#include "dcmtk/dcmjpeg/djencode.h"  

#include "dcmtk/dcmdata/dcpxitem.h"
#include <vector>

class PixelData
{
public:
	PixelData();
	~PixelData();

	


	unsigned char** data;

	//std::vector<DicomImage *> data2;

	int width;
	int height;
	int frames;
	int numberOfBytes;

	int borderXRight, borderXLeft, borderZCloser, borderZFurther, borderYTop, borderYBottom;

	int getPixelValue(int x, int y, int z);
	int getSmoothPixelValue(int x, int y, int z);

	/*int getWidthWithBorder();
	int getHeightWithBorder();
	int getFramesWithBorder();*/

	
};



