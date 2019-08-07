
#include "stdafx.h"
#include "Animations.hpp"

namespace basement
{

namespace model
{

auto Animated::Animations::CreateAnimation(
  const ComponentPtr_t & _pSkin,
  const ComponentPtr_t & _pAnimation,
  const ComponentPtr_t & _pTransform,
  const int _SkipAnimationFrames) -> BufferMapper_t
{
  namespace vertex = ::covellite::api::vertex;

  const auto Name = _pAnimation->GetValue(uT("name"), uT("Unknown"));
  const auto TicksPerSecond = _pAnimation->GetValue(uT("tps"), 1.0f);
  const auto RawFrames = _pAnimation->GetValue(uT("frames"), animation::Frames_t{});

  const auto pAnimation = Get(Name, TicksPerSecond, RawFrames);

  static const ::std::map<String_t, ::glm::mat4> TransformBonesDummy;
  const auto TransformBones =
    _pTransform->GetValue(uT("bones"), TransformBonesDummy);

  using animation::Skeleton_t;
  static const Skeleton_t SkeletonDummy;
  const auto & Skeleton =
    _pSkin->GetValue<const Skeleton_t &>(uT("skeleton"), SkeletonDummy);

  const auto UsingBones = Get(TransformBones, Skeleton);

  static const auto Random = [](const float _From, const float _To)
  {
    static ::std::mt19937 Generator{ ::std::random_device{}() };
    return ::std::uniform_real_distribution<float>{ _From, _To }(Generator);
  };

  const auto DeltaTime = Random(0.0f, 1.0f);

  using namespace ::std::chrono;

  const auto StartTime = system_clock::now();

  using namespace ::alicorn::extension::std;

  auto piLastFrameTemp = ::alicorn::extension::std::pool<64>::
    make_unique<::std::size_t>(static_cast<::std::size_t>(-1));
  const auto piLastFrame = make_shared(piLastFrameTemp);

  static const auto hRawData = ::std::hash<String_t>{}(uT("data"));

  return [=](vertex::Polyhedron * _pData)
  {
    if (_pData == nullptr)
    {
      const auto Time = duration_cast<milliseconds>(
        system_clock::now() - StartTime).count();

      const auto iFrame = pAnimation->GetFrameIndex(DeltaTime + Time / 1000.0f);
      const auto dFrame =
        static_cast<int>(*piLastFrame) - static_cast<int>(iFrame);
      if (abs(dFrame) <= _SkipAnimationFrames) return false;

      *piLastFrame = iFrame;
      return true;
    }

    const auto & Skeleton =
      _pSkin->GetValue<const Skeleton_t &>(uT("skeleton"), SkeletonDummy);

    auto Frame = pAnimation->GetFrame(*piLastFrame, Skeleton);
    if (Frame.empty()) return false;

    if (Frame.size() == Skeleton.size())
    {
      ::std::map<String, ::glm::mat4> TransformBones;

      for (const auto & Bone : UsingBones)
      {
        TransformBones[Bone.first] = Frame[Bone.second];
      }

      _pTransform->SetValue(uT("bones"), TransformBones);
    }

    using animation::Skin_t;
    static const Skin_t SkinDummy;
    const auto & SkinVertexes =
      _pSkin->GetValue<const Skin_t &>(uT("vertexes"), SkinDummy);

    const auto VertexCount = static_cast<int>(SkinVertexes.size());

    // ƒл€ использовани€ OpenMP:
    // - GCC - и компил€тору и линковщику добавить параметры '-fopenmp'.
    // - MSVS - в настройках проекта (C\C++ -> язык) включить поддержку OpenMP
//#     pragma omp parallel for
    for (int i = 0; i < VertexCount; i++)
    {
      const auto & SrcVertex = SkinVertexes[i];

      ::glm::mat4 Transform{ 0.0f };

      for (const auto & Bone : SrcVertex.WeightBones)
      {
        Transform +=
          //Skeleton[Bone.first].Global * Skeleton[Bone.first].Local *
          Frame[Bone.first] *
          Bone.second;
      }

      const auto Position = Transform * SrcVertex.Position;
      const auto Normal = Transform * SrcVertex.Normal;

      for (const auto & iDestVertex : SrcVertex.iCompleteVertexes)
      {
        auto & DestVertex = _pData[iDestVertex];

        DestVertex.x = Position.x;
        DestVertex.y = Position.y;
        DestVertex.z = Position.z;
        DestVertex.nx = Normal.x;
        DestVertex.ny = Normal.y;
        DestVertex.nz = Normal.z;
      }
    }

    return false;
  };
}

auto Animated::Animations::Get(
  const String_t & _Name, 
  const float _TicksPerSecond, 
  const animation::Frames_t & _Frames) -> AnimationPtr_t
{
  if (m_Animations.find(_Name) == m_Animations.end())
  {
    using Animation_t = ::basement::model::Animation;

    m_Animations[_Name] = 
      ::std::make_shared<Animation_t>(_TicksPerSecond, _Frames);
  }

  return m_Animations[_Name];
}

auto Animated::Animations::Get(
  const ::std::map<String_t, ::glm::mat4> & _TransformBones,
  const animation::Skeleton_t & _Skeleton) -> UsingBones_t
{
  UsingBones_t UsingBones;

  for (::std::size_t i = 0; i < _Skeleton.size(); i++)
  {
    using namespace ::alicorn::extension::std;

    const auto BoneName =
      string_cast<String, Locale::UTF8>(_Skeleton[i].Name);

    if (_TransformBones.find(BoneName) != _TransformBones.end())
    {
      UsingBones.push_back({ BoneName, i });
    }
  }

  return UsingBones;
}

} // namespace model

} // namespace basement
