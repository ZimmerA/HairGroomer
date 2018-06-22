#include "mvpmodel.h"

MvpModel::MvpModel()
{
	// Load the Model data into Memory (No openGL buffers are created yet)
	load_models();
}

void MvpModel::load_models()
{
	m_reference_model_ = new ModelData("D:/FH/6._Semester/Bachelor Arbeit/SoftwareRepository/Marsienne_Base.obj");
}
