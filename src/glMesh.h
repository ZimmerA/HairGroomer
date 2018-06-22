#ifndef GLMESH_H
#define GLMESH_H

#include <qopenglshaderprogram.h>
#include <qopenglvertexarrayobject.h>
#include <qopenglbuffer.h>
#include <meshData.h>

/*
* Used by the GLWidget to draw meshes
*/
class GlMesh
{
public:
	GlMesh();
	void draw(QOpenGLShaderProgram* shader);
	// Creates buffers and fills them with the mesh data
	void setup_buffers(MeshData* mesh_data);
	// Calls destroy on all buffers of the mesh
	void destroy_buffers();

private:
	QOpenGLVertexArrayObject m_vao_;
	QOpenGLBuffer m_vbo_;
	QOpenGLBuffer m_ibo_;

	// To be used in the GlDrawElements Function
	unsigned int m_indicie_amount_;
};

#endif // GLMESH_H
