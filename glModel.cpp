#include "glModel.h"
#include <cassert>

GlModel::GlModel()
= default;

// Iterate over the glMeshes and draw each
void GlModel::draw(QOpenGLShaderProgram* shader)
{
	for (auto& mesh : m_meshes_)
	{
		mesh->draw(shader);
	}
}

// Iterates over every meshData in the modelData and creates a glMesh for it, then sets up the buffers
void GlModel::setup_model(ModelData* model_data)
{
	m_meshes_.clear();
	for (auto& mesh : model_data->m_meshes)
	{
		// Create a unique Pointer to a glMesh and setup the buffers
		std::unique_ptr<GlMesh> temp_mesh(new GlMesh);
		temp_mesh->setup_buffers(&mesh);

		// Move it into the pointer cause we cant create copies of unique pointers
		m_meshes_.push_back(std::move(temp_mesh));
	}
}

// Destroys the buffer of every glmesh, then clears the list of glMeshes
void GlModel::cleanup_model()
{
	for (auto& mesh : m_meshes_)
	{
		mesh->destroy_buffers();
	}
	m_meshes_.clear();
}
