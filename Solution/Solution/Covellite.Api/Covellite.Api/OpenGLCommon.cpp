
#include "stdafx.h"
#include <Covellite\Api\OpenGLCommon.hpp>
#include <Covellite\Core\Settings.hpp>

namespace covellite
{

namespace api
{

static OpenGLCommon::Color GetBackgroundColor(void)
{
  const auto CovelliteppSection =
    ::covellite::core::Settings::GetInstance().GetFrameworkSection();
  auto BkColorSection = CovelliteppSection[uT("Window")][uT("BackgroundColor")];

  const auto R = BkColorSection.Get<float>(uT("R")) / 255.0f;
  const auto G = BkColorSection.Get<float>(uT("G")) / 255.0f;
  const auto B = BkColorSection.Get<float>(uT("B")) / 255.0f;
  const auto A = BkColorSection.Get<float>(uT("A")) / 255.0f;

  return OpenGLCommon::Color{ R, G, B, A };
}

/**
* \brief
*  ����������� ������.
*/
OpenGLCommon::OpenGLCommon(void) :
  m_BackgroundColor(GetBackgroundColor())
{

}

/**
* \brief
*  ������� ������� ���� ���������.
* \details
*  - �������� ���� ���������� ������, �������� � ����������
*  (������ Covellitepp -> Window -> BackgroundColor, ��������� RGBA).
*/
void OpenGLCommon::ClearWindow(void)
{
  glDisable(GL_BLEND);

  glClearColor(m_BackgroundColor.R, m_BackgroundColor.G, m_BackgroundColor.B,
    m_BackgroundColor.A);
  glClear(GL_COLOR_BUFFER_BIT);
}

/**
* \brief
*  ������� ��������� ������� ��������� �������� ����.
*  
* \param [in] _Width
*  ����� ������ ����.
* \param [in] _Height
*  ����� ������ ����.
*/
void OpenGLCommon::UpdateWindow(int32_t _Width, int32_t _Height)
{
  glViewport(0, 0, _Width, _Height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  GlOrtho(_Width, _Height);
}

} // namespace api

} // namespace covellite
