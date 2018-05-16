#include "glMesh.h"
#include <qopenglfunctions.h>

GlMesh::GlMesh() 
	: m_ibo(QOpenGLBuffer::IndexBuffer)
{
}

void GlMesh::destroyBuffers()
{
	m_vao.destroy();
	m_vbo.destroy();
	m_ibo.destroy();
}

void GlMesh::setupBuffers(MeshData * meshData)
{
	m_indicieAmount = meshData->indices.size();
	m_vao.create();
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
	m_vbo.create();
	m_ibo.create();

	m_vbo.bind();
	m_vbo.allocate(&meshData->vertices[0], (int)(meshData->vertices.size() * sizeof(Vertex)));

	m_ibo.bind();
	m_ibo.allocate(&meshData->indices[0], (int)(meshData->indices.size() * sizeof(unsigned int)));

	// Setup attrib pointers
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	f->glEnableVertexAttribArray(0);
	f->glEnableVertexAttribArray(1);
	f->glEnableVertexAttribArray(2);

	// position
	f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// normals
	f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// UV
	f->glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));
}

void GlMesh::draw(QOpenGLShaderProgram* shader)
{
	// TODO: Set texture uniforms in shader
	QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
	f->glDrawElements(GL_TRIANGLES, m_indicieAmount, GL_UNSIGNED_INT, 0);
}
