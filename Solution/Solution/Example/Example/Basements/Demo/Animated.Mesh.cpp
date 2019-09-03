
#include "stdafx.h"
#include "Animated.hpp"
#include "Animations.hpp"
#include <alicorn/boost/format.inl>
#include <alicorn/std/vector.hpp>
#include <alicorn/logger.hpp>
#include "Assimp.hpp"

namespace basement
{

namespace model
{

Animated::Animation::Animation(const Path_t & _FileName)
{
  Assimp Loader{ _FileName };

  m_TicksPerSecond = Loader.LoadAnimation([this](const Assimp::Frames & _Frames)
  {
    // В файлах .dae точки в названиях костей заменяются на символ 
    // подчеркивания.
    const auto BoneName = _Frames.BoneName.substr(0, _Frames.BoneName.find("_"));

    if (m_RawFrames.size() < _Frames.Keys.size())
    {
      m_RawFrames.resize(_Frames.Keys.size());
    }

    const auto FramesSize = static_cast<int>(_Frames.Keys.size());

    // Для использования OpenMP:
    // - GCC - и компилятору и линковщику добавить параметры '-fopenmp'.
    // - MSVS - в настройках проекта (C\C++ -> Язык) включить поддержку OpenMP
//# pragma omp parallel for
    for (int i = 0; i < FramesSize; i++)
    {
      m_RawFrames[i].first = _Frames.Keys[i].first;
      m_RawFrames[i].second[BoneName].Value = _Frames.Keys[i].second;
    }
  });
}

Object_t Animated::Animation::GetObject(void) const
{
  return 
  {
    Component_t::Make(
    {
      //{ uT("type"), uT("Animation") },
      { uT("type"),   uT("Data") },
      { uT("kind"),   uT("Animation") },
      { uT("name"),   uT("Walking") },
      { uT("tps"),    m_TicksPerSecond },
      { uT("frames"), m_RawFrames },
    }),
  };
}

Animated::Mesh::Mesh(const Path_t & _FileName)
{
  BuildBasementObject(1.0f);

  Assimp Loader{ _FileName };

  Loader.LoadSkeleton([this](const Assimp::Bone & _Bone)
  {
    m_Skeleton.Add(_Bone.Name, _Bone.ParentName, 
      _Bone.GlobalTransform, _Bone.LocalTransform);
  });

  LOGGER(Trace) << "    Skeleton bones count: " << m_Skeleton.GetBones().size();

  const Rect TextureCoord{ 0.0f, 1.0f, 1.0f, 0.0f };

  Loader.LoadGeometry([this, &TextureCoord](const Assimp::Face & _Face)
  {
    Triangle_t Triangle;
    Triangle.MaterialName = _Face.MaterialName;

    TriangleWeightBones TriangleWeightBones;

    for (int i = 0; i < 3; i++)
    {
      Triangle.Vertexes[i].Hash = _Face.Vertexes[i].Hash;

      Triangle.Vertexes[i].m_Vertex.px = _Face.Vertexes[i].Position.x;
      Triangle.Vertexes[i].m_Vertex.py = _Face.Vertexes[i].Position.y;
      Triangle.Vertexes[i].m_Vertex.pz = _Face.Vertexes[i].Position.z;
      Triangle.Vertexes[i].m_Vertex.pw = 1.0f;

      Triangle.Vertexes[i].m_Vertex.ex = _Face.Vertexes[i].Normal.x;
      Triangle.Vertexes[i].m_Vertex.ey = _Face.Vertexes[i].Normal.y;
      Triangle.Vertexes[i].m_Vertex.ez = _Face.Vertexes[i].Normal.z;
      Triangle.Vertexes[i].m_Vertex.ew = 0.0f;

      const auto Convert =
        [](const float _Source, const float _Min, const float _Max)
      {
        namespace math = ::alicorn::extension::cpp::math;

        const float Source = 0.999999f * _Source;
        const auto Fractional = Source - math::Floor(Source);
        const auto Result = Fractional * (_Max - _Min) + _Min;
        return Result;
      };

      Triangle.Vertexes[i].m_Vertex.tu = Convert(_Face.Vertexes[i].TexCoords.x,
        TextureCoord.left, TextureCoord.right);
      Triangle.Vertexes[i].m_Vertex.tv = Convert(_Face.Vertexes[i].TexCoords.y,
        TextureCoord.top, TextureCoord.bottom);

      for (const auto & Weight : _Face.Vertexes[i].WeightBones)
      {
        const auto iBone = m_Skeleton.GetBoneIndex(Weight.first);
        TriangleWeightBones.WeightBones[i].push_back({ iBone, Weight.second });
      }
    }

    Add(Triangle, TriangleWeightBones);
  });

  LOGGER(Trace) << "    All vertex count: " << GetAllVertexCount();
  LOGGER(Trace) << "    Skin vertex count: " << GetSkinVertexCount();
}

Object_t Animated::Mesh::GetObject(const Any_t & _Value) const /*override*/
{
  const auto Value = ::boost::any_cast<const Value_t &>(_Value);

  const auto Position = Value.Coords.ToPlane();
  const auto SquareDistance = Position.first * Position.first +
    Position.second * Position.second;

  // Оптимизация с пропуском кадров в зависимсоти от расстояния до камеры
  // увеличивает fps в ~1.5-2 раза.
  const auto SkipFrames = 
    (SquareDistance <= 4.0f) ? 0 : 
    (SquareDistance <= 9.0f) ? 1 : 
    (SquareDistance <= 16.0f) ? 2 : 
    (SquareDistance <= 25.0f) ? 4 : 8;

  static size_t Index = 0;
  Index++;

  using ::alicorn::extension::boost::Format;

  // Использование здесь String::Replace() увеличивает время формирования
  // идентификатора в 10(!) раз, что существенно сказывается
  // на скорости загрузки сцены.
  const auto Id = (Format{ uT(".Vertex.%1%") } % Index).ToString();

  const auto HoldInBoneName = uT("mixamorig:RightHandIndex2");

  if (m_pVertexData == nullptr)
  {
    m_pVertexData = Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Buffer") },
        { uT("data"), m_Vertexes.data() },
        { uT("count"), m_Vertexes.size() },
      });
  }

