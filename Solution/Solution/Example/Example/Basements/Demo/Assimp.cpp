
#include "stdafx.h"
#include "Assimp.hpp"
#include <alicorn/logger.hpp>
#include <Covellite/App/Settings.hpp>
#include <Covellite/Gui/Vfs.hpp>

#if BOOST_COMP_MSVC
# pragma warning(push)
# pragma warning(disable: 4996)
# pragma warning(disable: 4389)
# pragma warning(disable: 4127)
# pragma warning(disable: 4706)
# pragma warning(disable: 4244)
# pragma warning(disable: 4459)
# pragma warning(disable: 4456)
# pragma warning(disable: 4701)
# pragma warning(disable: 4702)
# pragma warning(disable: 4458)
# pragma warning(disable: 4267)
# pragma warning(disable: 4100)
# pragma warning(disable: 4457)
#endif

#undef min
#undef max

#define ASSIMP_BUILD_NO_X_IMPORTER
#define ASSIMP_BUILD_NO_AMF_IMPORTER
#define ASSIMP_BUILD_NO_3DS_IMPORTER
#define ASSIMP_BUILD_NO_MD3_IMPORTER
#define ASSIMP_BUILD_NO_MDL_IMPORTER
#define ASSIMP_BUILD_NO_MD2_IMPORTER
#define ASSIMP_BUILD_NO_PLY_IMPORTER
#define ASSIMP_BUILD_NO_ASE_IMPORTER
#define ASSIMP_BUILD_NO_HMP_IMPORTER
#define ASSIMP_BUILD_NO_SMD_IMPORTER
#define ASSIMP_BUILD_NO_MDC_IMPORTER
#define ASSIMP_BUILD_NO_MD5_IMPORTER
#define ASSIMP_BUILD_NO_STL_IMPORTER
#define ASSIMP_BUILD_NO_LWO_IMPORTER
#define ASSIMP_BUILD_NO_DXF_IMPORTER
#define ASSIMP_BUILD_NO_NFF_IMPORTER
#define ASSIMP_BUILD_NO_RAW_IMPORTER
#define ASSIMP_BUILD_NO_SIB_IMPORTER
#define ASSIMP_BUILD_NO_OFF_IMPORTER
#define ASSIMP_BUILD_NO_AC_IMPORTER
#define ASSIMP_BUILD_NO_BVH_IMPORTER
#define ASSIMP_BUILD_NO_IRRMESH_IMPORTER
#define ASSIMP_BUILD_NO_IRR_IMPORTER
#define ASSIMP_BUILD_NO_Q3D_IMPORTER
#define ASSIMP_BUILD_NO_B3D_IMPORTER
#define ASSIMP_BUILD_NO_TERRAGEN_IMPORTER
#define ASSIMP_BUILD_NO_CSM_IMPORTER
#define ASSIMP_BUILD_NO_3D_IMPORTER
#define ASSIMP_BUILD_NO_LWS_IMPORTER
#define ASSIMP_BUILD_NO_OGRE_IMPORTER
#define ASSIMP_BUILD_NO_OPENGEX_IMPORTER
#define ASSIMP_BUILD_NO_MS3D_IMPORTER
#define ASSIMP_BUILD_NO_COB_IMPORTER
#define ASSIMP_BUILD_NO_BLEND_IMPORTER
#define ASSIMP_BUILD_NO_Q3BSP_IMPORTER
#define ASSIMP_BUILD_NO_NDO_IMPORTER
#define ASSIMP_BUILD_NO_IFC_IMPORTER
#define ASSIMP_BUILD_NO_XGL_IMPORTER
#define ASSIMP_BUILD_NO_ASSBIN_IMPORTER
#define ASSIMP_BUILD_NO_GLTF_IMPORTER
#define ASSIMP_BUILD_NO_C4D_IMPORTER
#define ASSIMP_BUILD_NO_3MF_IMPORTER
#define ASSIMP_BUILD_NO_X3D_IMPORTER
#define ASSIMP_BUILD_NO_MMD_IMPORTER
#define ASSIMP_BUILD_NO_STEP_IMPORTER

