#include "FileReader.h"
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
#include <iostream>
#include <QFileDialog>

using namespace std;

FileReader::FileReader()
{

}

FileReader::FileReader(QObject *_parent) 
{
	
}






FileReader::~FileReader()
{
}

/*! \brief Pasando como parametro la ruta de los ficheros DICOM, see leen los pixels data y se escriben en un objeto de tipo PixelData
*/
int FileReader::loadDICOMPixelData(QStringList fileNames,PixelData*pixelData){


	int bitsAllocated, bitsStored, highBit, pixelRepresentation, frames, height, width;

	getRepresentation(fileNames[0].toStdString().c_str(), bitsAllocated, bitsStored, highBit, pixelRepresentation, frames, height, width);
	if (frames == 0){	//It didnt find the frame tag so it's a singleframe dicom
		frames = 1;
	}
	cout << "bitsAlocated=" << bitsAllocated << " bitsStored=" << bitsStored << " pixelRepresentation=" << pixelRepresentation << " frames=" << frames << " height=" << height << endl;
	pixelData->height = height;
	pixelData->width = width;
	pixelData->frames = fileNames.size();
	pixelData->numberOfBytes = bitsAllocated / 8;
	


	pixelData->data = (unsigned char**)malloc(fileNames.size()*sizeof(unsigned char*));
	for (int i = 0; i < fileNames.size(); i++){

		emit progressValueChangedSignal(i * 100 / fileNames.size());
		cout << "reading file" << fileNames[i].toStdString() << endl;
		//DicomImage *img = new DicomImage(fileNames[i].toStdString().c_str(), 0);
		/*DcmFileFormat fileformat;
		OFCondition status = fileformat.loadFile(fileNames[i].toStdString().c_str());
		DcmDataset *data = fileformat.getDataset();
		data->chooseRepresentation(EXS_LittleEndianImplicit, NULL);*/ //Not really necesarry because the dicomImage class already does tha
		//DicomImage* img = new DicomImage(data, data->getOriginalXfer(), CIF_UsePartialAccessToPixelData, i, 1);	//Get the img
		DicomImage *img = new DicomImage(fileNames[i].toStdString().c_str(), CIF_MayDetachPixelData, 0, 1);	//WatchOut. If you put CIF_UsePartialAccessToPixelData as the flag it seems to work but on the 510th frame it stops reading it. If you leave it at 0 it works correctly but it may not work well for multiframe dicomfiles (like the heart) more teasting needed
		//img->setMinMaxWindow();
		double center, width;
		img->getWindow(center, width);
		img->setWindow(400, 2000);
		//cout << "window is " << center << "  " << width << endl;
		cout << "copying pixel data" << endl;
		//(boost::get<unsigned char**>(model->pixelData))[i] = (unsigned char*)img->getOutputData(bitsAllocated, 0, 0);
		unsigned char* outputPointer = NULL;
		//(model->pixelData)[i] = (unsigned char*)img->getOutputData(bitsAllocated, 0, 0);		//WatchOut you are asinging pixel data to the output of that image which is made on the stack. when the function terminates, the data may not exist anymore
		outputPointer = (unsigned char*)img->getOutputData(bitsAllocated, 0, 0);
		(pixelData->data)[i] = (unsigned char*)malloc(pixelData->width*pixelData->height*bitsAllocated / 8);
		for (int j = 0; j < pixelData->width*pixelData->height*bitsAllocated / 8; j++){
			(pixelData->data)[i][j] = outputPointer[j];
		}

		img->deleteOutputData();
		delete(img);
		cout << "done" << endl;
	}

	emit dataFinishedReadingSignal();
	return 1;
}


/*! \brief Para la lectura de los ficheros DICOM es necesario conocer su representacion (alto, ancho, numeor de pixeles, bits por pixel, etc)
*/
int FileReader::getRepresentation(QString fileName, int& bitsAllocated, int& bitsStored, int& hightBit, int& pixelRepresentation, int& frames, int& height, int& width){
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


int FileReader::loadOBJFile(QString fileName, PixelData*pixelData, std::vector<glm::vec3>& verts, std::vector<glm::vec3>& normals){



	std::ifstream file(fileName.toStdString());
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string token;
		while (iss >> token)
		{
			// do something with token
			//std::cout << "token is" << token << std::endl;

			if (token.compare("v") == 0){
				//std::cout << "token is vertice"  << std::endl;

				glm::vec3 point;
				float temp = 0.0;

				iss >> token;
				temp = ::atof(token.c_str());
				point.x = temp;

				iss >> token;
				temp = ::atof(token.c_str());
				point.y = temp;

				iss >> token;
				temp = ::atof(token.c_str());
				point.z = temp;

				verts.push_back(point);

				//std::cout << "created point with " << point.x << "  " << point.y << "  " << point.z << std::endl;

			}
			if (token.compare("vn") == 0){
				//std::cout << "token is normal" << std::endl;
				glm::vec3 normal;
				float temp = 0.0;

				iss >> token;
				temp = ::atof(token.c_str());
				normal.x = temp;

				iss >> token;
				temp = ::atof(token.c_str());
				normal.y = temp;

				iss >> token;
				temp = ::atof(token.c_str());
				normal.z = temp;

				normals.push_back(normal);
				//std::cout << "created normal with " << normal.x << "  " << normal.y << "  " << normal.z << std::endl;

			}

			if (token.compare("representation") == 0){
				//std::cout << "token is normal" << std::endl;
				glm::vec3 normal;
				double temp;

				iss >> token;
				temp = ::atof(token.c_str());
				pixelData->width = temp;

				iss >> token;
				temp = ::atof(token.c_str());
				pixelData->height = temp;

				iss >> token;
				temp = ::atof(token.c_str());
				pixelData->frames = temp;

				//std::cout << "loaded representation" << model->pixelData->width << "  " << model->pixelData->height << "  " << model->pixelData->frames << std::endl;

			}




		}
	}

	emit objFinishedReadingSignal();
	return 0;

}