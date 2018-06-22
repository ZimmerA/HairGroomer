#ifndef MODEL_H
#define MODEL_H

#include <QOpenGLShaderProgram>
#include <vector>
#include <meshData.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

using namespace std;

class ModelData
{
public:
	explicit ModelData(const char* path);
	// Meshdata for every submodel
	vector<MeshData> m_meshes;
private:
	string m_directory_;

	void load_model(const string& path);
	// Process a node in the model tree
	void process_node(aiNode* node, const aiScene* scene);
	// Create a meshData object out of the data
	static MeshData process_mesh(aiMesh* mesh, const aiScene* scene);
};

#endif // MODEL_H
