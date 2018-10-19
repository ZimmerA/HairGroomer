#include "glMesh.h"

#include <qopenglfunctions.h>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

GlMesh::GlMesh()
	: m_ibo_(QOpenGLBuffer::IndexBuffer), m_indicie_amount(0)
{
}

/**
 * \brief Destroy all the buffers used by the mesh
 */
void GlMesh::destroy_buffers()
{
	m_vao_.destroy();
	m_vbo_.destroy();
	m_ibo_.destroy();
}

/**
 * \brief Creates the Relevant buffers, fills them with the mesh_data and sets the attribute pointers
 * \param mesh_data The data of the mesh
 */
void GlMesh::setup_buffers(MeshData* mesh_data)
{
	m_indicie_amount = static_cast<unsigned int>(mesh_data->m_indices.size());
	m_vao_.create();
	QOpenGLVertexArrayObject::Binder vao_binder(&m_vao_);
	m_vbo_.create();
	m_ibo_.create();

	m_vbo_.bind();
	m_vbo_.allocate(&mesh_data->m_vertices[0], static_cast<int>(mesh_data->m_vertices.size() * sizeof(Vertex)));

	m_ibo_.bind();
	m_ibo_.allocate(&mesh_data->m_indices[0], static_cast<int>(mesh_data->m_indices.size() * sizeof(unsigned int)));

	m_vertex_count = static_cast<int>(mesh_data->m_vertices.size());
	// Setup attrib pointers
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	f->glEnableVertexAttribArray(0);
	f->glEnableVertexAttribArray(1);
	f->glEnableVertexAttribArray(2);
	f->glEnableVertexAttribArray(3);
	f->glEnableVertexAttribArray(4);

	// position
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(nullptr));
	// normals
	f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                         reinterpret_cast<void*>(offsetof(Vertex, m_normal)));
	// tangent
	f->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                         reinterpret_cast<void*>(offsetof(Vertex, m_tangent)));
	// bitangent
	f->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                         reinterpret_cast<void*>(offsetof(Vertex, m_bitangent)));
	// UV
	f->glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_uv)));
}

/**
 * \brief Draws the indexed mesh
 */
void GlMesh::draw()
{

	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	QOpenGLVertexArrayObject::Binder vao_binder(&m_vao_);
	f->glDrawElements(GL_TRIANGLES, m_indicie_amount, GL_UNSIGNED_INT, nullptr);
}

/**
 * \brief Draws un-indexed mesh in point mode
 */
void GlMesh::draw_points()
{
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	QOpenGLVertexArrayObject::Binder vao_binder(&m_vao_);
	f->glDrawArrays(GL_POINTS, 0, m_vertex_count);
}
