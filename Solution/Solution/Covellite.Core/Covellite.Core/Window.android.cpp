
#include "stdafx.h"
#include <Covellite\Core\Window.hpp>

using namespace covellite::core;

/**
* \brief
*  ������� ��������� ������ ������-���� ���� ���������.
*  
* \return
*  - Windows: 0.
*  - Android: ������ (� ��������) ������ �����������.
*/
int32_t Window::GetStatusBarHeight(void) const
{
  return covellite::jni::Environment().GetStatusBarHeight();
}

/**
* \brief
*  ������� ���������� ������ ���������.
*  
* \exception std::exception
*  - �������� ���������� (��������� ��. �������� ����������).
*/
void Window::Exit(void) const
{
  using AppInfo_t = ::alicorn::system::platform::AppInfo;
  ANativeActivity_finish(AppInfo_t::Get<ANativeActivity *>());
}
