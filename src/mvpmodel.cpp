#include "mvpmodel.h"
#include <qimage.h>
#include <QFileDialog>

MvpModel::MvpModel()
{
	// Load the Model data into Memory (No openGL buffers are created yet)
	load_models();
	default_settings();
}

void MvpModel::load_models()
{
	m_reference_model_ = new ModelData("./res/eva3.fbx");
}

/**
 * \brief Stores the default settings of the ui control elements
 */
void MvpModel::default_settings()
{
	// General
	m_grid_visibility_default = true;

	// Hair
	m_hairlength_default = 3.00;
	m_hairsegment_count_default = 4;
	m_hair_color_default.setRgb(255,255,255);
	m_hair_root_color_default.setRgb(0,0,0);

	// Brush
	m_brushmode_default = Paintbrush::paintmode::length;
	m_brush_size_default = 0.10;
	m_brush_intensity_default = 1.0;

	// Light
	m_light_hair_default = true;
	m_light_color_default.setRgb(255,255,255);
}

/**
 * \brief Reserved for future versions of the Software
 * \param filename The name of the apx file
 * \return true if suceeded, false if failed
 */
bool MvpModel::export_hair_to_disk(const QString& filename)
{
	QFile file(filename);

	if(!file.open(QIODevice::WriteOnly))
		return false;

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
	hair_file << "    <value name=\"numGuideHairs\" type=\"U32\"></value>\n"; // TODO: Num guidehairs
	hair_file << "    <value name=\"numVertices\" type=\"U32\"></value>\n"; // TODO: fill Num vertices
	hair_file << "    <array name=\"vertices\" size=\"\" type=\"Vec3\">\n"; // TODO: size
	// TODO: add vertices
	hair_file << "    </array>\n";

	hair_file << "    <array name=\"endIndices\" size=\"\" type=\"U32\">\n"; // TODO: size
	// TODO: add endindices
	hair_file << "    </array>\n";

	hair_file << "    <value name=\"numFaces\" type=\"U32\"></value>\n"; // TODO: fill numFaces
	hair_file << "    <array name=\"faceIndices\" size=\"\" type=\"U32\">\n"; // TODO: size
	// TODO: add faceindices
	hair_file << "    </array>\n";

	hair_file << "    <array name=\"faceUVs\" size=\"\" type=\"Vec2\">\n"; // TODO: size
	// TODO: add faceUVs
	hair_file << "    </array>\n";

	hair_file << "    <value name=\"numBones\" type=\"U32\"></value>\n"; // TODO: fill numBones
	hair_file << "    <array name=\"boneIndices\" size=\"\" type=\"Vec4\">\n"; // TODO: size
	// TODO: add boneindices
	hair_file << "    </array>\n";

	hair_file << "    <array name=\"boneWeights\" size=\"\" type=\"Vec4\">\n"; // TODO: size
	// TODO: add boneweights
	hair_file << "    </array>\n";

	hair_file << "    <array name=\"boneNames\" size=\"\" type=\"U8\">\n"; // TODO: size
	// TODO: add bonenames
	hair_file << "    </array>\n";
	hair_file << "    <array name=\"boneNameList\" size=\"\" type=\"String\">\n"; // TODO: size
	// TODO: add bonenamelist
	hair_file << "    </array>\n";

	hair_file << "    <array name=\"bindPoses\" size=\"\" type=\"Mat44\">\n"; // TODO: size
	// TODO: add bindposes
	hair_file << "    </array>\n";
	hair_file << "    <array name=\"boneParents\" size=\"\" type=\"I32\">\n"; // TODO: size
	// TODO: add boneparents
	hair_file << "    </array>\n";

	hair_file << "    <value name=\"numBoneSpheres\" type=\"U32\"></value>\n"; // TODO: fill numBoneSpheres
	hair_file <<
		"    <array name=\"boneSpheres\" size=\"\" type=\"Struct\" structElements=\"boneSphereIndex(I32),boneSphereRadius(F32),boneSphereLocalPos(Vec3)\">\n";
	// TODO: size
	//TODO add boneSpheres
	hair_file << "    </array>\n";

	hair_file << "    <value name=\"numBoneCapsules\" type=\"U32\"></value>\n"; // TODO: fill numBoneCapsules
	hair_file << "    <array name=\"boneCapsuleIndices\" size=\"\" type=\"U32\">\n"; // TODO size
	// TODO add boncapsuleIndices
	hair_file << "    </array>\n";

	hair_file << "    <value name=\"numPinConstraints\" type=\"U32\">0</value>\n"; // TODO was ist das und folgende
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

	return true;
}


/**
 * \brief Saves the given hairstyle to disk
 * \param image The image to be saved
 * \param filename The path where the file should be saved
 * \return True if saving succeeded false if saving failed
 */
bool MvpModel::export_hairstyle_to_disk(const QImage& image, const QString& filename)
{
	return image.mirrored().save(filename);
}

/**
 * \brief Loads a hairstyle image from the disk
 * \param filename The path of the file to be opened
 * \return The requested image if succeeded, a null QImage image if failed
 */
QImage MvpModel::load_hairstyle_from_disk(const QString& filename)
{
	QImage hairstyle;

	const bool sucess = hairstyle.load(filename);
	
	if(!sucess)
		return QImage(); // return a null image

	// Flip the image for opengl and set the correct format
	hairstyle = hairstyle.convertToFormat(QImage::Format_RGBA8888).mirrored();

	return hairstyle;
}
