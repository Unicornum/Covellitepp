
#include "stdafx.h"
#include "GraphicApi.hpp"
#include <Covellite/Api/Vertex.hpp>
#include <Covellite/Api/Defines.hpp>
#include <Covellite/Api/Component.inl>

namespace covellite
{

namespace api
{

namespace renderer
{

GraphicApi::GraphicApi(void) :
  m_StartProgram{ std::chrono::system_clock::now() },
  m_CurrentFrameTime{ 0.0f }
{
  m_Creators =
  {
    { uT("Test"), [=](const ComponentPtr_t &)
      { return [](void) {}; } },
    { uT("Camera"), [=](const ComponentPtr_t & _pComponent)
      { return CreateCamera(_pComponent); } },
    { uT("BkSurface"), [=](const ComponentPtr_t & _pComponent)
      { return CreateBkSurface(_pComponent); } },
    { uT("State"), [=](const ComponentPtr_t & _pComponent)
      { return CreateState(_pComponent); } },
    { uT("Texture"), [=](const ComponentPtr_t & _pComponent)
      { return CreateTexture(_pComponent); } },
    { uT("TextureArray"), [=](const ComponentPtr_t & _pComponent)
      { return CreateTextureArray(_pComponent); } },
    { uT("Shader"), [=](const ComponentPtr_t & _pComponent)
      { return CreateShader(_pComponent); } },
    { uT("Buffer"), [=](const ComponentPtr_t & _pComponent)
      { return CreateBuffer(_pComponent); } },
    { uT("Transform"), [this](const ComponentPtr_t & _pComponent)
      { return CreateTransform(_pComponent); } },
    { uT("Present"), [=](const ComponentPtr_t & _pComponent)
      { return CreatePresentBuffer(_pComponent); } },
    { uT("Updater"), [this](const ComponentPtr_t & _pComponent)
      { return CreateUpdater(_pComponent); } },
  };
}

void GraphicApi::PresentFrame(void) /*override*/
{
  const ::std::chrono::duration<float> Time =
    (std::chrono::system_clock::now() - m_StartProgram);
  m_CurrentFrameTime = Time.count();
  m_IsResizeWindow = false;
}

auto GraphicApi::GetCreators(void) const noexcept -> const Creators_t & /*final*/
{
  return m_Creators;
}

auto GraphicApi::CreateUpdater(const ComponentPtr_t & _pComponent) const -> Render_t
{
  using ::covellite::api::Updater_t;
  using ::covellite::api::Component;

  static const auto hFunctionName = Component::GetHash(uT("function"));
  static const Updater_t Empty = [](const float) {};

  // 11.10.2020: Вероятно, это уже неактуально после замены интерфейса:
  // Если переданный функциональный объект захватить здесь, то оверхед
  // будет существенно меньше (55kk против 12kk вызовов в секунду), но даже так
  // оверхед достаточно мал, чтобы оказывать какое-либо существенное влияние
  // на время рендеринга кадра (updater'ов в приципе не может быть много).

  return [=](void)
  {
    // Создание здесь объекта, а не ссылки позволяет релизовать замену
    // функции обратного вызова внутри самой функции обратного вызова.
    const Updater_t Updater = (*_pComponent)[hFunctionName].Default(Empty);

    Updater(m_CurrentFrameTime);
  };
}

} // namespace renderer

} // namespace api

} // namespace covellite
