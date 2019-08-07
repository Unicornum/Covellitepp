
#include "stdafx.h"
#include "Updater.hpp"
#include <Covellite/Api/Defines.hpp>
#include "Component.hpp"

using namespace covellite::api::renderer;

Updater::Updater(void) :
  m_StartProgram(std::chrono::system_clock::now()),
  m_CurrentFrameTime(0.0f)
{

}

auto Updater::GetCreator(void) const -> CreatorPair_t
{
  return { uT("Updater"), [this](const ComponentPtr_t & _pComponent)
    { return CreateUpdater(_pComponent); } };
}

void Updater::UpdateTime(void)
{
  const ::std::chrono::duration<float> Time =
    (std::chrono::system_clock::now() - m_StartProgram);
  m_CurrentFrameTime = Time.count();
}

auto Updater::CreateUpdater(const ComponentPtr_t & _pComponent) const -> Render_t
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
    const auto Updater =
      _pComponent->GetValue<const Updater_t &>(hFunctionName, Empty);

    Updater(m_CurrentFrameTime);
  };
}
