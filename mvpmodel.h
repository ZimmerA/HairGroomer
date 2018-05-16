#ifndef MVPMODEL_H
#define MVPMODEL_H
#include <modelData.h>

/*
*	
*
*/
class MvpModel
{
public:

	MvpModel();
	void loadModels();

	// Getters/Setters
	ModelData * getReferenceModel() const { return referenceModel; }
	ModelData * getGrowthMesh() const { return growthMesh; }

private:
	ModelData * referenceModel, *growthMesh;
};

#endif // MVPMODEL_H
