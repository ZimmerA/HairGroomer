#ifndef MODEL_H
#define MODEL_H

#include "meshData.h"
#include <vector>
#include <QOpenGLShaderProgram>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

using namespace std;

/**
 * \brief Loads and stores all the data of a model, contains a list of MeshData objects that make up the model
 */
class ModelData
{
public:
	explicit ModelData(const char* path);

	// MeshData for every sub model
	vector<MeshData> m_meshes;
private:
	string m_directory_;
	void load_model(const string& path);

	// Processes Assimp node in the model tree
	void process_node(aiNode* node, const aiScene* scene);

	// Create a meshData object from the Assimp data
	static MeshData process_mesh(aiMesh* mesh);
};

#endif // MODEL_H
