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
		DicomImage *img = new DicomImage(fileNames[i].toStdString().c_str(), CIF_MayDetachPixelData, 0, 1);	
		double center=0.0, width=0.0;
		img->getWindow(center, width);
		img->setWindow(400, 2000);
		
		unsigned char* outputPointer = NULL; //Se obtiene el puntero a la zona de memoria de los pixels y se empieza a copiar desde alli 
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

/*! \brief Carga el valor de los vertice y normales de un fichero OBJ. Tambien es necesario leer la representacion, es decir al alto, ancho y grueso de la figura para representarlo correctamente
*/
int FileReader::loadOBJFile(QString fileName, PixelData*pixelData, Mesh* mesh){



	std::ifstream file(fileName.toStdString());
	std::string line;
	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string token;
		while (iss >> token)
		{

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

				mesh->verts.push_back(point);

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

				mesh->normals.push_back(normal);

			}

			if (token.compare("representation") == 0){
				//std::cout << "token is normal" << std::endl;
				glm::vec3 normal;
				double temp;

				iss >> token;
				temp = ::atof(token.c_str());
				mesh->width = temp;

				iss >> token;
				temp = ::atof(token.c_str());
				mesh->height = temp;

				iss >> token;
				temp = ::atof(token.c_str());
				mesh->frames = temp;

			}

		}
	}

	emit objFinishedReadingSignal();
	return 0;

}