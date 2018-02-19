
#include "stdafx.h"
#include <Covellite\Core\Settings.hpp>

using namespace covellite::core;

/**
* \brief
*  ������� ��������� ������������� ������� �������� ����������.
*/
/*static*/ const Settings & Settings::GetInstance(void)
{
  static const Settings Instance;
  return Instance;
}

/**
* \brief
*  ������� ��������� ������� ������� �������� ����������: \n
*  |- Covellitepp
*/
auto Settings::GetFrameworkSection(void) const -> Section_t
{
  Section_t m_FrameworkSection{ uT("Covellitepp") };
  SetDefaultValues(m_FrameworkSection);
  return m_FrameworkSection;
}
