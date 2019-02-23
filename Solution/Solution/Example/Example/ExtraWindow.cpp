﻿
#include "stdafx.h"
#include "ExtraWindow.hpp"
#include "Basements/Dummy.hpp"
#include "Basements/Simple3DObject.hpp"
#include "Basements/Simple2DGame.hpp"

ExtraWindow::ExtraWindow(const WindowApi_t & _WindowApi) :
  m_WindowApi(_WindowApi),
  m_Events(_WindowApi),
  m_pBasement(::std::make_shared<::basement::Dummy>())
{
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

  m_Events[::events::Simple2DGame.Start].Connect(
    [&](const ::std::vector<float> & _Rect)
  {
    const ::basement::Simple2DGame::Rect Rect =
    { 
      _Rect[0], _Rect[1], _Rect[2], _Rect[3]
    };

    m_pBasement = ::std::make_shared<::basement::Simple2DGame>(
      m_Events, m_WindowApi.GetRenders(), Rect);
  });

  m_Events[::events::Basement.Stop].Connect([&](void) 
  { 
    m_pBasement = ::std::make_shared<::basement::Dummy>();
  });

  m_Events[::covellite::events::Drawing.Do].Connect([&](void)
  {
    m_pBasement->Render();
  });
}

ExtraWindow::operator const WindowApi_t & (void) const
{
  return m_WindowApi;
}
