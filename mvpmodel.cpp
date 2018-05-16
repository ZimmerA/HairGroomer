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
    referenceModel = new ModelData("F:/FH/6. Semester/Bachelor Arbeit/SoftwareRepository/Marsienne_Base.obj");
}

