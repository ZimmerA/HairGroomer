
#include "modelData.h"

#include <unordered_map>
#include <sstream>

#include <QDebug>

/**
 * \brief The constructor of the model loads the model at the specified path
 * \param path The path to the fbx model
 */
ModelData::ModelData(const QString& path)
{
	load_model(path);
}

/**
 * \brief Loads the model at the given path using the Fbx sdk
 * \param path The path to the model on the harddrive
 */
void ModelData::load_model(const QString& path)
{
	FbxPointer<FbxManager> sdk_manager(FbxManager::Create());
	
	FbxPointer<FbxIOSettings> io_settings(FbxIOSettings::Create(sdk_manager.get(), IOSROOT));
	sdk_manager->SetIOSettings(io_settings.get());

	FbxPointer<FbxImporter> importer(FbxImporter::Create(sdk_manager.get(), ""));

	const bool import_status = importer->Initialize(path.toLocal8Bit().constData(), -1, sdk_manager->GetIOSettings());
	if (!import_status)
	{
		std::stringstream error_stream;
		error_stream << "Mesh couldn't be imported: " << importer->GetStatus().GetErrorString();
		throw std::runtime_error(error_stream.str());
	}

	m_path = path;
	
	FbxPointer<FbxScene> scene(FbxScene::Create(sdk_manager.get(), "myScene"));

	// Import the contents of the file into the scene.
	importer->Import(scene.get());

	// File format version numbers to be populated.
	int file_major, file_minor, file_revision;
	// Populate the FBX file format version numbers with the import file.
	importer->GetFileVersion(file_major, file_minor, file_revision);

	// Traverse Nodes to load skeleton and model data
	process_skeleton_hierachy(scene->GetRootNode(), -1);
	process_mesh_nodes(scene->GetRootNode(), sdk_manager.get());

	if (m_meshes.empty())
	{
		throw std::runtime_error("The loaded model doesn't have any meshes");
	}
}

/**
 * \brief Recursively iterates over every child node. If it is a bone, its added to the list of bones
 * \param node The current Fbx node
 * \param parent_index The index of the parent bone
 */
void ModelData::process_skeleton_hierachy(FbxNode* node, const int parent_index)
{
	uint next_parent_index = parent_index;
	if (node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() && node->GetNodeAttribute()->GetAttributeType() ==FbxNodeAttribute::eSkeleton)
	{
		const std::string bone_name = node->GetName();

		// Check if the bone is in the map, if no add it
		if (m_bone_map_.find(bone_name) == m_bone_map_.end())
		{
			const uint current_bone_index = static_cast<uint>(m_bone_map_.size());
			Bone bone;

			bone.m_name = bone_name;
			bone.m_parent = parent_index;

			m_bone_list.push_back(bone);
			m_bone_map_[bone_name] = current_bone_index;

			next_parent_index = current_bone_index;
		}
		else // Bone is already in the map get the index of the bone
		{
			next_parent_index = m_bone_map_[bone_name];
		}
	}

	for (int i = 0; i < node->GetChildCount(); i++)
	{
		process_skeleton_hierachy(node->GetChild(i), next_parent_index);
	}
}

/**
 * \brief Recursively iterates over every node. Calls the process_mesh function to add it to the list of meshes if the node is a mesh
 *  * \param node The current Fbx node
 * \param manager The Fbx Sdk manager
 */
void ModelData::process_mesh_nodes(FbxNode* node, FbxManager* manager)
{
	if (node->GetNodeAttribute() != nullptr)
	{
		// Get the node type
		const FbxNodeAttribute::EType attribute_type = node->GetNodeAttribute()->GetAttributeType();

		// Mesh
		if (attribute_type == FbxNodeAttribute::eMesh)
		{
			FbxMesh* mesh = node->GetMesh();
			m_meshes.push_back(process_mesh(mesh, manager,node->GetName()));
		}
	}

	// Process child nodes
	for (int i = 0; i < node->GetChildCount(); i++)
		process_mesh_nodes(node->GetChild(i), manager);
}

/**
 * \brief Tries to retrieve the data of the Fbx mesh and creates a MeshData object to be added to the list of meshes
 * \param mesh The current Fbx mesh
 * \param manager The Fbx Sdk manager
 * \param mesh_name The name of the mesh
 * \return A MeshData object that contains the data of the given Fbx mesh
 */