  if (m_pSkinData == nullptr)
  {
    m_pSkinData = Component_t::Make(
      {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Skin") },
      { uT("vertexes"), m_SkinVertexes },
      { uT("skeleton"), m_Skeleton.GetBones() },
      });
  }

  if (m_pTransformBones == nullptr)
  {
    m_pTransformBones = Component_t::Make(
      {
        { uT("type"), uT("Data") },
        { uT("kind"), uT("Transform") },
        { uT("bones"), BoneTransforms_t { { HoldInBoneName, ::glm::mat4{ 1.0f } } } },
      });
  }

  if (m_pAnimations == nullptr)
  {
    m_pAnimations = ::std::make_shared<Animations>();
  }

  const auto cbVertexData = m_pAnimations->CreateAnimation(
    m_pSkinData, Value.pAnimation, m_pTransformBones, SkipFrames);

  Object_t Result =
  {
    m_pVertexData,
    Component_t::Make(
    {
      { uT("id"), m_MeshId + Id },
      { uT("type"), uT("Buffer") },
      { uT("mapper"), cbVertexData },
    }),
  };

  using namespace ::alicorn::extension::std;

  for (const auto & Material : m_MaterialIndices)
  {
    Result += Object_t
    {
      Component_t::Make(
      {
        { uT("id"), m_MeshId + uT(".Index.") + Material.first },
        { uT("type"), uT("Buffer") },
        { uT("data"), static_cast<const int *>(Material.second.data()) },
        { uT("count"), Material.second.size() },
      }),
    };

    Result += Value.Material(Material.first);
  }

  if (::alicorn::extension::cpp::IS_DEBUG_CONFIGURATION)
  {
    Result += GetHoldInHandObject(HoldInBoneName);
  }

  return Result;
}

