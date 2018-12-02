#ifndef GLMODEL_H
#define GLMODEL_H

#include "glMesh.h"
#include "baseapp/modelData.h"

#include <vector>
#include <memory>

/**
 * \brief The model used by the OpenGL contexts
 */
class GLModel
{
public:

	void draw();
	void draw_points();
	void draw_at_index(int i);
	void draw_points_at_index(int i);
	void setup_model(ModelData *model_data);
	void cleanup_model();

	// Getters/Setters
	unsigned int get_vertex_amount() noexcept;

private:

	std::vector<std::unique_ptr<GLMesh>> m_meshes_;
};

#endif // GLMODEL_H