MeshData ModelData::process_mesh(FbxMesh* mesh, FbxManager* manager, const std::string mesh_name)
{
	// Triangulate mesh if needed
	if (!mesh->IsTriangleMesh())
	{
		FbxGeometryConverter converter(manager);
		mesh = static_cast<FbxMesh*>(converter.Triangulate(mesh, true));

		if (!mesh)
		{
			std::stringstream error_stream;
			error_stream << "Mesh" << mesh_name << " couldn't be triangulated";
		}
	}

	// Generate Tangent/Binormals if needed
	if ((mesh->GetElementBinormalCount() <= 0) || (mesh->GetElementTangentCount() <= 0))
		mesh->GenerateTangentsData(0, true);

	// Preload control points to make skeleton loading easier
	std::vector<ControlPointInfo> control_points;
	const unsigned int controlpoint_count = mesh->GetControlPointsCount();

	for (unsigned int i = 0; i < controlpoint_count; ++i)
	{
		ControlPointInfo controlpoint;
		const FbxVector4 position = mesh->mControlPoints[i];
		controlpoint.m_position.x = position.mData[0];
		controlpoint.m_position.y = position.mData[1];
		controlpoint.m_position.z = position.mData[2];
		control_points.push_back(controlpoint);
	}

	// Load Vertex Bone info
	const unsigned int num_deformers = mesh->GetDeformerCount();

	for (unsigned int deformer_index = 0; deformer_index < num_deformers; ++deformer_index)
	{
		auto *curr_skin = reinterpret_cast<FbxSkin*>(mesh->GetDeformer(deformer_index, FbxDeformer::eSkin));
		
		// Deformer isn't a skin
		if (!curr_skin)
			continue;

		const unsigned int num_clusters = curr_skin->GetClusterCount();
		for (unsigned int cluster_index = 0; cluster_index < num_clusters; ++cluster_index)
		{
			FbxCluster* curr_cluster = curr_skin->GetCluster(cluster_index);
			std::string bone_name = curr_cluster->GetLink()->GetName();
			const int bone_id = m_bone_map_[bone_name];

			FbxAMatrix transform_matrix;
			FbxAMatrix transform_link_matrix;
			// The transformation of the mesh at binding time
			transform_matrix = curr_cluster->GetTransformMatrix(transform_matrix);
			// The transformation of the cluster(joint) at binding time from joint space to world space
			transform_link_matrix = curr_cluster->GetTransformLinkMatrix(transform_link_matrix);
			FbxAMatrix global_bindpose = (transform_link_matrix.Inverse() * transform_matrix).Inverse();

			// set the bindpose Matrix in the bone
			for (int x = 0; x < 4; ++x)
				for (int y = 0; y < 4; ++y)
					m_bone_list[bone_id].m_global_bindpose[x][y] = global_bindpose[x][y];

			// Get amount of control points this bone affects
			const unsigned int num_indices = curr_cluster->GetControlPointIndicesCount();

			// Assign the weight and bone id info to the control points
			for (unsigned int i = 0; i < num_indices; ++i)
			{
				VertexBoneInfo bone_info{};
				bone_info.m_id = bone_id;
				bone_info.m_weight = curr_cluster->GetControlPointWeights()[i];
				control_points.at(curr_cluster->GetControlPointIndices()[i]).m_bone_info.push_back(bone_info);
			}
		}
	}

	// Load vertices
	const int num_triangles = mesh->GetPolygonCount();
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// Hashmap for finding duplicate vertices
	std::unordered_map<Vertex, uint32_t> unique_vertices;

	// Used for reading out normals, uv, tangent, binormal that have a per polygon vertex mapping
	int index_count = 0;

	// Iterate over every triangle in the mesh
	for (int triangle = 0; triangle < num_triangles; ++triangle)
	{
		// 3 points per polygon (triangulated)
		for (int index_in_polygon = 0; index_in_polygon < 3; ++index_in_polygon)
		{
			Vertex vert;

			const int control_point_index = mesh->GetPolygonVertex(triangle, index_in_polygon);

			const ControlPointInfo control_point = control_points.at(control_point_index);

			// Get the vertex info
			vert.m_position = control_point.m_position;
			vert.m_normal = read_normal(mesh, control_point_index, index_count);
			vert.m_uv = read_uv(mesh, control_point_index, index_count);
			vert.m_tangent = read_tangent(mesh, control_point_index, index_count);
			vert.m_bitangent = read_binormal(mesh, control_point_index, index_count);
			vert.m_bones = control_point.m_bone_info;

			// Check if this vertex is already in the list of unique verts, if no add it
			if (unique_vertices.count(vert) == 0)
			{
				unique_vertices[vert] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vert);
			}

			// Get the correct index of the vert using the hashmap
			indices.push_back(unique_vertices[vert]);
			++index_count;
		}
	}
	m_name_list.push_back(mesh_name);
	return MeshData(mesh_name, vertices, indices, num_triangles);
}

