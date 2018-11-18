#ifndef GLMESH_H
#define GLMESH_H

#include "baseapp/meshData.h"

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

/*
* Used by the GLWidget to draw meshes
*/
class GlMesh
{
public:
	explicit GlMesh(MeshData* mesh_data);

	void draw();
	void draw_points();
	
	unsigned int m_indicie_count;
	unsigned int m_vertex_count;

	// don't allow copy or assignment
	GlMesh(const GlMesh&) = delete;
	GlMesh& operator =(const GlMesh&) = delete;

private:
	QOpenGLVertexArrayObject m_vao_;
	QOpenGLBuffer m_vbo_;
	QOpenGLBuffer m_ibo_;
};

#endif // GLMESH_H