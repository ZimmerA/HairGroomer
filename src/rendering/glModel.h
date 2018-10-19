#ifndef GLMODEL_H
#define GLMODEL_H
#include <vector>
#include "glMesh.h"
#include "./modelData.h"
#include <memory> // unique pointer

/**
 * \brief The model used by the OpenGL contexts
 */
class GlModel
{
public:
	GlModel();
	void draw();
	void draw_points();

	// Takes modeldata and creates the according glMeshes
	void setup_model(ModelData* model_data);
	void cleanup_model();

private:
	vector<std::unique_ptr<GlMesh>> m_meshes_;
};

#endif // GLMODEL_H