/**
 * \brief Loads the normals for the given vertex
 * \param mesh The current Fbx mesh
 * \param control_point_index The index of the current control point of this vertex
 * \param vertex_counter The according vertex
 * \return A glm::vec3 containing the normal of the vertex
 */
glm::vec3 ModelData::read_normal(FbxMesh* mesh, const int control_point_index, const int vertex_counter)
{
	glm::vec3 out_normal(0, 0, 0);

	if (mesh->GetElementNormalCount() >= 1)
	{
		FbxGeometryElementNormal* vertex_normal = mesh->GetElementNormal(0);
		switch (vertex_normal->GetMappingMode())
		{
			// Normal is per Control point
		case FbxGeometryElement::eByControlPoint:
			switch (vertex_normal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				{
					out_normal.x = vertex_normal->GetDirectArray().GetAt(control_point_index).mData[0];
					out_normal.y = vertex_normal->GetDirectArray().GetAt(control_point_index).mData[1];
					out_normal.z = vertex_normal->GetDirectArray().GetAt(control_point_index).mData[2];
				}
				break;
			case FbxGeometryElement::eIndexToDirect:
				{
					const int normal_index = vertex_normal->GetIndexArray().GetAt(control_point_index);
					out_normal.x = vertex_normal->GetDirectArray().GetAt(normal_index).mData[0];
					out_normal.y = vertex_normal->GetDirectArray().GetAt(normal_index).mData[1];
					out_normal.z = vertex_normal->GetDirectArray().GetAt(normal_index).mData[2];
				}
				break;
			default:
				qDebug() << "Invalid ref";
				break;
			}
			break;
			// Normal is per Polygon Vert
		case FbxGeometryElement::eByPolygonVertex:
			switch (vertex_normal->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				{
					out_normal.x = vertex_normal->GetDirectArray().GetAt(vertex_counter).mData[0];
					out_normal.y = vertex_normal->GetDirectArray().GetAt(vertex_counter).mData[1];
					out_normal.z = vertex_normal->GetDirectArray().GetAt(vertex_counter).mData[2];
				}
				break;
			case FbxGeometryElement::eIndexToDirect:
				{
					const int normal_index = vertex_normal->GetIndexArray().GetAt(vertex_counter);
					out_normal.x = vertex_normal->GetDirectArray().GetAt(normal_index).mData[0];
					out_normal.y = vertex_normal->GetDirectArray().GetAt(normal_index).mData[1];
					out_normal.z = vertex_normal->GetDirectArray().GetAt(normal_index).mData[2];
				}
				break;
			default:
				qDebug() << "Invalid ref";
			}
			break;
		default:
			qDebug() << "Invalid ref";
			break;
		}
	}
	else
	{
		qDebug() << "Mesh doesn't have normals";
	}

	return out_normal;
}

/**
 * \brief Loads the UV for the given vertex
 * \param mesh The current Fbx mesh
 * \param control_point_index The index of the current control point of this vertex
 * \param vertex_counter The according vertex
 * \return A glm::vec2 containing the UV of the vertex
 */