//#define ASSIMP_BUILD_NO_OBJ_IMPORTER
//#define ASSIMP_BUILD_NO_COLLADA_IMPORTER
//#define ASSIMP_BUILD_NO_FBX_IMPORTER

#ifndef ASSIMP_BUILD_NO_OBJ_IMPORTER
#include <assimp/../../source/ObjFileImporter.cpp>
#include <assimp/../../source/ObjFileParser.cpp>
#include <assimp/../../source/ObjFileMtlImporter.cpp>
#endif

#ifndef ASSIMP_BUILD_NO_COLLADA_IMPORTER
#define desc Collada_desc
#include <assimp/../../source/ColladaLoader.cpp>
#include <assimp/../../source/ColladaParser.cpp>
#include <assimp/../../contrib/irrXML/irrXML.cpp>
#undef desc
#endif

#ifndef ASSIMP_BUILD_NO_FBX_IMPORTER
#define PropertyMap FBX_PropertyMap
#define desc FBX_desc
#include <assimp/../../source/FBXImporter.cpp>
#include <assimp/../../source/FBXParser.cpp>
#include <assimp/../../source/FBXTokenizer.cpp>
#include <assimp/../../source/FBXBinaryTokenizer.cpp>
#include <assimp/../../source/FBXUtil.cpp>
#include <assimp/../../source/FBXDocument.cpp>
#include <assimp/../../source/FBXProperties.cpp>
#include <assimp/../../source/FBXNodeAttribute.cpp>
#include <assimp/../../source/FBXModel.cpp>
#include <assimp/../../source/FBXMaterial.cpp>
#include <assimp/../../source/FBXAnimation.cpp>
#include <assimp/../../source/FBXDeformer.cpp>
#include <assimp/../../source/FBXMeshGeometry.cpp>
#include <assimp/../../source/FBXConverter.cpp>
#include <assimp/../../source/FBXDocumentUtil.cpp>
#undef PropertyMap
#undef desc
#endif

#include <assimp/../../source/ImporterRegistry.cpp>
#include <assimp/../../source/Importer.cpp>
#include <assimp/../../source/BaseImporter.cpp>
#include <assimp/../../source/Assimp.cpp>
#include <assimp/../../source/Version.cpp>
#include <assimp/../../source/MaterialSystem.cpp>
#include <assimp/../../source/Scene.cpp>
#include <assimp/../../source/DefaultLogger.cpp>
#include <assimp/../../source/BaseProcess.cpp>
#include <assimp/../../source/ScenePreprocessor.cpp>
#include <assimp/../../source/DefaultIOSystem.cpp>
#include <assimp/../../source/DefaultIOStream.cpp>
#include <assimp/../../source/ValidateDataStructure.cpp>
#include <assimp/../../source/ProcessHelper.cpp>
#include <assimp/../../source/SpatialSort.cpp>
#include <assimp/../../source/CInterfaceIOWrapper.cpp>
#include <assimp/../../source/CreateAnimMesh.cpp>
#include <assimp/../../source/SkeletonMeshBuilder.cpp>

