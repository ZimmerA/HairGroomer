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
	void draw(QOpenGLShaderProgram * shader);
	// Creates buffers and fills them with the mesh data
	void setupBuffers(MeshData * mesh);
	// Calls destroy on all buffers of the mesh
	void destroyBuffers();

private:
	QOpenGLVertexArrayObject m_vao;
	QOpenGLBuffer m_vbo;
	QOpenGLBuffer m_ibo;
	
	// To be used in the GlDrawElements Function
	unsigned int m_indicieAmount; 

};

#endif // GLMESH_H