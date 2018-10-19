#include "glModel.h"

GlModel::GlModel()
= default;

/**
 * \brief Iterates over each mesh of the model and draws it
 */
void GlModel::draw()
{
	for (auto& mesh : m_meshes_)
	{
		mesh->draw();
	}
}

/**
 * \brief Iterates over each mesh of the model and draws it in un-indexed point mode
 */
void GlModel::draw_points()
{
	
	//for (auto& mesh : m_meshes_)
	//{
		m_meshes_.at(1)->draw_points();
	//}
}
// 
/**
 * \brief Iterates over every meshData in the modelData and creates a glMesh for it, then sets up the buffers
 * \param model_data The model data
 */
void GlModel::setup_model(ModelData* model_data)
{
	m_meshes_.clear();
	for (auto& mesh : model_data->m_meshes)
	{
		// Create a unique Pointer to a glMesh and setup the buffers
		std::unique_ptr<GlMesh> temp_mesh = std::make_unique<GlMesh>();
		temp_mesh->setup_buffers(&mesh);

		// Move it into the vector cause we cant create copies of unique pointers
		m_meshes_.push_back(std::move(temp_mesh));
	}
}


/**
 * \brief Destroys the buffer of every glmesh, then clears the list of glMeshes
 */
void GlModel::cleanup_model()
{
	for (auto& mesh : m_meshes_)
	{
		mesh->destroy_buffers();
	}
	m_meshes_.clear();
}

unsigned int GlModel::get_vertice_amount()
{
	unsigned int amount = 0;
	for(auto& mesh : m_meshes_)
	{
		amount += mesh->m_vertex_count;
	}
	return amount;
}