#define ASSIMP_BUILD_NO_CALCTANGENTS_PROCESS
//#define ASSIMP_BUILD_NO_JOINVERTICES_PROCESS
#define ASSIMP_BUILD_NO_MAKELEFTHANDED_PROCESS
#define ASSIMP_BUILD_NO_FLIPUVS_PROCESS
#define ASSIMP_BUILD_NO_FLIPWINDINGORDER_PROCESS
#define ASSIMP_BUILD_NO_TRIANGULATE_PROCESS
#define ASSIMP_BUILD_NO_DROPFACENORMALS_PROCESS
#define ASSIMP_BUILD_NO_GENFACENORMALS_PROCESS
#define ASSIMP_BUILD_NO_GENVERTEXNORMALS_PROCESS
//#define ASSIMP_BUILD_NO_REMOVEVC_PROCESS
#define ASSIMP_BUILD_NO_SPLITLARGEMESHES_PROCESS
#define ASSIMP_BUILD_NO_PRETRANSFORMVERTICES_PROCESS
//#define ASSIMP_BUILD_NO_LIMITBONEWEIGHTS_PROCESS
//#define ASSIMP_BUILD_NO_VALIDATEDS_PROCESS
//#define ASSIMP_BUILD_NO_IMPROVECACHELOCALITY_PROCESS
#define ASSIMP_BUILD_NO_FIXINFACINGNORMALS_PROCESS
//#define ASSIMP_BUILD_NO_REMOVE_REDUNDANTMATERIALS_PROCESS
#define ASSIMP_BUILD_NO_EMBEDTEXTURES_PROCESS
//#define ASSIMP_BUILD_NO_FINDINVALIDDATA_PROCESS
//#define ASSIMP_BUILD_NO_FINDDEGENERATES_PROCESS
#define ASSIMP_BUILD_NO_SORTBYPTYPE_PROCESS
#define ASSIMP_BUILD_NO_GENUVCOORDS_PROCESS
#define ASSIMP_BUILD_NO_TRANSFORMTEXCOORDS_PROCESS
//#define ASSIMP_BUILD_NO_FINDINSTANCES_PROCESS
//#define ASSIMP_BUILD_NO_OPTIMIZEMESHES_PROCESS
//#define ASSIMP_BUILD_NO_OPTIMIZEGRAPH_PROCESS
#define ASSIMP_BUILD_NO_SPLITBYBONECOUNT_PROCESS
#define ASSIMP_BUILD_NO_DEBONE_PROCESS
#define ASSIMP_BUILD_NO_GLOBALSCALE_PROCESS

#include <assimp/../../source/PostStepRegistry.cpp>

#ifndef ASSIMP_BUILD_NO_JOINVERTICES_PROCESS
#include <assimp/../../source/JoinVerticesProcess.cpp>
#endif

#ifndef ASSIMP_BUILD_NO_REMOVEVC_PROCESS
#include <assimp/../../source/RemoveVCProcess.cpp>
#endif

#ifndef ASSIMP_BUILD_NO_IMPROVECACHELOCALITY_PROCESS
#include <assimp/../../source/ImproveCacheLocality.cpp>
#include <assimp/../../source/VertexTriangleAdjacency.cpp>
#endif

#ifndef ASSIMP_BUILD_NO_REMOVE_REDUNDANTMATERIALS_PROCESS
#include <assimp/../../source/RemoveRedundantMaterials.cpp>
#endif

#ifndef ASSIMP_BUILD_NO_FINDINVALIDDATA_PROCESS
#include <assimp/../../source/FindInvalidDataProcess.cpp>
#endif

#ifndef ASSIMP_BUILD_NO_FINDDEGENERATES_PROCESS
#include <assimp/../../source/FindDegenerates.cpp>
#endif

#ifndef ASSIMP_BUILD_NO_FINDINSTANCES_PROCESS
#include <assimp/../../source/FindInstancesProcess.cpp>
#endif

#ifndef ASSIMP_BUILD_NO_OPTIMIZEMESHES_PROCESS
#include <assimp/../../source/OptimizeMeshes.cpp>
#include <assimp/../../source/SceneCombiner.cpp>
#endif

#ifndef ASSIMP_BUILD_NO_OPTIMIZEGRAPH_PROCESS
#include <assimp/../../source/OptimizeGraph.cpp>
#endif

#ifndef ASSIMP_BUILD_NO_LIMITBONEWEIGHTS_PROCESS
#include <assimp/../../source/LimitBoneWeightsProcess.cpp>
#endif

#ifndef ASSIMP_BUILD_NO_DEBONE_PROCESS
#include <assimp/../../source/DeboneProcess.cpp>
#endif

#if BOOST_COMP_MSVC
# pragma warning(pop)
# endif

