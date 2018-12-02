#ifndef GLMESH_H
#define GLMESH_H

#include "baseapp/meshData.h"

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

/*
* Used by the GLWidget to draw meshes
*/
class GLMesh
{
public:

	unsigned int m_index_count;
	unsigned int m_vertex_count;
	
	explicit GLMesh(MeshData *mesh_data);

	void draw();
	void draw_points();
	
private:

	// Don't allow copy or assignment
	GLMesh(const GLMesh&) = delete;
	GLMesh& operator =(const GLMesh&) = delete;

	QOpenGLVertexArrayObject m_vao_;
	QOpenGLBuffer m_vbo_;
	QOpenGLBuffer m_ibo_;
};

#endif // GLMESH_H