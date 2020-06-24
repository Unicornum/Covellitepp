
#include "stdafx.h"
#include "Animations.hpp"
#include <alicorn/std/string/encoding.hpp>

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
  const String_t Name = 
    (*_pAnimation)[uT("name")].Default(uT("Unknown"));
  const float TicksPerSecond = 
    (*_pAnimation)[uT("tps")].Default(1.0f);
  const animation::Frames_t RawFrames = 
    (*_pAnimation)[uT("frames")].Default(animation::Frames_t{});

  const auto pAnimation = Get(Name, TicksPerSecond, RawFrames);

  static const ::std::map<String_t, ::glm::mat4> TransformBonesDummy;
  const ::std::map<String_t, ::glm::mat4> TransformBones =
    (*_pTransform)[uT("bones")].Default(TransformBonesDummy);

  using animation::Skeleton_t;
  const Skeleton_t & Skeleton = 
    (*_pSkin)[uT("skeleton")].Default(Skeleton_t{});

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

  return [=](::covellite::api::Vertex * _pData)
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

    const Skeleton_t & Skeleton =
      (*_pSkin)[uT("skeleton")].Default(Skeleton_t{});

    auto Frame = pAnimation->GetFrame(*piLastFrame, Skeleton);
    if (Frame.empty()) return false;

    if (Frame.size() == Skeleton.size())
    {
      ::std::map<String, ::glm::mat4> TransformBones;

      for (const auto & Bone : UsingBones)
      {
        TransformBones[Bone.first] = Frame[Bone.second];
      }

      (*_pTransform)[uT("bones")] = TransformBones;
    }

    using animation::Skin_t;
    const Skin_t & SkinVertexes = (*_pSkin)[uT("vertexes")].Default(Skin_t{});

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

        DestVertex.px = Position.x;
        DestVertex.py = Position.y;
        DestVertex.pz = Position.z;
        DestVertex.ex = Normal.x;
        DestVertex.ey = Normal.y;
        DestVertex.ez = Normal.z;
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
      // cppcheck-suppress stlFindInsert
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
      string_cast<String, Encoding::UTF8>(_Skeleton[i].Name);

    if (_TransformBones.find(BoneName) != _TransformBones.end())
    {
      UsingBones.push_back({ BoneName, i });
    }
  }

  return UsingBones;
}

} // namespace model

} // namespace basement