namespace basement
{

namespace model
{

Assimp::Assimp(const Path_t & _FileName) :
  m_pScene(LoadFile(_FileName, m_Importer))
{
  LOGGER(Trace) << "Load file: " << _FileName;
}

void Assimp::LoadSkeleton(const BoneListener_t & _BoneListener)
{
  LOGGER(Trace) << "  Load skeleton:";

  const auto BoneLocals = LoadBoneLocals(m_pScene);
  LOGGER(Trace) << "    Mesh bones count: " << BoneLocals.size();

  LoadSkeleton(_BoneListener, BoneLocals, m_pScene->mRootNode);
}

void Assimp::LoadGeometry(const FaceListener_t & _FaceListener)
{
  LOGGER(Trace) << "  Load geometry:";

  LoadSkin(_FaceListener, m_pScene, m_pScene->mRootNode);
}

float Assimp::LoadAnimation(const FrameListener_t & _FrameListener)
{
  LOGGER(Trace) << "  Load animation:";

  if (!m_pScene->HasAnimations())
  {
    LOGGER(Trace) << "    No animations detected!";
    return 1.0f;
  }

  LOGGER(Trace) << "    Animation count: " << m_pScene->mNumAnimations;

  const auto * const pAnimation = m_pScene->mAnimations[0];

  for (unsigned int iBone = 0; iBone < pAnimation->mNumChannels; iBone++)
  {
    const auto * const pBone = pAnimation->mChannels[iBone];

    const auto BoneFrames = BuildFrames(
      ::std::vector<aiVectorKey>{
        pBone->mPositionKeys, pBone->mPositionKeys + pBone->mNumPositionKeys},
      ::std::vector<aiQuatKey>{
        pBone->mRotationKeys, pBone->mRotationKeys + pBone->mNumRotationKeys},
      ::std::vector<aiVectorKey>{
        pBone->mScalingKeys, pBone->mScalingKeys + pBone->mNumScalingKeys});

    _FrameListener({ pBone->mNodeName.C_Str(), BoneFrames });
  }

  return (pAnimation->mTicksPerSecond != 0.0) ?
    static_cast<float>(pAnimation->mTicksPerSecond) : 1.0f;
}

inline /*static*/ ::glm::mat4 Assimp::aiMatrix4x4ToGlm(const aiMatrix4x4 & from)
{
  ::glm::mat4 to;
  //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
  to[0][0] = from.a1; to[1][0] = from.a2;	to[2][0] = from.a3; to[3][0] = from.a4;
  to[0][1] = from.b1; to[1][1] = from.b2;	to[2][1] = from.b3; to[3][1] = from.b4;
  to[0][2] = from.c1; to[1][2] = from.c2;	to[2][2] = from.c3; to[3][2] = from.c4;
  to[0][3] = from.d1; to[1][3] = from.d2;	to[2][3] = from.d3; to[3][3] = from.d4;
  return to;
}

/*static*/ const ::aiScene * Assimp::LoadFile(
  const Path_t & _FileName, 
  ::Assimp::Importer & _Importer)
{
  using ::covellite::app::Settings_t;
  using namespace ::alicorn::extension::std;

  const auto PathToMeshDirectory =
    Settings_t::GetInstance().Get<Path_t>(uT("PathToMeshDirectory"));

  const auto PathToMeshFile = PathToMeshDirectory / "demo" / _FileName;

  const auto RawData =
    ::covellite::gui::Vfs_t::GetInstance().GetData(PathToMeshFile);

  // ‘лаги дл€ использовани€ aiProcess_RemoveComponent
  _Importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS,
    aiComponent_TANGENTS_AND_BITANGENTS |
    aiComponent_COLORS |
    aiComponent_LIGHTS |
    aiComponent_CAMERAS |
    aiComponent_TEXTURES |
    0);

  //_Importer.SetPropertyInteger(AI_CONFIG_PP_LBW_MAX_WEIGHTS, 1);

  const auto * const pScene = _Importer.ReadFileFromMemory(
    reinterpret_cast<const char *>(RawData.data()),
    static_cast<unsigned int>(RawData.size()),
    aiProcess_ValidateDataStructure |
    aiProcess_FindInvalidData |
    aiProcess_JoinIdenticalVertices |
    aiProcess_RemoveComponent |
    aiProcess_ImproveCacheLocality |
    //aiProcess_LimitBoneWeights |
    //aiProcess_FindDegenerates |
    //aiProcess_FindInstances |
    //aiProcess_OptimizeMeshes |
    //aiProcess_RemoveRedundantMaterials |
    //aiProcess_OptimizeGraph |
    0, "");

  if (pScene == nullptr)
  {
    throw STD_EXCEPTION << PathToMeshFile << "(): failed : " <<
      _Importer.GetErrorString();
  }

