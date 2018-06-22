#include "mvppresenter.h"

MvpPresenter::MvpPresenter(): m_model_(nullptr), m_view_(nullptr)
{
}

void MvpPresenter::export_hair(QFile* file)
{
	QTextStream hairFile(file);

	hairFile << "<!DOCTYPE NvParameters>\n";
	hairFile << "<NvParameters numObjects=\"4\" version=\"1.0\" >\n";
	hairFile << "<value name=\"\" type=\"Ref\" className=\"HairWorksInfo\" version=\"1.1\" checksum=\"\">\n";
	hairFile << "  <struct name=\"\">\n";
	hairFile << "    <value name=\"fileVersion\" type=\"String\">1.2</value>\n";
	hairFile << "    <value name=\"toolVersion\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"sourcePath\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"authorName\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"lastModified\" type=\"String\"></value>\n";
	hairFile << "  </struct>\n";
	hairFile << "</value>\n";
	hairFile << "<value name=\"\" type=\"Ref\" className=\"HairSceneDescriptor\" version=\"1.1\" checksum=\"\">\n";
	hairFile << "  <struct name=\"\">\n";
	hairFile << "    <value name=\"densityTexture\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"rootColorTexture\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"tipColorTexture\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"widthTexture\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"rootWidthTexture\" null=\"1\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"tipWidthTexture\" null=\"1\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"stiffnessTexture\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"rootStiffnessTexture\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"clumpScaleTexture\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"clumpRoundnessTexture\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"clumpNoiseTexture\" null=\"1\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"waveScaletexture\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"waveFreqTexture\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"strandTexture\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"lengthTexture\" type=\"String\"></value>\n";
	hairFile << "    <value name=\"specularTexture\" type=\"String\"></value>\n";
	hairFile << "  </struct>\n";
	hairFile << "</value>\n";
	hairFile << "<value name=\"\" type=\"Ref\" className=\"HairAssetDescriptor\" version=\"1.1\" checksum=\"\">\n";
	hairFile << "  <struct name=\"\">\n";
	hairFile << "    <value name=\"numGuideHairs\" type=\"U32\"></value>\n"; // TODO: Num guidehairs
	hairFile << "    <value name=\"numVertices\" type=\"U32\"></value>\n"; // TODO: fill Num vertices
	hairFile << "    <array name=\"vertices\" size=\"\" type=\"Vec3\">\n"; // TODO: size
	// TODO: add vertices
	hairFile << "    </array>\n";

	hairFile << "    <array name=\"endIndices\" size=\"\" type=\"U32\">\n"; // TODO: size
	// TODO: add endindices
	hairFile << "    </array>\n";

	hairFile << "    <value name=\"numFaces\" type=\"U32\"></value>\n"; // TODO: fill numFaces
	hairFile << "    <array name=\"faceIndices\" size=\"\" type=\"U32\">\n"; // TODO: size
	// TODO: add faceindices
	hairFile << "    </array>\n";

	hairFile << "    <array name=\"faceUVs\" size=\"\" type=\"Vec2\">\n"; // TODO: size
	// TODO: add faceUVs
	hairFile << "    </array>\n";

	hairFile << "    <value name=\"numBones\" type=\"U32\"></value>\n"; // TODO: fill numBones
	hairFile << "    <array name=\"boneIndices\" size=\"\" type=\"Vec4\">\n"; // TODO: size
	// TODO: add boneindices
	hairFile << "    </array>\n";

	hairFile << "    <array name=\"boneWeights\" size=\"\" type=\"Vec4\">\n"; // TODO: size
	// TODO: add boneweights
	hairFile << "    </array>\n";

	hairFile << "    <array name=\"boneNames\" size=\"\" type=\"U8\">\n"; // TODO: size
	// TODO: add bonenames
	hairFile << "    </array>\n";
	hairFile << "    <array name=\"boneNameList\" size=\"\" type=\"String\">\n"; // TODO: size
	// TODO: add bonenamelist
	hairFile << "    </array>\n";

	hairFile << "    <array name=\"bindPoses\" size=\"\" type=\"Mat44\">\n"; // TODO: size
	// TODO: add bindposes
	hairFile << "    </array>\n";
	hairFile << "    <array name=\"boneParents\" size=\"\" type=\"I32\">\n"; // TODO: size
	// TODO: add boneparents
	hairFile << "    </array>\n";

	hairFile << "    <value name=\"numBoneSpheres\" type=\"U32\"></value>\n"; // TODO: fill numBoneSpheres
	hairFile <<
		"    <array name=\"boneSpheres\" size=\"\" type=\"Struct\" structElements=\"boneSphereIndex(I32),boneSphereRadius(F32),boneSphereLocalPos(Vec3)\">\n";
	// TODO: size
	//TODO add boneSpheres
	hairFile << "    </array>\n";

	hairFile << "    <value name=\"numBoneCapsules\" type=\"U32\"></value>\n"; // TODO: fill numBoneCapsules
	hairFile << "    <array name=\"boneCapsuleIndices\" size=\"\" type=\"U32\">\n"; // TODO size
	// TODO add boncapsuleIndices
	hairFile << "    </array>\n";

	hairFile << "    <value name=\"numPinConstraints\" type=\"U32\">0</value>\n"; // TODO was ist das und folgende
	hairFile <<
		"    <array name=\"pinConstraints\" size=\"0\" type=\"Struct\" structElements=\"boneSphereIndex(I32),boneSphereRadius(F32),boneSphereLocalPos(Vec3),pinStiffness(F32),influenceFallOff(F32),useDynamicPin(Bool),doLra(Bool),useStiffnessPin(Bool),influenceFallOffCurve(Vec4)\"></array>\n";
	hairFile << "    <value name=\"sceneUnit\" type=\"F32\">1</value>\n";
	hairFile << "    <value name=\"upAxis\" type=\"U32\">1</value>\n";
	hairFile << "    <value name=\"handedness\" type=\"U32\">1</value>\n";
	hairFile << "  </struct>\n";
	hairFile << "</value>\n";
	hairFile << "<value name=\"\" type=\"Ref\" className=\"HairInstanceDescriptor\" version=\"1.1\" checksum=\"\">\n";
	hairFile << "  <struct name=\"\">\n";
	hairFile << "    <array name=\"materials\" size=\"4\" type=\"Struct\">\n";
	for (int i = 0; i < 4; ++i)
	{
		hairFile << "      <struct>\n";
		hairFile << "        <value name=\"name\" null=\"1\" type=\"String\"></value>\n";
		hairFile << "        <value name=\"densityTextureChan\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"widthTextureChan\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"rootWidthTextureChan\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"tipWidthTextureChan\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"clumpScaleTextureChan\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"clumpNoiseTextureChan\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"clumpRoundnessTextureChan\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"waveScaleTextureChan\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"waveFreqTextureChan\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"lengthTextureChan\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"stiffnessTextureChan\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"rootStiffnessTextureChan\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"splineMultiplier\" type=\"U32\">1</value>\n";
		hairFile << "        <value name=\"assetType\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"assetPriority\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"assetGroup\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"width\" type=\"F32\">0.25</value>\n";
		hairFile << "        <value name=\"widthNoise\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"clumpNoise\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"clumpNumSubclumps\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"clumpRoundness\" type=\"F32\">1</value>\n";
		hairFile << "        <value name=\"clumpScale\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"clumpPerVertex\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"density\" type=\"F32\">0.150000006</value>\n";
		hairFile << "        <value name=\"lengthNoise\" type=\"F32\">1</value>\n";
		hairFile << "        <value name=\"lengthScale\" type=\"F32\">1</value>\n";
		hairFile << "        <value name=\"widthRootScale\" type=\"F32\">1</value>\n";
		hairFile << "        <value name=\"widthTipScale\" type=\"F32\">0.100000001</value>\n";
		hairFile << "        <value name=\"waveRootStraighten\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"waveScale\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"waveScaleNoise\" type=\"F32\">0.5</value>\n";
		hairFile << "        <value name=\"waveFreq\" type=\"F32\">1</value>\n";
		hairFile << "        <value name=\"waveFreqNoise\" type=\"F32\">0.5</value>\n";
		hairFile << "        <value name=\"waveScaleStrand\" type=\"F32\">1</value>\n";
		hairFile << "        <value name=\"waveScaleClump\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"enableDistanceLOD\" type=\"Bool\">true</value>\n";
		hairFile << "        <value name=\"distanceLODStart\" type=\"F32\">10</value>\n";
		hairFile << "        <value name=\"distanceLODEnd\" type=\"F32\">40</value>\n";
		hairFile << "        <value name=\"distanceLODFadeStart\" type=\"F32\">1000</value>\n";
		hairFile << "        <value name=\"distanceLODDensity\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"distanceLODWidth\" type=\"F32\">1</value>\n";
		hairFile << "        <value name=\"enableDetailLOD\" type=\"Bool\">true</value>\n";
		hairFile << "        <value name=\"detailLODStart\" type=\"F32\">7</value>\n";
		hairFile << "        <value name=\"detailLODEnd\" type=\"F32\">1</value>\n";
		hairFile << "        <value name=\"detailLODDensity\" type=\"F32\">0.100000001</value>\n";
		hairFile << "        <value name=\"detailLODWidth\" type=\"F32\">0.200000003</value>\n";
		hairFile << "        <value name=\"colorizeLODOption\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"useViewfrustrumCulling\" type=\"Bool\">true</value>\n";
		hairFile << "        <value name=\"useBackfaceCulling\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"backfaceCullingThreshold\" type=\"F32\">-0.200000003</value>\n";
		hairFile << "        <value name=\"usePixelDensity\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"alpha\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"strandBlendScale\" type=\"F32\">1</value>\n";
		hairFile << "        <value name=\"baseColor\" type=\"Vec4\">0 0 0 0</value>\n";
		hairFile << "        <value name=\"diffuseBlend\" type=\"F32\">0.5</value>\n";
		hairFile << "        <value name=\"diffuseScale\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"diffuseHairNormalWeight\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"diffuseBoneIndex\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"diffuseBoneLocalPos\" type=\"Vec3\">0 0 0</value>\n";
		hairFile << "        <value name=\"diffuseNoiseFreqU\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"diffuseNoiseFreqV\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"diffuseNoiseScale\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"diffuseNoiseGain\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"textureBrightness\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"diffuseColor\" type=\"Vec4\">0 0 0 0</value>\n";
		hairFile <<
			"        <value name=\"rootColor\" type=\"Vec4\">0.0509803928 0.0156862754 0.0156862754 1</value>\n";
		hairFile << "        <value name=\"tipColor\" type=\"Vec4\">0.121568628 0.0509803928 0.0509803928 1</value>\n";
		hairFile << "        <value name=\"glintStrength\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"glintCount\" type=\"F32\">256</value>\n";
		hairFile << "        <value name=\"glintExponent\" type=\"F32\">2</value>\n";
		hairFile << "        <value name=\"rootAlphaFalloff\" type=\"F32\">0.0500000007</value>\n";
		hairFile << "        <value name=\"rootTipColorWeight\" type=\"F32\">0.439999998</value>\n";
		hairFile << "        <value name=\"rootTipColorFalloff\" type=\"F32\">1</value>\n";
		hairFile << "        <value name=\"shadowSigma\" type=\"F32\">0.200000003</value>\n";
		hairFile <<
			"        <value name=\"specularColor\" type=\"Vec4\">0.400000006 0.443137258 0.474509805 1</value>\n";
		hairFile << "        <value name=\"specularPrimary\" type=\"F32\">0.100000001</value>\n";
		hairFile << "        <value name=\"specularNoiseScale\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"specularEnvScale\" type=\"F32\">0.25</value>\n";
		hairFile << "        <value name=\"specularPrimaryBreakup\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"specularSecondary\" type=\"F32\">0.0500000007</value>\n";
		hairFile << "        <value name=\"specularSecondaryOffset\" type=\"F32\">0.100000001</value>\n";
		hairFile << "        <value name=\"specularPowerPrimary\" type=\"F32\">100</value>\n";
		hairFile << "        <value name=\"specularPowerSecondary\" type=\"F32\">20</value>\n";
		hairFile << "        <value name=\"strandBlendMode\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"useTextures\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"useShadows\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"shadowDensityScale\" type=\"F32\">0.5</value>\n";
		hairFile << "        <value name=\"castShadows\" type=\"Bool\">true</value>\n";
		hairFile << "        <value name=\"receiveShadows\" type=\"Bool\">true</value>\n";
		hairFile << "        <value name=\"backStopRadius\" type=\"F32\">1</value>\n";
		hairFile << "        <value name=\"bendStiffness\" type=\"F32\">0.5</value>\n";
		hairFile << "        <value name=\"interactionStiffness\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"pinStiffness\" type=\"F32\">1</value>\n";
		hairFile << "        <value name=\"collisionOffset\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"useCollision\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"useDynamicPin\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"damping\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"friction\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"massScale\" type=\"F32\">2</value>\n";
		hairFile << "        <value name=\"gravity\" type=\"Vec3\">0 0 -1</value>\n";
		hairFile << "        <value name=\"inertiaScale\" type=\"F32\">1</value>\n";
		hairFile << "        <value name=\"inertiaLimit\" type=\"F32\">100</value>\n";
		hairFile << "        <value name=\"rootStiffness\" type=\"F32\">0.5</value>\n";
		hairFile << "        <value name=\"tipStiffness\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"simulate\" type=\"Bool\">true</value>\n";
		hairFile << "        <value name=\"stiffness\" type=\"F32\">0.5</value>\n";
		hairFile << "        <value name=\"stiffnessStrength\" type=\"F32\">1</value>\n";
		hairFile << "        <value name=\"stiffnessDamping\" type=\"F32\">0.200000003</value>\n";
		hairFile << "        <value name=\"stiffnessCurve\" type=\"Vec4\">1 1 1 1</value>\n";
		hairFile << "        <value name=\"stiffnessStrengthCurve\" type=\"Vec4\">1 1 1 1</value>\n";
		hairFile << "        <value name=\"stiffnessDampingCurve\" type=\"Vec4\">1 1 1 1</value>\n";
		hairFile <<
			"        <value name=\"bendStiffnessCurve\" type=\"Vec4\">1 0.910256386 0.75999999 0.217948735</value>\n";
		hairFile << "        <value name=\"interactionStiffnessCurve\" type=\"Vec4\">1 1 1 1</value>\n";
		hairFile << "        <value name=\"wind\" type=\"Vec3\">0 0 0</value>\n";
		hairFile << "        <value name=\"windNoise\" type=\"F32\">0</value>\n";
		hairFile << "        <value name=\"visualizeBones\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"visualizeBoundingBox\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"visualizeCapsules\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"visualizeControlVertices\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"visualizeCullSphere\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"visualizeDiffuseBone\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"visualizeFrames\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"visualizeGrowthMesh\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"visualizeGuideHairs\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"visualizeHairInteractions\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"visualizeHairSkips\" type=\"U32\">0</value>\n";
		hairFile << "        <value name=\"visualizeLocalPos\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"visualizePinConstraints\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"visualizeShadingNormals\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"visualizeSkinnedGuideHairs\" type=\"Bool\">false</value>\n";
		hairFile << "        <value name=\"drawRenderHairs\" type=\"Bool\">true</value>\n";
		hairFile << "        <value name=\"enable\" type=\"Bool\">true</value>\n";
		hairFile << "      </struct>\n";
	}
	hairFile << "    </array>\n";
	hairFile << "  </struct>\n";
	hairFile << "</value>\n";
	hairFile << "</NvParameters>\n";
}
