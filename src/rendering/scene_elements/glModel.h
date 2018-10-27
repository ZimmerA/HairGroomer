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
	void draw();
	void draw_points();
	void draw_points_at_index(int i);
	void draw_at_index(int i);

	// Takes model data and creates the according glMeshes
	void setup_model(ModelData* model_data);
	void cleanup_model();
	unsigned int get_vertex_amount() noexcept;

private:
	std::vector<std::unique_ptr<GlMesh>> m_meshes_;
};

#endif // GLMODEL_H
