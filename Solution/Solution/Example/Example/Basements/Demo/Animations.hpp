
#pragma once
#include "Animated.hpp"
#include <random>
#include <alicorn/std/memory.hpp>
#include <Covellite/Covellite.hpp>

namespace basement
{

namespace model
{

class Animated::Animations
{
  using AnimationPtr_t = ::std::shared_ptr<::basement::model::Animation>;
  using ComponentPtr_t = ::covellite::api::Component::ComponentPtr_t;
  using BufferMapper_t = 
    ::covellite::api::cbBufferMap_t<::covellite::api::Vertex>;
  using UsingBones_t = ::std::vector<::std::pair<String_t, ::std::size_t>>;

public:
  BufferMapper_t CreateAnimation(const ComponentPtr_t &, const ComponentPtr_t &,
    const ComponentPtr_t &, const int);

private:
  AnimationPtr_t Get(const String_t &, const float, const animation::Frames_t &);
  UsingBones_t Get(const ::std::map<String_t, ::glm::mat4> &,
    const animation::Skeleton_t &);

private:
  ::std::map<String_t, AnimationPtr_t> m_Animations;
};

} // namespace model

} // namespace basement
