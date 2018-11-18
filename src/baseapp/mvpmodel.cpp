
#include "mvpmodel.h"

#include <vector>
#include <fstream>
#include <iomanip> // std::setw

#include <QImage>
#include <QFileDialog>
#include <QTextStream>

#include "rendering/glwidget.h"

ProjectSettings MvpModel::load_project_file_from_disk(const QString& filename) const
{
	std::ifstream file(filename.toLocal8Bit().constData());
	json json_object;
	file >> json_object;

	ProjectSettings project;
	project = json_object;

	return project;
}

void MvpModel::save_project_file_to_disk(const QString& filename, const ProjectSettings& proj) const
{
	// Copy the loaded fbx model into the project directory
	if (!proj.m_growthmesh_name.empty() && m_fbx_model_)
	{
		const QString fbx_destination = m_fbx_model_->m_path;
		const QString copy_destination = get_directory_from_path(filename).append(proj.m_growthmesh_name.data());
		// If the fbx file to be copied doesnt exist, cancel
		if (QFile::exists(fbx_destination))
		{
			// If the destination file already exists and it isn't the same as the one to be copied delete it
			if (QFile::exists(copy_destination) && fbx_destination != copy_destination)
			{
				QFile::remove(copy_destination);
			}

			// If the file doesn't exist now, copy the new one over
			if (!QFile::exists(copy_destination))
				QFile::copy(fbx_destination, copy_destination);
		}
	}

	std::ofstream o(filename.toLocal8Bit().constData());
	const json j = proj;
	o << std::setw(4) << j << std::endl;
}

/**
 * \brief Exports the apx Hairfile to the disk
 * \param filename The name of the apx file
 * \param hairdata The data of the hair
 * \param up_axis_index 0 = y, 1 = z
 */
