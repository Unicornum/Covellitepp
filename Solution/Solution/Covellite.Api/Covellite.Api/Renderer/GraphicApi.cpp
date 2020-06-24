
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
    { uT("Transform"), [this](const ComponentPtr_t & _pComponent)
      { return CreateTransform(_pComponent); } },
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
  m_IsResizeWindow = false;
}

auto GraphicApi::GetCreators(void) const noexcept -> const Creators_t & /*final*/
{
  return m_Creators;
}

auto GraphicApi::CreatePresent(const ComponentPtr_t & _pComponent) -> Render_t
{
  if (_pComponent->Kind == uT("Geometry"))
  {
    return CreateGeometry(_pComponent);
  }

  // 13 �������� 2019 12:20 (unicornum.verum@gmail.com)
  TODO("��� �������� ����������� ���� �������� ����� ����� �� CreateBuffer()");
  return CreatePresentBuffer(_pComponent);
}

auto GraphicApi::CreateUpdater(const ComponentPtr_t & _pComponent) const -> Render_t
{
  using ::covellite::api::Updater_t;

  static const auto hFunctionName =
    ::covellite::api::Component::GetHash(uT("function"));

  static const Updater_t Empty = [](const float) {};

  // ���� ���������� �������������� ������ ��������� �����, �� �������
  // ����� ������ (55kk ������ 12kk ������� � �������), �� ���� ��� �������
  // ���������� ���, ����� ��������� �����-���� ������������ �������
  // (updater'�� � ������� �� ����� ���� �����).

  return [=](void)
  {
    // �������� ����� �������, � �� ������ ��������� ���������� ������
    // ������� ��������� ������ ������ ����� ������� ��������� ������.
    const Updater_t Updater =
      (*_pComponent)[hFunctionName].Default(Empty);

    Updater(m_CurrentFrameTime);
  };
}

} // namespace renderer

} // namespace api

} // namespace covellite
