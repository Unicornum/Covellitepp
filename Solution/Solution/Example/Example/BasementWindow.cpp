
#include "stdafx.h"
#include "BasementWindow.hpp"
#include "Basements/Dummy.hpp"
#include "Basements/Empty.hpp"
#include "Basements/Simple3DObject.hpp"
#include "Basements/Simple2DGame.hpp"
#include "Basements/Demo.hpp"
#include "Layers/Demo.hpp"

BasementWindow::BasementWindow(
  const WindowApi_t & _WindowApi, 
  WindowExpanse_t & _Window) :
  m_Events(_WindowApi),
  m_pBasement(::std::make_shared<::basement::Dummy>())
{
  const auto * const pWindowApi = &_WindowApi;
  auto * const pWindowExpanse = &_Window;

  m_Events[::covellite::events::Drawing.Do].Connect([=](void)
  {
    m_pBasement->Render();
  });

  // **************************** Simple2DGame ****************************** //

  auto pRectSimple2DGame = ::std::make_shared<::basement::Simple2DGame::Rect>();

  m_Events[::events::Simple2DGame.Rect].Connect(
    [=](const ::std::vector<float> & _Rect)
  {
    (*pRectSimple2DGame) = { _Rect[0], _Rect[1], _Rect[2], _Rect[3] };
  });

  m_Events[::events::Simple2DGame.Start].Connect(
    [=](void)
  {
    //m_pBasement = ::std::make_shared<::basement::Empty>(*pWindowExpanse);

    m_pBasement = ::std::make_shared<::basement::Simple2DGame>(
      *pWindowExpanse, *pRectSimple2DGame);
  });

  // ************************** Simple3DObject ****************************** //

  m_Events[::events::Simple3DObject.Start].Connect(
    [=](const ::std::pair<int, int> & _Info)
  {
    m_pBasement = ::std::make_shared<::basement::Simple3DObject>(
      *pWindowExpanse, _Info.first, _Info.second);
  });

  m_Events[::events::Simple3DObject.LightsChanged].Connect(
    [&](const int & _Info)
  {
    m_pBasement->Notify(::events::Simple3DObject.LightsChanged, _Info);
  });

  // ******************************** Demo ********************************** //

  m_Events[::events::Demo.Start].Connect(
    [=](void)
  {
    m_pBasement = ::std::make_shared<::basement::Demo>(
      m_Events, pWindowApi->GetRenders());

    const auto WindowRect = pWindowApi->GetClientRect();
    m_Events[::events::Demo.Resize](
      ::std::pair<int, int>{ WindowRect.Width, WindowRect.Height });
  });

  m_Events[::covellite::events::Window.Resize].Connect(
    [=](void)
  {
    const auto WindowRect = pWindowApi->GetClientRect();
    m_Events[::events::Demo.Resize](
      ::std::pair<int, int>{ WindowRect.Width, WindowRect.Height });
  });

  // ******************************* Exit *********************************** //

  m_Events[::events::Basement.Stop].Connect([=](void)
  {
    m_pBasement = ::std::make_shared<::basement::Dummy>();
  });
}
