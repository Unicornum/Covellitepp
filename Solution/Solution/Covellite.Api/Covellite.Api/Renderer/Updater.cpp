
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
