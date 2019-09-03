
#include "stdafx.h"
#include "GraphicApi.hpp"

#if BOOST_COMP_MSVC 
# pragma warning(push)
# pragma warning(disable: 4996)
#elif defined BOOST_COMP_GNUC
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <Covellite/Api/Vertex.hpp>
#if BOOST_COMP_MSVC 
# pragma warning(pop)
#elif defined BOOST_COMP_GNUC
# pragma GCC diagnostic pop
#endif

#include <Covellite/Api/Defines.hpp>
#include "Component.hpp"

namespace covellite
{

namespace api
{

namespace renderer
{

GraphicApi::GraphicApi(void) :
  m_ServiceComponents{ m_Components },
  m_StartProgram{ std::chrono::system_clock::now() },
  m_CurrentFrameTime{ 0.0f }
{
  m_Creators =
  {
    {
      uT("Data"), [=](const ComponentPtr_t & _pComponent)
      {
        m_Components.push_back(_pComponent);
        return Render_t{};
      }
    },
    { uT("Camera"), [=](const ComponentPtr_t & _pComponent)
      { return CreateCamera(_pComponent); } },
    { uT("State"), [=](const ComponentPtr_t & _pComponent)
      { return CreateState(_pComponent); } },
    { uT("Fog"), [=](const ComponentPtr_t & _pComponent)
      { return CreateFog(_pComponent); } },
    { uT("Material"), [=](const ComponentPtr_t & _pComponent)
      { return CreateMaterial(_pComponent); } },
    { uT("Light"), [=](const ComponentPtr_t & _pComponent)
      { return CreateLight(_pComponent); } },
    { uT("Texture"), [=](const ComponentPtr_t & _pComponent)
      { return CreateTexture(_pComponent); } },
    { uT("Shader"), [=](const ComponentPtr_t & _pComponent)
      { return CreateShader(_pComponent); } },
    { uT("Buffer"), [=](const ComponentPtr_t & _pComponent)
      { return CreateBuffer(_pComponent); } },
    { uT("Present"), [=](const ComponentPtr_t & _pComponent)
      { return CreatePresent(_pComponent); } },
    { uT("Updater"), [this](const ComponentPtr_t & _pComponent)
      { return CreateUpdater(_pComponent); } },
  };
}

void GraphicApi::PresentFrame(void) /*override*/
{
  const ::std::chrono::duration<float> Time =
    (std::chrono::system_clock::now() - m_StartProgram);
  m_CurrentFrameTime = Time.count();
}

auto GraphicApi::GetCreators(void) const -> const Creators_t & /*final*/
{
  return m_Creators;
}

auto GraphicApi::CreatePresent(const ComponentPtr_t & _pComponent) -> Render_t
{
  ::std::map<String_t, ::std::function<Render_t(void)>> Creators =
  {
    { uT("Geometry"), [&](void) { return CreateGeometry(_pComponent); } },
  };

  return Creators[_pComponent->Kind]();
}

auto GraphicApi::CreateUpdater(const ComponentPtr_t & _pComponent) const -> Render_t
{
  using ::covellite::api::Updater_t;

  static const auto hFunctionName =
    ::covellite::api::Component::GetHash(uT("function"));

  static const Updater_t Empty = [](const float) {};

  // Если переданный функциональный объект захватить здесь, то оверхед
  // будет меньше (55kk против 12kk вызовов в секунду), но даже так оверхед
  // достаточно мал, чтобы оказывать какое-либо существенное влияние
  // (updater'ов в приципе не может быть много).

  return [=](void)
  {
    // Создание здесь объекта, а не ссылки позволяет релизовать замену
    // функции обратного вызова внутри самой функции обратного вызова.
    const auto Updater =
      _pComponent->GetValue<const Updater_t &>(hFunctionName, Empty);

    Updater(m_CurrentFrameTime);
  };
}

} // namespace renderer

} // namespace api

} // namespace covellite