void MvpModel::export_hair_to_disk(const QString& filename, const HairData& hairdata, const int up_axis_index) const
{
	QFile file(filename);
	if (!m_fbx_model_)
		throw std::runtime_error("There is no FBX model loaded");

	const int growthmesh_index = hairdata.m_growthmesh_index;

	if (growthmesh_index >= m_fbx_model_->m_meshes.size())
		throw std::runtime_error("The selected growthmesh could not be found");

	const int num_hair = hairdata.m_num_hair;
	const size_t num_hair_vertices = hairdata.m_vertices.size();

	if (num_hair != m_fbx_model_->m_meshes.at(growthmesh_index).m_vertices.size())
		throw std::runtime_error("The number of hair doesn't match the number of mesh vertices of the growthmesh");

	const int num_faces = m_fbx_model_->m_meshes.at(growthmesh_index).get_num_faces();

	std::vector<unsigned int> face_indices = m_fbx_model_->m_meshes.at(growthmesh_index).get_indices();
	const size_t num_indices = face_indices.size();

	std::vector<glm::vec2> uvs = m_fbx_model_->m_meshes.at(growthmesh_index).get_face_uvs();
	const size_t num_uvs = uvs.size();

	if (num_uvs != num_indices)
		throw std::runtime_error("The number of uv's doesn't match the number of indices");

	const size_t num_bones = m_fbx_model_->get_num_bones() == 0 ? 1 : m_fbx_model_->get_num_bones();
	std::vector<int> bone_indices;
	std::vector<float> bone_weights;

	// Every hair has 4 bone weights and 4 indices
	for (int i = 0; i < num_hair; i++)
	{
		int num_appended = 0;

		if (m_fbx_model_->get_num_bones() > 0)
		{
			// For every bone per Hair Vertex
			for (auto& vertex_bone_info : m_fbx_model_->m_meshes.at(growthmesh_index).m_vertices.at(i).m_bones)
			{
				// max. 4 bones
				if (num_appended < 4)
				{
					const int id = vertex_bone_info.m_id;
					const float weight = vertex_bone_info.m_weight;
					bone_weights.push_back(weight);
					bone_indices.push_back(id);
					num_appended++;
				}
			}
			// Make sure we have 4 bone values per vertex
			while (bone_indices.size() % 4 != 0 || bone_indices.empty())
			{
				bone_indices.push_back(0);
				bone_weights.push_back(0);
			}			
		}else
		{
			for (int i = 0; i < 4; i++)
			{
				if (i == 0)
					bone_weights.push_back(1);
				else
					bone_weights.push_back(0);

				bone_indices.push_back(0);
			}
		}

	}

	std::vector<std::string> bone_name_strings;
	std::vector<int> bone_names; // chars as u8
	std::vector<int> bone_parents;

	if (m_fbx_model_->get_num_bones() > 0)
	{
		// add every bone name and parent to a list and convert the name to ints
		for (auto& bone : m_fbx_model_->m_bone_list)
		{
			bone_name_strings.push_back(bone.m_name);
			for (auto& letter : bone.m_name)
			{
				bone_names.push_back(static_cast<int>(letter));
			}
			bone_parents.push_back(bone.m_parent);
		}
	}else
	{
		bone_name_strings.push_back("hairRoot");
		for (auto& letter : "hairRoot")
		{
			bone_names.push_back(static_cast<int>(letter));
		}
		bone_parents.push_back(-1);
	}

	if (!file.open(QIODevice::WriteOnly))
		throw std::runtime_error("The file you are trying to access could not be opened");

	QTextStream hair_file(&file);

	hair_file << "<!DOCTYPE NvParameters>\n";
	hair_file << "<NvParameters numObjects=\"4\" version=\"1.0\" >\n";
	hair_file << "<value name=\"\" type=\"Ref\" className=\"HairWorksInfo\" version=\"1.1\" checksum=\"\">\n";
	hair_file << "  <struct name=\"\">\n";
	hair_file << "    <value name=\"fileVersion\" type=\"String\">1.2</value>\n";
	hair_file << "    <value name=\"toolVersion\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"sourcePath\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"authorName\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"lastModified\" type=\"String\"></value>\n";
	hair_file << "  </struct>\n";
	hair_file << "</value>\n";
	hair_file << "<value name=\"\" type=\"Ref\" className=\"HairSceneDescriptor\" version=\"1.1\" checksum=\"\">\n";
	hair_file << "  <struct name=\"\">\n";
	hair_file << "    <value name=\"densityTexture\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"rootColorTexture\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"tipColorTexture\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"widthTexture\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"rootWidthTexture\" null=\"1\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"tipWidthTexture\" null=\"1\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"stiffnessTexture\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"rootStiffnessTexture\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"clumpScaleTexture\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"clumpRoundnessTexture\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"clumpNoiseTexture\" null=\"1\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"waveScaletexture\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"waveFreqTexture\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"strandTexture\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"lengthTexture\" type=\"String\"></value>\n";
	hair_file << "    <value name=\"specularTexture\" type=\"String\"></value>\n";
	hair_file << "  </struct>\n";
	hair_file << "</value>\n";
	hair_file << "<value name=\"\" type=\"Ref\" className=\"HairAssetDescriptor\" version=\"1.1\" checksum=\"\">\n";
	hair_file << "  <struct name=\"\">\n";
	hair_file << "    <value name=\"numGuideHairs\" type=\"U32\">" << num_hair << "</value>\n";
	hair_file << "    <value name=\"numVertices\" type=\"U32\">" << num_hair_vertices << "</value>\n";
	// Vertices
	hair_file << "    <array name=\"vertices\" size=\"" << num_hair_vertices << "\" type=\"Vec3\">\n";
	hair_file << "        ";
	for (int i = 0; i < num_hair_vertices; i++)
	{
		const glm::vec3 vertex = hairdata.m_vertices[i];

		hair_file << vertex.x << " " << vertex.y << " " << vertex.z;

		// Don't do anything after the last vertex
		if (i != num_hair_vertices - 1)
		{
			// Set a comma after each vertex
			hair_file << ",";

			if ((i + 1) % 5 == 0)
			{
				// place a newline every 5 vertices
				hair_file << "\n";
				hair_file << "        ";
			}
			else
			{
				// Else just place a space
				hair_file << " ";
			}
		}
	}
	hair_file << "\n    </array>\n";
	// End Indices
	hair_file << "    <array name=\"endIndices\" size=\"" << hairdata.m_num_hair << "\" type=\"U32\">\n";
	hair_file << "        ";
	for (int i = hairdata.m_num_segments; i < hairdata.m_num_hair * (hairdata.m_num_segments + 1); i += hairdata.
	     m_num_segments + 1)
	{
		hair_file << i << " ";
	}
	hair_file << "\n    </array>\n";
	hair_file << "    <value name=\"numFaces\" type=\"U32\">" << num_faces << "</value>\n";
	// Face Indices
	hair_file << "    <array name=\"faceIndices\" size=\"" << num_indices << "\" type=\"U32\">\n";
	hair_file << "        ";
	for (int i = 0; i < num_indices; i++)
	{
		hair_file << face_indices[i];
		if (i != num_indices)
		{
			if ((i + 1) % 15 == 0)
			{
				hair_file << "\n";
				hair_file << "        ";
			}
			else
			{
				hair_file << " ";
			}
		}
	}
	hair_file << "\n     </array>\n";
	// Face uvs
	hair_file << "    <array name=\"faceUVs\" size=\"" << num_uvs << "\" type=\"Vec2\">\n";
	hair_file << "        ";
	for (int i = 0; i < num_uvs; i++)
	{
		const glm::vec2 uv = uvs[i];
		hair_file << uv.x << " " << uv.y;
		if (i != num_uvs - 1)
		{
			hair_file << ",";
			if ((i + 1) % 10 == 0)
			{
				hair_file << "\n";
				hair_file << "        ";
			}
			else
			{
				hair_file << " ";
			}
		}
	}
	hair_file << "\n    </array>\n";
	hair_file << "    <value name=\"numBones\" type=\"U32\">" << num_bones << "</value>\n";
	hair_file << "    <array name=\"boneIndices\" size=\"" << num_hair << "\" type=\"Vec4\">\n";
	hair_file << "        ";
	for (int i = 0; i < bone_indices.size(); i++)
	{
		hair_file << bone_indices[i];
		if (i < bone_indices.size() - 1)
		{
			if ((i + 1) % 4 == 0)
			{
				hair_file << ", ";
			}
			else
			{
				hair_file << " ";
			}
		}
		if ((i + 1) % 64 == 0 && i != 0)
			hair_file << "\n        ";
	}
	hair_file << "\n    </array>\n";

	hair_file << "    <array name=\"boneWeights\" size=\"" << num_hair << "\" type=\"Vec4\">\n";
	hair_file << "        ";
	for (int i = 0; i < bone_weights.size(); i++)
	{
		hair_file << bone_weights[i];
		if (i < bone_weights.size() - 1)
		{
			if ((i + 1) % 4 == 0)
			{
				hair_file << ", ";
			}
			else
			{
				hair_file << " ";
			}
		}
		if ((i + 1) % 64 == 0 && i != 0)
			hair_file << "\n        ";
	}
	hair_file << "\n    </array>\n";

	hair_file << "    <array name=\"boneNames\" size=\"" << bone_names.size() << "\" type=\"U8\">\n";
	hair_file << "        ";
	for (int i = 0; i < bone_names.size(); i++)
	{
		hair_file << bone_names[i];
		if (i < bone_names.size() - 1)
		{
			hair_file << " ";
		}
		else
		{
			hair_file << "\n";
		}

		if ((i + 1) % 32 == 0 && i != 0)
			hair_file << "\n        ";
	}
	hair_file << "    </array>\n";

	hair_file << "    <array name=\"boneNameList\" size=\"" << bone_name_strings.size() << "\" type=\"String\">";
	for (auto& bone_name_string : bone_name_strings)
	{
		hair_file << "\n      <value type=\"String\">" << bone_name_string.data() << "</value>";
	}
	hair_file << "\n    </array>\n";

	hair_file << "    <array name=\"bindPoses\" size=\"" << num_bones << "\" type=\"Mat44\">\n";
	if (m_fbx_model_->get_num_bones() > 0)
	{
		for (int i = 0; i < num_bones; i++)
		{
			for (uint x = 0; x < 4; x++)
			{
				for (uint y = 0; y < 4; y ++)
				{
					glm::mat4 final_bone = m_fbx_model_->m_bone_list[i].m_global_bindpose;

					// Up axis is Z, rotate the skeleton upwards
					if (up_axis_index == 1)
					{
						glm::mat4 rotation_matrix(1);
						rotation_matrix = rotate(rotation_matrix, glm::radians(-90.f), glm::vec3(1.0, 0, 0));
						final_bone = rotation_matrix * final_bone;
					}

					hair_file << final_bone[x][y] << " ";
				}
			}
			if (i < num_bones - 1)
			{
				hair_file << ", ";
			}
		}
	}else
	{
		hair_file << "      1 0 0 0 0 0 -1 0 0 1 0 0 0 0 0 1";
	}
	hair_file << "\n    </array>\n";
	hair_file << "    <array name=\"boneParents\" size=\"" << bone_parents.size() << "\" type=\"I32\">\n";
	hair_file << "        ";
	for (int i = 0; i < bone_parents.size(); i++)
	{
		hair_file << bone_parents[i];
		if (i < bone_parents.size() - 1)
		{
			hair_file << " ";
		}
		else
		{
			hair_file << "\n";
		}

		if ((i + 1) % 32 == 0 && i != 0)
			hair_file << "\n        ";
	}
	hair_file << "    </array>\n";

	hair_file << "    <value name=\"numBoneSpheres\" type=\"U32\">0</value>\n";
	hair_file << "    <array name=\"boneSpheres\" size=\"0\" type=\"Struct\" structElements=\"boneSphereIndex(I32),boneSphereRadius(F32),boneSphereLocalPos(Vec3)\">\n";
	hair_file << "    </array>\n";
	hair_file << "    <value name=\"numBoneCapsules\" type=\"U32\">0</value>\n";
	hair_file << "    <array name=\"boneCapsuleIndices\" size=\"0\" type=\"U32\">\n";
	hair_file << "    </array>\n";
	hair_file << "    <value name=\"numPinConstraints\" type=\"U32\">0</value>\n";
	hair_file <<
		"    <array name=\"pinConstraints\" size=\"0\" type=\"Struct\" structElements=\"boneSphereIndex(I32),boneSphereRadius(F32),boneSphereLocalPos(Vec3),pinStiffness(F32),influenceFallOff(F32),useDynamicPin(Bool),doLra(Bool),useStiffnessPin(Bool),influenceFallOffCurve(Vec4)\"></array>\n";
	hair_file << "    <value name=\"sceneUnit\" type=\"F32\">1</value>\n";
	hair_file << "    <value name=\"upAxis\" type=\"U32\">1</value>\n";
	hair_file << "    <value name=\"handedness\" type=\"U32\">1</value>\n";
	hair_file << "  </struct>\n";
	hair_file << "</value>\n";
	hair_file << "<value name=\"\" type=\"Ref\" className=\"HairInstanceDescriptor\" version=\"1.1\" checksum=\"\">\n";
	hair_file << "  <struct name=\"\">\n";
	hair_file << "    <array name=\"materials\" size=\"4\" type=\"Struct\">\n";
	for (int i = 0; i < 4; ++i)
	{
		hair_file << "      <struct>\n";
		hair_file << "        <value name=\"name\" null=\"1\" type=\"String\"></value>\n";
		hair_file << "        <value name=\"densityTextureChan\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"widthTextureChan\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"rootWidthTextureChan\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"tipWidthTextureChan\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"clumpScaleTextureChan\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"clumpNoiseTextureChan\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"clumpRoundnessTextureChan\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"waveScaleTextureChan\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"waveFreqTextureChan\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"lengthTextureChan\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"stiffnessTextureChan\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"rootStiffnessTextureChan\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"splineMultiplier\" type=\"U32\">1</value>\n";
		hair_file << "        <value name=\"assetType\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"assetPriority\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"assetGroup\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"width\" type=\"F32\">0.25</value>\n";
		hair_file << "        <value name=\"widthNoise\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"clumpNoise\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"clumpNumSubclumps\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"clumpRoundness\" type=\"F32\">1</value>\n";
		hair_file << "        <value name=\"clumpScale\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"clumpPerVertex\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"density\" type=\"F32\">0.150000006</value>\n";
		hair_file << "        <value name=\"lengthNoise\" type=\"F32\">1</value>\n";
		hair_file << "        <value name=\"lengthScale\" type=\"F32\">1</value>\n";
		hair_file << "        <value name=\"widthRootScale\" type=\"F32\">1</value>\n";
		hair_file << "        <value name=\"widthTipScale\" type=\"F32\">0.100000001</value>\n";
		hair_file << "        <value name=\"waveRootStraighten\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"waveScale\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"waveScaleNoise\" type=\"F32\">0.5</value>\n";
		hair_file << "        <value name=\"waveFreq\" type=\"F32\">1</value>\n";
		hair_file << "        <value name=\"waveFreqNoise\" type=\"F32\">0.5</value>\n";
		hair_file << "        <value name=\"waveScaleStrand\" type=\"F32\">1</value>\n";
		hair_file << "        <value name=\"waveScaleClump\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"enableDistanceLOD\" type=\"Bool\">true</value>\n";
		hair_file << "        <value name=\"distanceLODStart\" type=\"F32\">10</value>\n";
		hair_file << "        <value name=\"distanceLODEnd\" type=\"F32\">40</value>\n";
		hair_file << "        <value name=\"distanceLODFadeStart\" type=\"F32\">1000</value>\n";
		hair_file << "        <value name=\"distanceLODDensity\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"distanceLODWidth\" type=\"F32\">1</value>\n";
		hair_file << "        <value name=\"enableDetailLOD\" type=\"Bool\">true</value>\n";
		hair_file << "        <value name=\"detailLODStart\" type=\"F32\">7</value>\n";
		hair_file << "        <value name=\"detailLODEnd\" type=\"F32\">1</value>\n";
		hair_file << "        <value name=\"detailLODDensity\" type=\"F32\">0.100000001</value>\n";
		hair_file << "        <value name=\"detailLODWidth\" type=\"F32\">0.200000003</value>\n";
		hair_file << "        <value name=\"colorizeLODOption\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"useViewfrustrumCulling\" type=\"Bool\">true</value>\n";
		hair_file << "        <value name=\"useBackfaceCulling\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"backfaceCullingThreshold\" type=\"F32\">-0.200000003</value>\n";
		hair_file << "        <value name=\"usePixelDensity\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"alpha\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"strandBlendScale\" type=\"F32\">1</value>\n";
		hair_file << "        <value name=\"baseColor\" type=\"Vec4\">0 0 0 0</value>\n";
		hair_file << "        <value name=\"diffuseBlend\" type=\"F32\">0.5</value>\n";
		hair_file << "        <value name=\"diffuseScale\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"diffuseHairNormalWeight\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"diffuseBoneIndex\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"diffuseBoneLocalPos\" type=\"Vec3\">0 0 0</value>\n";
		hair_file << "        <value name=\"diffuseNoiseFreqU\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"diffuseNoiseFreqV\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"diffuseNoiseScale\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"diffuseNoiseGain\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"textureBrightness\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"diffuseColor\" type=\"Vec4\">0 0 0 0</value>\n";
		hair_file <<
			"        <value name=\"rootColor\" type=\"Vec4\">0.0509803928 0.0156862754 0.0156862754 1</value>\n";
		hair_file << "        <value name=\"tipColor\" type=\"Vec4\">0.121568628 0.0509803928 0.0509803928 1</value>\n";
		hair_file << "        <value name=\"glintStrength\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"glintCount\" type=\"F32\">256</value>\n";
		hair_file << "        <value name=\"glintExponent\" type=\"F32\">2</value>\n";
		hair_file << "        <value name=\"rootAlphaFalloff\" type=\"F32\">0.0500000007</value>\n";
		hair_file << "        <value name=\"rootTipColorWeight\" type=\"F32\">0.439999998</value>\n";
		hair_file << "        <value name=\"rootTipColorFalloff\" type=\"F32\">1</value>\n";
		hair_file << "        <value name=\"shadowSigma\" type=\"F32\">0.200000003</value>\n";
		hair_file <<
			"        <value name=\"specularColor\" type=\"Vec4\">0.400000006 0.443137258 0.474509805 1</value>\n";
		hair_file << "        <value name=\"specularPrimary\" type=\"F32\">0.100000001</value>\n";
		hair_file << "        <value name=\"specularNoiseScale\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"specularEnvScale\" type=\"F32\">0.25</value>\n";
		hair_file << "        <value name=\"specularPrimaryBreakup\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"specularSecondary\" type=\"F32\">0.0500000007</value>\n";
		hair_file << "        <value name=\"specularSecondaryOffset\" type=\"F32\">0.100000001</value>\n";
		hair_file << "        <value name=\"specularPowerPrimary\" type=\"F32\">100</value>\n";
		hair_file << "        <value name=\"specularPowerSecondary\" type=\"F32\">20</value>\n";
		hair_file << "        <value name=\"strandBlendMode\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"useTextures\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"useShadows\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"shadowDensityScale\" type=\"F32\">0.5</value>\n";
		hair_file << "        <value name=\"castShadows\" type=\"Bool\">true</value>\n";
		hair_file << "        <value name=\"receiveShadows\" type=\"Bool\">true</value>\n";
		hair_file << "        <value name=\"backStopRadius\" type=\"F32\">1</value>\n";
		hair_file << "        <value name=\"bendStiffness\" type=\"F32\">0.5</value>\n";
		hair_file << "        <value name=\"interactionStiffness\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"pinStiffness\" type=\"F32\">1</value>\n";
		hair_file << "        <value name=\"collisionOffset\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"useCollision\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"useDynamicPin\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"damping\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"friction\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"massScale\" type=\"F32\">2</value>\n";
		hair_file << "        <value name=\"gravity\" type=\"Vec3\">0 0 -1</value>\n";
		hair_file << "        <value name=\"inertiaScale\" type=\"F32\">1</value>\n";
		hair_file << "        <value name=\"inertiaLimit\" type=\"F32\">100</value>\n";
		hair_file << "        <value name=\"rootStiffness\" type=\"F32\">0.5</value>\n";
		hair_file << "        <value name=\"tipStiffness\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"simulate\" type=\"Bool\">true</value>\n";
		hair_file << "        <value name=\"stiffness\" type=\"F32\">0.5</value>\n";
		hair_file << "        <value name=\"stiffnessStrength\" type=\"F32\">1</value>\n";
		hair_file << "        <value name=\"stiffnessDamping\" type=\"F32\">0.200000003</value>\n";
		hair_file << "        <value name=\"stiffnessCurve\" type=\"Vec4\">1 1 1 1</value>\n";
		hair_file << "        <value name=\"stiffnessStrengthCurve\" type=\"Vec4\">1 1 1 1</value>\n";
		hair_file << "        <value name=\"stiffnessDampingCurve\" type=\"Vec4\">1 1 1 1</value>\n";
		hair_file <<
			"        <value name=\"bendStiffnessCurve\" type=\"Vec4\">1 0.910256386 0.75999999 0.217948735</value>\n";
		hair_file << "        <value name=\"interactionStiffnessCurve\" type=\"Vec4\">1 1 1 1</value>\n";
		hair_file << "        <value name=\"wind\" type=\"Vec3\">0 0 0</value>\n";
		hair_file << "        <value name=\"windNoise\" type=\"F32\">0</value>\n";
		hair_file << "        <value name=\"visualizeBones\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"visualizeBoundingBox\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"visualizeCapsules\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"visualizeControlVertices\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"visualizeCullSphere\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"visualizeDiffuseBone\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"visualizeFrames\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"visualizeGrowthMesh\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"visualizeGuideHairs\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"visualizeHairInteractions\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"visualizeHairSkips\" type=\"U32\">0</value>\n";
		hair_file << "        <value name=\"visualizeLocalPos\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"visualizePinConstraints\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"visualizeShadingNormals\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"visualizeSkinnedGuideHairs\" type=\"Bool\">false</value>\n";
		hair_file << "        <value name=\"drawRenderHairs\" type=\"Bool\">true</value>\n";
		hair_file << "        <value name=\"enable\" type=\"Bool\">true</value>\n";
		hair_file << "      </struct>\n";
	}
	hair_file << "    </array>\n";
	hair_file << "  </struct>\n";
	hair_file << "</value>\n";
	hair_file << "</NvParameters>\n";
}

/**
 * \brief Saves the given hairstyle to disk
 * \param image The image to be saved
 * \param filename The path where the file should be saved
 * \return True if saving succeeded false if saving failed
 */
bool MvpModel::export_hairstyle_to_disk(const QImage& image, const QString& filename) const
{
	// Mirror on y axis because OpenGL flips it
	return image.mirrored().save(filename,"PNG");
}

/**
 * \brief Loads a hairstyle image from the disk
 * \param filename The path of the file to be opened
 * \return The requested image if succeeded, a null QImage image if failed
 */
QImage MvpModel::load_hairstyle_from_disk(const QString& filename) const
{
	QImage hairstyle;

	const bool sucess = hairstyle.load(filename);

	if (!sucess)
		return QImage(); // return a "null image"

	// Mirror the Y-Axis for opengl and set the correct format
	hairstyle = hairstyle.convertToFormat(QImage::Format_RGBA8888).mirrored();

	return hairstyle;
}

void MvpModel::load_fbx_model_from_disk(const QString& filename)
{
	m_fbx_model_ = std::make_unique<ModelData>(filename);
}

void MvpModel::reset_fbx_model()
{
	m_fbx_model_.reset();
}
