#include "glModel.h"
#include <assert.h>
GlModel::GlModel()
{
}

// Iterate over the glMeshes and draw each
void GlModel::draw(QOpenGLShaderProgram * shader)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
        meshes[i]->draw(shader);
	}
}

// Iterates over every meshData in the modelData and creates a glMesh for it, then sets up the buffers
void GlModel::setupModel(ModelData * modelData)
{
	meshes.clear();
	for (unsigned int i = 0; i < modelData->meshes.size(); i++)
	{
		// Create a unique Pointer to a glMesh and setup the buffers
        std::unique_ptr <GlMesh> tempMesh(new GlMesh);
        tempMesh->setupBuffers(&modelData->meshes.at(i));

		// Move it into the pointer cause we cant create copies of unique pointers
        meshes.push_back(std::move(tempMesh));
	}
}

// Destroys the buffer of every glmesh, then clears the list of glMeshes
void GlModel::cleanupModel()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes.at(i)->destroyBuffers();
	}
	meshes.clear();
}