Object_t Animated::Mesh::GetHoldInHandObject(
  const String_t & _HoldBoneName) const
{
  using Updater_t = ::covellite::api::Updater_t;

  const auto & HoldInHand = m_MaterialIndices[uT("Human.Head.png")];

  const auto pHolInHandPosition = Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
    });

  const auto pHolInHandRotation = Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Rotation") },
    });

  // Захватывать нужно локальный объект, переменная класса не будет захвачена
  // отдельно от this.
  const auto pTransformBones = m_pTransformBones;

  const Updater_t HoldInHandUpdater = [=](const float /*_Time*/)
  {
    auto BoneTransforms =
      pTransformBones->GetValue(uT("bones"), BoneTransforms_t{});

    const auto tfrmHand = BoneTransforms[_HoldBoneName];

    // Вершина на поверхности ладони (в Blender'e в режиме редактирования
    // выбрать вершину и нажать N).
    const ::glm::vec4 HandPosition{ 0.70857f, 0.04504f, 1.14949f, 1.0f };

    const auto Translation = tfrmHand * HandPosition;

    pHolInHandPosition->SetValue(uT("x"), Translation.x);
    pHolInHandPosition->SetValue(uT("y"), Translation.y);
    pHolInHandPosition->SetValue(uT("z"), Translation.z);

    const ::glm::vec4 HandRotation{ 0.0f, 0.0f, -1.0f, 0.0f };

    const auto Rotation = tfrmHand * HandRotation;

    pHolInHandRotation->SetValue(uT("x"), Rotation.x);
    pHolInHandRotation->SetValue(uT("y"), Rotation.y);
    pHolInHandRotation->SetValue(uT("z"), Rotation.z);
  };

  return Object_t
  {
    Component_t::Make(
    {
      { uT("id"), uT("Demo.Animated.HoldInHand.Updater") },
      { uT("type"), uT("Updater") },
      { uT("function"), HoldInHandUpdater },
    }),
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Buffer") },
      { uT("data"), static_cast<const Vertex_t *>(m_Vertexes.data()) },
      { uT("count"), m_Vertexes.size() },
    }),
    Component_t::Make(
    {
      { uT("id"), m_MeshId + uT(".Vertex.HoldInHand") },
      { uT("type"), uT("Buffer") },
    }),
    Component_t::Make(
    {
      { uT("id"), m_MeshId + uT(".Index.HoldInHand") },
      { uT("type"), uT("Buffer") },
      { uT("data"), static_cast<const int *>(HoldInHand.data()) },
      { uT("count"), HoldInHand.size() },
    }),
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Position") },
      { uT("x"), 0.0f },
      { uT("y"), 0.1f },
      { uT("z"), -1.8f },
    }),
    Component_t::Make(
    {
      { uT("type"), uT("Data") },
      { uT("kind"), uT("Scale") },
      { uT("x"), 0.5f },
      { uT("y"), 0.5f },
      { uT("z"), 0.5f },
    }),
    pHolInHandRotation,
    pHolInHandPosition,
    Component_t::Make(
    {
      { uT("id"), m_MeshId + uT(".Present.HoldInHand") },
      { uT("type"), uT("Present") },
      { uT("kind"), uT("Geometry") },
    })
  };
}

void Animated::Mesh::Add(
  const Triangle_t & _Triangle, 
  const TriangleWeightBones & _TriangleWeightBones)
{
  const cbVertex_t cbVertex =
    [&](const Vertex & _Vertex, const ::std::size_t _iVertexes)
  {
    const auto iVertex = &_Vertex - &_Triangle.Vertexes[0];
    AddSkin(_Vertex, _iVertexes, _TriangleWeightBones.WeightBones[iVertex]);
  };

  GameObject::Mesh::Add(_Triangle, cbVertex);
}

void Animated::Mesh::AddSkin(
  const Vertex & _Vertex,
  const ::std::size_t _iVertexes,
  const WeightBones_t & _WeightBones)
{
  if (_WeightBones.empty()) return;

  const ::glm::vec4 Position{
    _Vertex.m_Vertex.px, _Vertex.m_Vertex.py, _Vertex.m_Vertex.pz, 1.0f };

  const ::glm::vec4 Normal{
    _Vertex.m_Vertex.ex, _Vertex.m_Vertex.ey, _Vertex.m_Vertex.ez, 0.0f };

  bool IsExist = false;

  for (auto & Vertex : m_SkinVertexes)
  {
    constexpr auto Delta = 1e-6f;

    if (::glm::length(Vertex.Position - Position) <= Delta &&
      ::glm::length(Vertex.Normal - Normal) <= Delta)
    {
      IsExist = true;
      Vertex.iCompleteVertexes.push_back(_iVertexes);
      break;
    }
  }

  if (!IsExist)
  {
    m_SkinVertexes.push_back(
      {
        Position,
        Normal,
        { _iVertexes },
        _WeightBones
      });
  }
}

} // namespace model

} // namespace basement
