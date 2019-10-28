
#include "stdafx.h"
#include "BasementWindow.hpp"
#include "Basements/Dummy.hpp"
#include "Basements/Simple3DObject.hpp"
#include "Basements/Simple2DGame.hpp"
#include "Basements/Demo.hpp"
#include "Layers/Demo.hpp"

BasementWindow::BasementWindow(const WindowApi_t & _WindowApi) :
  m_WindowApi(_WindowApi),
  m_Events(_WindowApi),
  m_pBasement(::std::make_shared<::basement::Dummy>())
{
  m_Events[::events::Basement.Stop].Connect([&](void)
  {
    m_pBasement = ::std::make_shared<::basement::Dummy>();
  });

  m_Events[::covellite::events::Drawing.Do].Connect([&](void)
  {
    m_pBasement->Render();
  });

  // ************************** Simple3DObject ****************************** //

  m_Events[::events::Simple3DObject.Start].Connect(
    [&](const ::std::pair<int, int> & _Info)
  { 
    m_pBasement = ::std::make_shared<::basement::Simple3DObject>(
      m_WindowApi.GetRenders(), _Info.first, _Info.second);
  });

  m_Events[::events::Simple3DObject.LightsChanged].Connect(
    [&](const int & _Info)
  {
    m_pBasement->Notify(::events::Simple3DObject.LightsChanged, _Info);
  });

  // **************************** Simple2DGame ****************************** //

  auto pRectSimple2DGame = ::std::make_shared<::basement::Simple2DGame::Rect>();

  m_Events[::events::Simple2DGame.Rect].Connect(
    [&, pRectSimple2DGame](const ::std::vector<float> & _Rect)
  {
    (*pRectSimple2DGame) = { _Rect[0], _Rect[1], _Rect[2], _Rect[3] };
  });

  m_Events[::events::Simple2DGame.Start].Connect(
    [&, pRectSimple2DGame](void)
  {
    m_pBasement = ::std::make_shared<::basement::Simple2DGame>(
      m_Events, m_WindowApi.GetRenders(), *pRectSimple2DGame);
  });

  // ******************************** Demo ********************************** //

  m_Events[::events::Demo.Start].Connect(
    [=](void)
  {
    m_pBasement = ::std::make_shared<::basement::Demo>(
      m_Events, m_WindowApi.GetRenders());

    const auto WindowRect = m_WindowApi.GetClientRect();
    m_Events[::events::Demo.Resize](
      ::std::pair<int, int>{ WindowRect.Width, WindowRect.Height });
  });

  m_Events[::covellite::events::Window.Resize].Connect(
    [=](void)
  {
    const auto WindowRect = m_WindowApi.GetClientRect();
    m_Events[::events::Demo.Resize](
      ::std::pair<int, int>{ WindowRect.Width, WindowRect.Height });
  });
}

BasementWindow::operator const WindowApi_t & (void) const
{
  return m_WindowApi;
}