  return pScene;
}

/*static*/ Assimp::BoneLocals_t Assimp::LoadBoneLocals(const aiScene * const _pScene)
{
  BoneLocals_t Result;

  for (unsigned int iMesh = 0; iMesh < _pScene->mNumMeshes; iMesh++)
  {
    const auto * const pMesh = _pScene->mMeshes[iMesh];

    for (unsigned int iBone = 0; iBone < pMesh->mNumBones; iBone++)
    {
      const auto * const pBone = pMesh->mBones[iBone];

      Result[pBone->mName.C_Str()] = pBone->mOffsetMatrix;
    }
  }

  return Result;
}

/*static*/ void Assimp::LoadSkeleton(
  const BoneListener_t & _BoneListener,
  const BoneLocals_t & _BoneInfos,
  const aiNode * const _pNode,
  const ::glm::mat4 & _ParentTransformation)
{
  const auto Transformation =
    _ParentTransformation * aiMatrix4x4ToGlm(_pNode->mTransformation);

  const auto itBone = _BoneInfos.find(_pNode->mName.C_Str());
  if (itBone != _BoneInfos.cend())
  {
    _BoneListener(Bone{ itBone->first, _pNode->mParent->mName.C_Str(),
      Transformation, aiMatrix4x4ToGlm(itBone->second) });
  }

  for (unsigned int iChildrenNode = 0; iChildrenNode < _pNode->mNumChildren; iChildrenNode++)
  {
    LoadSkeleton(_BoneListener, _BoneInfos,
      _pNode->mChildren[iChildrenNode], Transformation);
  }
}

Assimp::BoneWeights_t Assimp::LoadBoneWeights(const aiMesh * const _pMesh)
{
  if (!_pMesh->HasBones()) return {};

  BoneWeights_t Result;

  for (unsigned int iBone = 0; iBone < _pMesh->mNumBones; iBone++)
  {
    const auto * pBone = _pMesh->mBones[iBone];

    for (unsigned int iWeight = 0; iWeight < pBone->mNumWeights; iWeight++)
    {
      const auto & Weight = pBone->mWeights[iWeight];

      // »гнорироваие весов вершин, вклад которых в движение мал:
      // - 0.2f увеличивает количество обрабатываемых объектов в ~1.2 раза.
      if (Weight.mWeight < 0.05f) continue;

      Result[Weight.mVertexId].push_back({ pBone->mName.C_Str(), Weight.mWeight });
    }
  }

  float MinBoneWeight = 1.0f;
  size_t NumBones = 0;
  size_t MaxNumBonesPerVertex = 0;

  for (auto & BoneInfos : Result)
  {
    float Summ = 0.0f;

    for (const auto & Info : BoneInfos.second)
    {
      Summ += Info.second;
    }

    for (auto & Info : BoneInfos.second)
    {
      Info.second *= 1.0f / Summ;

      MinBoneWeight = ::std::min(MinBoneWeight, Info.second);
      NumBones++;
    }

    MaxNumBonesPerVertex = ::std::max(MaxNumBonesPerVertex,
      BoneInfos.second.size());
  }

  LOGGER(Trace) << "      MinBoneWeight: " << MinBoneWeight;
  LOGGER(Trace) << "      MaxNumBonesPerVertex: " << MaxNumBonesPerVertex;
  LOGGER(Trace) << "      Middle bones per vertex: " << NumBones / (float)Result.size();

  return Result;
}

void Assimp::LoadSkin(
  const FaceListener_t & _FaceListener,
  const aiScene * _pScene,
  const aiNode * _pNode)
{
  for (unsigned int n = 0; n < _pNode->mNumMeshes; ++n)
  {
    const auto * pMesh = _pScene->mMeshes[_pNode->mMeshes[n]];

    using namespace ::alicorn::extension::std;

    const auto RawMaterialName = string_cast<String, Locale::UTF8>(
      _pScene->mMaterials[pMesh->mMaterialIndex]->GetName().C_Str());
    const auto MaterialName = RawMaterialName.Replace(uT("_"), uT("."));

    LOGGER(Trace) << "    Mesh: " << MaterialName;
    LOGGER(Trace) << "      Vertex count: " << pMesh->mNumVertices;
    LOGGER(Trace) << "      Faces count: " << pMesh->mNumFaces;

    auto Weights = LoadBoneWeights(pMesh);

    for (unsigned int t = 0; t < pMesh->mNumFaces; ++t)
    {
      const aiFace * pFace = &pMesh->mFaces[t];
      if (pFace->mNumIndices != 3) continue;

      Face FaceInfo;
      FaceInfo.MaterialName = MaterialName;

      for (unsigned int i = 0; i < pFace->mNumIndices; i++)
      {
        const auto Index = pFace->mIndices[i];

        // ’еши совпадающих вршин совпадают.
        FaceInfo.Vertexes[i].Hash = static_cast<uint64_t>((n << 16) | Index);

        if (pMesh->HasPositions())
        {
          FaceInfo.Vertexes[i].Position = pMesh->mVertices[Index];
        }

        if (pMesh->HasNormals())
        {
          FaceInfo.Vertexes[i].Normal = pMesh->mNormals[Index];
        }

        if (pMesh->HasTextureCoords(0))
        {
          FaceInfo.Vertexes[i].TexCoords = pMesh->mTextureCoords[0][Index];
        }

        for (const auto & Weight : Weights[Index])
        {
          FaceInfo.Vertexes[i].WeightBones.push_back(Weight);
        }
      }

      _FaceListener(FaceInfo);
    }
  }

  for (unsigned int n = 0; n < _pNode->mNumChildren; ++n)
  {
    LoadSkin(_FaceListener, _pScene, _pNode->mChildren[n]);
  }
}

Assimp::Frames::Keys_t Assimp::BuildFrames(
  const ::std::vector<aiVectorKey> & _PositionKeys,
  const ::std::vector<aiQuatKey> & _RotationKeys,
  const ::std::vector<aiVectorKey> & _ScalingKeys)
{
  // 07 »юль 2019 10:10 (unicornum.verum@gmail.com)
  TODO("«десь можно насоздавать любое количество интерполированных кадров анимации");

  const auto FrameCount = ::std::max(_PositionKeys.size(),
    ::std::max(_RotationKeys.size(), _ScalingKeys.size()));

  Frames::Keys_t Result{ FrameCount };

  // ƒл€ использовани€ OpenMP:
  // - GCC - и компил€тору и линковщику добавить параметры '-fopenmp'.
  // - MSVS - в настройках проекта (C\C++ -> язык) включить поддержку OpenMP
//#   pragma omp parallel for
  for (int i = 0; i < static_cast<int>(FrameCount); i++)
  {
    float Time = 0.0f;
    aiVector3D Translation{ 0.0f, 0.0f, 0.0f };
    aiQuaternion Rotation{ 0.0f, 0.0f, 0.0f };
    aiVector3D Scaling{ 1.0f, 1.0f, 1.0f };

    if (!_PositionKeys.empty())
    {
      const auto PositionCount = static_cast<int>(_PositionKeys.size());

      const auto iFrame = (i < PositionCount) ? i : PositionCount - 1;
      Translation = _PositionKeys[iFrame].mValue;
      Time = static_cast<float>(_PositionKeys[iFrame].mTime);
    }

    if (!_RotationKeys.empty())
    {
      const auto RotationCount = static_cast<int>(_RotationKeys.size());

      const auto iFrame = (i < RotationCount) ? i : RotationCount - 1;
      Rotation = _RotationKeys[iFrame].mValue;
    }

    if (!_ScalingKeys.empty())
    {
      const auto ScalingCount = static_cast<int>(_ScalingKeys.size());

      const auto iFrame = (i < ScalingCount) ? i : ScalingCount - 1;
      Scaling = _ScalingKeys[iFrame].mValue;
    }

    // 07 »юль 2019 10:47 (unicornum.verum@gmail.com)
    TODO("”читывать врем€ каждого кадра (врем€ может отличатьс€?)");
    Result[i] = { Time, aiMatrix4x4ToGlm({ Scaling, Rotation, Translation }) };
  }

  return Result;
}

} // namespace model

} // namespace basement
