
#include "glMesh.h"

#include <qopenglfunctions.h>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "rendering/glattributes.h"

/**
 * \brief Loads the given MeshData into an openGl buffer and sets the attribute pointers 
 * \param mesh_data The meshdata
 */
GLMesh::GLMesh(MeshData *mesh_data) : m_index_count(0), m_vertex_count(0), m_ibo_(QOpenGLBuffer::IndexBuffer)
{
	m_index_count = static_cast<unsigned int>(mesh_data->m_indices.size());
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
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glEnableVertexAttribArray(ATTRIBUTE_POSITION);
	f->glEnableVertexAttribArray(ATTRIBUTE_NORMAL);
	f->glEnableVertexAttribArray(ATTRIBUTE_TANGENT);
	f->glEnableVertexAttribArray(ATTRIBUTE_BITANGENT);
	f->glEnableVertexAttribArray(ATTRIBUTE_TEXCOORD);

	f->glVertexAttribPointer(ATTRIBUTE_POSITION,  3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(nullptr));
	f->glVertexAttribPointer(ATTRIBUTE_NORMAL,    3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_normal)));
	f->glVertexAttribPointer(ATTRIBUTE_TANGENT,   3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_tangent)));
	f->glVertexAttribPointer(ATTRIBUTE_BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_bitangent)));
	f->glVertexAttribPointer(ATTRIBUTE_TEXCOORD,  2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_uv)));
}

/**
 * \brief Draws the indexed mesh
 */
void GLMesh::draw()
{
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	QOpenGLVertexArrayObject::Binder vao_binder(&m_vao_);
	f->glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, nullptr);
}

/**
 * \brief Draws mesh in un-indexed point mode
 */
void GLMesh::draw_points()
{
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	QOpenGLVertexArrayObject::Binder vao_binder(&m_vao_);
	f->glDrawArrays(GL_POINTS, 0, m_vertex_count);
}
