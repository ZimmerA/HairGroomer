
#include "glModel.h"

/**
 * \brief Iterates over each mesh of the model and draws it
 */
void GLModel::draw()
{
	for (auto &mesh : m_meshes_)
		mesh->draw();
}

/**
 * \brief Iterates over each mesh of the model and draws it in point mode
 */
void GLModel::draw_points()
{
	for (auto &mesh : m_meshes_)
		mesh->draw_points();
}

/**
 * \brief Draws the mesh at index i in point mode
 * \param i The index in the m_meshes vector
 */
void GLModel::draw_points_at_index(const int i)
{
	if (i < m_meshes_.size())
		m_meshes_[i]->draw_points();
}

/**
 * \brief Draws the mesh at index i
 * \param i The index in the m_meshes vector
 */
void GLModel::draw_at_index(const int i)
{
	if (i < m_meshes_.size())
		m_meshes_[i]->draw();
}

/**
 * \brief Iterates over every meshData in the modelData and creates a glMesh for it
 * \param model_data The model data
 */
void GLModel::setup_model(ModelData *model_data)
{
	cleanup_model();

	for (auto &mesh : model_data->m_meshes)
	{
		// Create a unique Pointer to a glMesh and setup the buffers
		std::unique_ptr<GLMesh> temp_mesh = std::make_unique<GLMesh>(&mesh);

		// Move it into the vector cause we cant create copies of unique pointers
		m_meshes_.push_back(std::move(temp_mesh));
	}
}

/**
 * \brief Clears the list of glMeshes
 */
void GLModel::cleanup_model()
{
	m_meshes_.clear();
}

/**
 * \brief Returns the sum of vertices of each mesh in the model
 * \return The amount of vertices
 */
unsigned int GLModel::get_vertex_amount() noexcept
{
	unsigned int amount = 0;

	for (auto &mesh : m_meshes_)
		amount += mesh->m_vertex_count;
	
	return amount;
}
