#ifndef GLMODEL_H
#define GLMODEL_H

#include "glMesh.h"
#include "baseapp/modelData.h"

#include <vector>
#include <memory>

/**
 * \brief The model used by the OpenGL contexts
 */
class GlModel
{
public:
	GlModel();
	void draw();
	void draw_points();

	// Takes model data and creates the according glMeshes
	void setup_model(ModelData* model_data);
	void cleanup_model();
	unsigned int get_vertice_amount();

private:
	vector<std::unique_ptr<GlMesh>> m_meshes_;
};

#endif // GLMODEL_H