glm::vec2 ModelData::read_uv(FbxMesh* mesh, const int control_point_index, const int vertex_counter)
{
	glm::vec2 uv(0, 0);

	if (mesh->GetElementUVCount() >= 1)
	{
		FbxGeometryElementUV* uv_elemnt = mesh->GetElementUV(0);

		const bool lUseIndex = uv_elemnt->GetReferenceMode() != FbxGeometryElement::eDirect;

		if (uv_elemnt->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			//the UV index depends on the reference mode
			const int uv_index = lUseIndex
			                     ? uv_elemnt->GetIndexArray().GetAt(control_point_index)
			                     : control_point_index;

			const FbxVector2 uv_value = uv_elemnt->GetDirectArray().GetAt(uv_index);

			uv.x = uv_value.mData[0];
			uv.y = uv_value.mData[1];
		}
		else if (uv_elemnt->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			//the UV index depends on the reference mode
			const int uv_index = lUseIndex ? uv_elemnt->GetIndexArray().GetAt(vertex_counter) : vertex_counter;

			const FbxVector2 uv_value = uv_elemnt->GetDirectArray().GetAt(uv_index);
			uv.x = uv_value.mData[0];
			uv.y = uv_value.mData[1];
		}
	}
	else
	{
		qDebug() << "Mesh doesn't have uv layer";
	}

	return uv;
}

/**
 * \brief Loads the binormal for the given vertex
 * \param mesh The current Fbx mesh
 * \param control_point_index The index of the current control point of this vertex
 * \param vertex_counter The according vertex
 * \return A glm::vec3 containing the binormal of the vertex
 */
glm::vec3 ModelData::read_binormal(FbxMesh* mesh, const int control_point_index, const int vertex_counter)
{
	glm::vec3 binormal(0, 0, 0);

	if (mesh->GetElementBinormalCount() >= 1)
	{
		FbxGeometryElementBinormal* binormal_element = mesh->GetElementBinormal(0);

		const bool uses_index = binormal_element->GetReferenceMode() != FbxGeometryElement::eDirect;

		if (binormal_element->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			// The UV index depends on the reference mode
			const int binormal_index = uses_index
			                           ? binormal_element->GetIndexArray().GetAt(control_point_index)
			                           : control_point_index;

			const FbxVector4 binormal_value = binormal_element->GetDirectArray().GetAt(binormal_index);
			binormal.x = binormal_value.mData[0];
			binormal.y = binormal_value.mData[1];
			binormal.z = binormal_value.mData[2];
		}
		else if (binormal_element->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			// The UV index depends on the reference mode
			const int binormal_index = uses_index ? binormal_element->GetIndexArray().GetAt(vertex_counter) : vertex_counter;

			const FbxVector4 binormal_value = binormal_element->GetDirectArray().GetAt(binormal_index);
			binormal.x = binormal_value.mData[0];
			binormal.y = binormal_value.mData[1];
			binormal.z = binormal_value.mData[2];
		}
	}
	else
	{
		qDebug() << "Mesh doesn't have binormals";
	}

	return binormal;
}

/**
 * \brief Loads the tangent for the given vertex
 * \param mesh The current Fbx mesh
 * \param control_point_index The index of the current control point of this vertex
 * \param vertex_counter The according vertex
 * \return A glm::vec3 containing the tangent of the vertex
 */
glm::vec3 ModelData::read_tangent(FbxMesh* mesh, const int control_point_index, const int vertex_counter)
{
	glm::vec3 tangent(0, 0, 0);

	if (mesh->GetElementTangentCount() >= 1)
	{
		FbxGeometryElementTangent* tangent_element = mesh->GetElementTangent(0);

		const bool uses_index = tangent_element->GetReferenceMode() != FbxGeometryElement::eDirect;

		if (tangent_element->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			// The UV index depends on the reference mode
			const int tangent_index = uses_index
			                          ? tangent_element->GetIndexArray().GetAt(control_point_index)
			                          : control_point_index;

			const FbxVector4 tangent_value = tangent_element->GetDirectArray().GetAt(tangent_index);
			tangent.x = tangent_value.mData[0];
			tangent.y = tangent_value.mData[1];
			tangent.z = tangent_value.mData[2];
		}
		else if (tangent_element->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			// The UV index depends on the reference mode
			const int tangent_index = uses_index
			                          ? tangent_element->GetIndexArray().GetAt(vertex_counter)
			                          : vertex_counter;
			
			const FbxVector4 tangent_value = tangent_element->GetDirectArray().GetAt(tangent_index);
			tangent.x = tangent_value.mData[0];
			tangent.y = tangent_value.mData[1];
			tangent.z = tangent_value.mData[2];
		}
	}
	else
	{
		qDebug() << "Mesh doesn't have tangents";
	}

	return tangent;
}
