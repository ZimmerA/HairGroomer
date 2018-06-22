#include "glMesh.h"
#include <qopenglfunctions.h>

GlMesh::GlMesh()
	: m_ibo_(QOpenGLBuffer::IndexBuffer), m_indicie_amount_(0)
{
}

void GlMesh::destroy_buffers()
{
	m_vao_.destroy();
	m_vbo_.destroy();
	m_ibo_.destroy();
}

void GlMesh::setup_buffers(MeshData* mesh_data)
{
	m_indicie_amount_ = static_cast<unsigned int>(mesh_data->m_indices.size());
	m_vao_.create();
	QOpenGLVertexArrayObject::Binder vao_binder(&m_vao_);
	m_vbo_.create();
	m_ibo_.create();

	m_vbo_.bind();
	m_vbo_.allocate(&mesh_data->m_vertices[0], static_cast<int>(mesh_data->m_vertices.size() * sizeof(Vertex)));

	m_ibo_.bind();
	m_ibo_.allocate(&mesh_data->m_indices[0], static_cast<int>(mesh_data->m_indices.size() * sizeof(unsigned int)));

	// Setup attrib pointers
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	f->glEnableVertexAttribArray(0);
	f->glEnableVertexAttribArray(1);
	f->glEnableVertexAttribArray(2);

	// position
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(nullptr));
	// normals
	f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
	                         reinterpret_cast<void*>(offsetof(Vertex, m_normal)));
	// UV
	f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, m_uv)));
}

void GlMesh::draw(QOpenGLShaderProgram* shader)
{
	// TODO: Set texture uniforms in shader
	QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
	QOpenGLVertexArrayObject::Binder vao_binder(&m_vao_);
	f->glDrawElements(GL_TRIANGLES, m_indicie_amount_, GL_UNSIGNED_INT, nullptr);
}
