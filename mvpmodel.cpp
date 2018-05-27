#include "mvpmodel.h"

MvpModel::MvpModel()
{
    referenceModel = NULL;
    growthMesh = NULL;

	// Load the Model data into Memory (No openGL buffers are created yet)
	loadModels();
}

void MvpModel::loadModels()
{
    referenceModel = new ModelData("D:/FH/6._Semester/Bachelor Arbeit/SoftwareRepository/bb8.obj");
}

