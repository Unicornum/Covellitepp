
#pragma once
#include <alicorn\settings.mock.hpp>
#include <Covellite\Core\Settings.hpp>

namespace covellite
{

namespace core
{

inline /*static*/ const Settings & Settings::GetInstance(void)
{
  static const Settings Instance;
  return Instance;
}

inline auto Settings::GetFrameworkSection(void) const -> Section_t
{
  Section_t m_FrameworkSection{ uT("MockSection") };
  SetDefaultValues(m_FrameworkSection);
  return m_FrameworkSection;
}

inline /*static*/ void Settings::SetDefaultValues(Section_t &)
{

}

} // namespace core

} // namespace covellite
